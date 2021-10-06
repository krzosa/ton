///////////////////////
// @Section: Data language
enum ASTKind {
  ASTKind_None,
  ASTKind_String,
  ASTKind_Number,
  ASTKind_Object,
  ASTKind_File,
};

struct AST {
  ASTKind kind;
  AST *next;
  AST *prev;
  AST *parent;
  
  AST *first_tag;
  AST *last_tag ;
  bool child_tag; // Tag near key or value: @nearkey key: @nearvalue value
  
  S8 key;
  S8 s8;
  union {
    F64 f64;
    struct {
      AST *first_child;
      AST *last_child;
    };
  };
};

fn AST *new_ast(MArena *arena, S8 key=s8(), ASTKind kind=ASTKind_None) {
  AST *ast = push_struct(arena, AST);
  zero_struct(ast); 
  ast->kind = kind;
  ast->key = key;
  return ast;
}

fn AST *new_string(MArena *arena, S8 key, S8 value) {
  AST *result = new_ast(arena, key, ASTKind_String);
  result->s8  = value;
  return result;
}

fn AST *new_number(MArena *arena, S8 key, F64 value) {
  AST *result = new_ast(arena, key, ASTKind_Number);
  result->f64 = value;
  return result;
}

fn void make_object(AST *ast) {
  ast->kind = ASTKind_Object;
}

fn void make_number(AST *ast, S8 key, S8 value) {
  ast->kind = ASTKind_Number;
  ast->f64 = s8_to_f64(value);
  ast->key = key;
}

fn void make_string(AST *ast, S8 key, S8 value) {
  ast->kind = ASTKind_String;
  ast->key = key;
  ast->s8 = value;
}

fn AST *new_number(MArena *arena, S8 key, S8 value) {
  F64 result = s8_to_f64(value);
  return new_number(arena, key, result);
}

fn AST *new_object(MArena *arena, S8 key) {
  AST *result = new_ast(arena, key, ASTKind_Object);
  return result;
}

fn AST *push_child(AST *parent, AST *push) {
  expect(parent->kind == ASTKind_Object, "Pushing child not on object");
  push->parent = parent;
  if(parent->last_child) {
    parent->last_child->next = push;
    push->prev = parent->last_child;
    parent->last_child = push;
  }
  else {
    parent->first_child = parent->last_child = push;
    push->prev = 0;
  }
  return push;
}

fn AST *get_node(AST *node, S8 key) {
  for(AST *a = node; a; a=a->next) {
    if(equals(a->key, key)) {
      return a;
    }
  }
  return 0;
}

fn AST *get_child(AST *node, S8 key) {
  for(AST *a = node->first_child; a; a=a->next) {
    if(equals(a->key, key)) {
      return a;
    }
  }
  return 0;
}

fn AST *push_tag(AST *parent, AST *tag) {
  tag->parent = parent;
  if(parent->last_tag) {
    parent->last_tag->next = tag;
    tag->prev = parent->last_tag;
    parent->last_tag = tag;
  }
  else {
    parent->first_tag = parent->last_tag = tag;
    tag->prev = 0;
  }
  return tag;
}

///////////////////////////////////////
// @Section: Lexer 
enum TokenKind {
  // Reserved for ASCII tokens
  TokenKind_Identifier=128,
  TokenKind_Number,
  TokenKind_Comment,
};

struct Token {
  TokenKind kind;
  union{
    S8 s; // string
    struct {
      U8 *str;
      U64 len;
    };
  };
  S8 string_raw;
  operator S8() { return s; }
  B32 is_eof(){ return kind == (TokenKind)0; }
  B32 is_identifier(){ return kind == TokenKind_Identifier; }
  B32 is_number(){ return kind == TokenKind_Number; }
  B32 is(char _kind){ return kind == (TokenKind)_kind; }
};

struct Lex {
  S8 filename;
  S8 stream;
  I64 stream_i;
  I64 line;
  I64 column;
  Token last;
};

fn void advance(Lex *x) {
  if(x->stream_i < (I64)x->stream.len) {
    U8 result = *(x->stream.str + x->stream_i++);
    if(result == '\n') {
      x->line++;
      x->column = 1;
    }
    else if(result == ' ') {
      x->column++;
    }
    else if(result == '\t') {
      x->column += 2;
    }
  }
}

inline U8 *current_char(Lex *c) {
  U8 *result = c->stream.str + c->stream_i;
  return result;
}

inline U64 is_eof(Lex *c) {
  if(c->stream_i < (I64)c->stream.len) return false;
  return true;
}

inline B32 is_identifier(Token token, S8 identifier) {
  B32 result = token.kind == TokenKind_Identifier && equals(token.s, identifier);
  return result;
}

inline B32 is_identifier(Token token) {
  B32 result = token.kind == TokenKind_Identifier;
  return result;
}

inline B32 is_kind(Token token, U8 c) {
  B32 result = token.kind == (TokenKind)c;
  return result;
}

inline B32 is_number(Token token) {
  B32 result = token.kind == TokenKind_Number;
  return result;
}

fn Token next_token_raw(Lex *c) {
  Token result = {};
  if(c->stream_i >= (I64)c->stream.len) {
    return result;
  }
  
  B32 is_literal = false;
  result.s.str = current_char(c);
  U8 chr = *result.s.str;
  advance(c);
  switch(chr) {
    case '0':case '4':
    case '1':case '5':
    case '2':case '6':
    case '3':case '7':
    case '9':case '8':{
      result.kind = TokenKind_Number;
      while(is_number(*current_char(c))) {
        advance(c);
      }
      if(*current_char(c) == '.') {
        advance(c);
        while(is_number(*current_char(c))) {
          advance(c);
        }
      }
    } break;
    case 'A': case 'a':case 'M': case 'm':
    case 'B': case 'b':case 'N': case 'n':
    case 'C': case 'c':case 'O': case 'o':
    case 'D': case 'd':case 'P': case 'p':
    case 'E': case 'e':case 'Q': case 'q':
    case 'F': case 'f':case 'R': case 'r':
    case 'G': case 'g':case 'S': case 's':
    case 'H': case 'h':case 'T': case 't':
    case 'I': case 'i':case 'U': case 'u':
    case 'J': case 'j':case 'V': case 'v':
    case 'K': case 'k':case 'W': case 'w':
    case 'L': case 'l':case 'X': case 'x':
    case 'Z': case 'z':case 'Y': case 'y':
    case '_': {
      result.kind = TokenKind_Identifier;
      while(!is_eof(c) && (is_alphanumeric(*current_char(c)) || *current_char(c) == '_')) {
        advance(c);
      }
    } break;
    case '"': case '\'': {
      is_literal = true;
      result.kind = TokenKind_Identifier;
      for(;;) {
        if(is_eof(c)) {
          fatal_error("Incomplete literal! Reached end of file at: %s:%d\n\n", c->filename, c->line);
          break;
        }
        if(*current_char(c) == chr) { advance(c); break; }
        advance(c);
      }
    } break;
    case '/': {
      U8 curr = *current_char(c);
      if(curr == '/') {
        result.kind =  TokenKind_Comment;
        for(;;) {
          advance(c);
          if(is_eof(c) || *current_char(c) == '\n') break;
        }
      }
      else if(curr == '*') {
        result.kind =  TokenKind_Comment;
        B32 star = false;
        for(;;) {
          advance(c);
          curr = *current_char(c);
          if(is_eof(c)) break;
          if(star && curr == '/') { advance(c); break; }
          if(curr == '*') star = true;
          else star = false;
        }
      }
      else {
        result.kind = (TokenKind)chr;
      }
    } break;
    default: {
      if(chr < 128) {
        result.kind = (TokenKind)chr;
      }
    }
  }
  
  result.len = (U64)(current_char(c) - result.str);
  result.string_raw = result.s;
  if(is_literal) {
    result.len-=2;
    result.str++;
  }
  
  return result;
}

fn Token next_token(Lex *c) {
  Token result = {};
  do { 
    result = next_token_raw(c);
  } while(result.kind == ' ' || result.kind == '\n' || 
          result.kind == '\t' || result.kind == '\r' ||
          result.kind == TokenKind_Comment);
  c->last = result;
  return result;
}

fn Token peek_token(Lex *c) {
  Lex cache = *c;
  Token result = next_token(c);
  *c = cache;
  return result;
}

fn B32 expect_token(Lex *c, Token token, TokenKind kind) {
  B32 result = false;
  if(token.kind == kind) {
    result = true;
  }
  else {
    push_log(console, "Error: Expected token of kind: %c." 
             "Instead got token of kind: %c, at around %s:%d\n", 
             kind, token.kind, c->filename, c->line);
  }
  return result;
}

fn B32 expect_tokens(Lex *c, Token token, S8 *identifiers, U32 len) {
  B32 result = false;
  if(token.kind == TokenKind_Identifier) {
    if(match_any_in_list(token.s, identifiers, len)) {
      result = true;
    }
  }
  
  if(!result) {
    console_log("Error: Expected one identifier out of list: [");
    for(U32 i = 0; i < len; i++) {
      console_log("%s", identifiers[i]);
      if(i != len-1) console_log(", ");
    }
    console_log("] Instead got: %s, at around %s:%d\n",
                token.s, c->filename, c->line);
  }
  
  return result;
}

fn B32 expect_tokens(Lex *c, S8 *identifiers, U32 len, Token *out=0) {
  Token peek = peek_token(c);
  if(expect_tokens(c, peek, identifiers, len)) {
    if(out) *out = peek;
    next_token(c);
    return true;
  }
  return false;
}

fn B32 expect_token(Lex *c, Token token, S8 identifier) {
  B32 result = false;
  if(is_identifier(token, identifier)) {
    result = true;
  }
  else {
    push_log(console, "Error: Expected identfier %s." 
             "Instead got: %s at around %s:%d\n", 
             identifier, token.s, c->filename, c->line);
  }
  return result;
}

fn B32 expect_token(Lex *c, Token token, char kind) {
  return expect_token(c, token, (TokenKind)kind);
}

fn B32 match_tokens(Token token, S8 *identifiers, U32 len) {
  B32 result = false;
  if(token.kind == TokenKind_Identifier) {
    if(match_any_in_list(token.s, identifiers, len)) {
      result = true;
    }
  }
  return result;
}

fn B32 match_tokens(Lex *c, S8 *identifiers, U32 len, Token *out=0) {
  Token peek = peek_token(c);
  if(match_tokens(peek, identifiers, len)) {
    if(out) *out = peek;
    next_token(c);
    return true;
  }
  return false;
}

fn B32 match_token(Lex *c, S8 identifier) {
  if(match_tokens(c, &identifier, 1)) {
    return true;
  }
  return false;
}

fn B32 match_token(Lex *c, TokenKind kind, Token *token=0) {
  B32 result = false;
  if(peek_token(c).kind == kind) {
    Token tok = next_token(c);
    if(token) *token = tok;
    result = true;
  }
  return result;
}

fn B32 expect_token(Lex *c, S8 identifier) {
  Token peek = peek_token(c);
  if(expect_token(c, peek, identifier)) {
    next_token(c);
    return true;
  }
  return false;
}

fn B32 expect_token(Lex *c, TokenKind kind) {
  B32 result = false;
  Token peek = peek_token(c);
  if(expect_token(c, peek, kind)) {
    result = true;
    next_token(c);
  }
  
  return result;
}

fn B32 match_token(Lex *c, char kind) {
  return match_token(c,(TokenKind)kind);
}

fn B32 expect_token(Lex *c, char kind) {
  return expect_token(c,(TokenKind)kind);
}

fn void begin_lexing(Lex *c, S8 stream, S8 filename = lit("")) {
  c->line = 1;
  c->column = 1;
  c->filename = filename;
  c->stream_i = 0;
  c->stream = stream;
}

fn Lex begin_lexing(S8 stream, S8 filename = lit("")) {
  Lex result = {};
  begin_lexing(&result, stream, filename);
  return result;
}

inline Token last_token(Lex *c) {
  Token result = c->last;
  return result;
}

//////////////////////
// @Section: Parser
struct Sym {
  AST *node;
  Sym *next;
};

struct MetaCtx {
  MArena *arena;
  Lex lex;
  Sym sym_table[512];
  AST *top; // For parsing, top most ast
  operator MArena*(){ return arena; }
  operator Lex*   (){ return &lex;  }
};

fn void put_sym(MetaCtx *c, AST *node) {
  U64 hash = fnv_hash(node->key)%array_cap(c->sym_table);
  assert(hash<array_cap(c->sym_table));
  expect(node->key.len != 0, "AST Node's key is null, cant put it into sym table!");
  Sym *sym = c->sym_table + hash;
  if(sym->node) {
    for(;;) {
      if(equals(sym->node->key, node->key)) {
        break;
      }
      else if(sym->next) {
        sym = sym->next;
      }
      else {
        sym = sym->next = push_struct(c->arena, Sym, true);
        break;
      }
    }
  }
  sym->node = node;
}

fn AST *get_sym(MetaCtx *c, S8 key) {
  AST *result = 0;
  
  U64 hash = fnv_hash(key)%array_cap(c->sym_table);
  assert(hash<array_cap(c->sym_table));
  for(Sym *sym = c->sym_table + hash;sym;sym=sym->next) {
    if(sym->node) {
      if(equals(sym->node->key, key)) {
        result = sym->node;
        break;
      }
    }
    else {
      break;
    }
  }
  return result;
}

fn AST *has_tag(AST *node, S8 name, B32 child_tag=false) {
  for(AST *tag = node->first_tag; tag; tag=tag->next) {
    if(tag->child_tag == child_tag && equals(name, tag->key)) {
      return tag;
    }
  }
  return 0;
}

fn void populate_sym_table(MetaCtx *c, AST *root) {
  for(AST *node = root; node; node=node->next) {
    if(node->kind == ASTKind_Object) {
      if(has_tag(node, lit("struct")) || has_tag(node, lit("enum"))) {
        put_sym(c, node);
      }
    }
  }
}

fn void parse_identifier(MetaCtx *c, AST *result, Token token);
fn void parse_tag(MetaCtx *c, AST *parent, B32 child_tag) {
  AST *ast = new_ast(*c);
  if(expect_token(*c, TokenKind_Identifier)) {
    parse_identifier(c, ast, last_token(*c));
    ast->child_tag = child_tag;
    push_tag(parent, ast);
  }
}

fn void parse_tags(MetaCtx *c, AST *parent, B32 child_tag) {
  while(match_token(*c, '@')) {
    parse_tag(c, parent, child_tag);
  }
}

fn void parse_block(MetaCtx *c, AST *parent) {
  AST *ast = new_ast(*c); // potentially wasting memory? Do I care?
  parse_tags(c, ast, false);
  
  Token peek = peek_token(*c);
  if(peek.is_number() || peek.is_identifier()) {
    parse_identifier(c, ast, next_token(*c));
    push_child(parent, ast);
  }
  else if(match_token(*c, '{')) {
    make_object(ast);
    parse_block(c, ast);
    expect_token(*c, '}');
    push_child(parent, ast);
  }
  
  if(ast->kind != ASTKind_None && match_token(*c, ',')) {
    peek = peek_token(*c);
    if(peek.is_identifier() || peek.is_number() || peek.is('@') || peek.is('{')) {
      parse_block(c, parent);
    }
  }
}

fn void parse_identifier(MetaCtx *c, AST *result, Token token) {
  result->key = token;
  if(match_token(*c, ':')) {
    parse_tags(c, result, true);
    
    if(match_token(*c, '{')) {
      make_object(result);
      parse_block(c, result);
      expect_token(*c, '}');
    }
    else if(match_token(*c, TokenKind_Number)) {
      make_number(result, result->key, last_token(*c));
    }
    else if(match_token(*c, TokenKind_Identifier)) {
      make_string(result, result->key, last_token(*c));
    }
    else {
      fatal_error("Colon not followed by Identifier, Number or Object %s:%d", c->lex.filename, c->lex.line);
    }
  }
  else {
    if(token.is_number()) {
      make_number(result, s8(), result->key);
    }
    else if(token.is_identifier()) {
      make_string(result, result->key, s8());
    }
  }
}

fn void parse(MetaCtx *c, AST *ast) {
  if(match_token(*c, '{')) {
    make_object(ast);
    parse_block(c, ast);
    expect_token(*c, '}');
  }
  else if(match_token(*c, TokenKind_Identifier) || match_token(*c, TokenKind_Number)) {
    parse_identifier(c, ast, last_token(*c));
  }
  else if(match_token(*c, '@')) {
    do {
      parse_tag(c, ast, false);
    } while(match_token(*c, '@'));
    parse(c, ast);
  }
  else {
    fatal_error("Expected Identifier, Tag or OpenBrace, instead got: %c at: %s:%d", last_token(*c).kind, c->lex.filename, c->lex.line);
  }
}

fn AST *parse(MetaCtx *c, S8 content, S8 filename=s8()) {
  begin_lexing(*c, content, filename);
  AST *result = new_ast(*c, filename, ASTKind_File);
  AST *node = result;
  for(;;) {
    Token token = peek_token(*c);
    if(token.is_eof()) {
      break;
    }
    
    node->next = new_ast(*c);
    node = node->next;
    parse(c, node);
  }
  
  return result;
}

fn MetaCtx begin_parse(MArena *arena) {
  MetaCtx ctx = {};
  ctx.arena = arena;
  return ctx;
}
