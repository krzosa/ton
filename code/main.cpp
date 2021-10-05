#include "base.cpp"
#include "lang.cpp"

fn S8    do_codegen(MetaCtx *c, AST *parent);
fn void _do_codegen(MetaCtx *c, AST *node, I32 nested);
fn void do_codegen_struct_member(MetaCtx *c, AST *node, I32 nested);

fn void do_codegen_struct_member(MetaCtx *c, AST *node, I32 nested) {
  if(node->kind == ASTKind_String) {
    bool is_embed = false;
    if(has_tag(node, lit("embed"), true)) {
      push_string(*c, "union {");
      AST *type = get_sym(c, node->s8);
      S8 name = type->key;
      type->key = lit("");
      _do_codegen(c, type, nested+1);
      type->key = name;
      is_embed = true;
    }
    
    push_string(*c, "%s ", node->s8);
    if(has_tag(node, lit("ptr"), true)) {
      push_string(*c, "*");
    }
    push_string_copy(*c, node->key);
    push_string(*c, ";");
    if(is_embed) {
      push_string(*c, "};");
    }
  }
  else if(node->kind == ASTKind_Object) {
    if(has_tag(node, lit("union"))) {
      push_string(*c, "union {");
    }
    else {
      push_string(*c, "struct {");
    }
    loop_list(node->first_child, child) {
      do_codegen_struct_member(c, child, nested+1);
    }
    push_string(*c, "}%s;", node->key);
  }
}

fn void _do_codegen(MetaCtx *c, AST *node, I32 nested) {
  AST *mapping_enum = has_tag(node, lit("mapping_enum"));
  AST *is_struct    = has_tag(node, lit("struct"));
  switch(node->kind) {
    case ASTKind_Object: {
      if(is_struct) {
        if(!(nested == 0 && equals(is_struct->s8, lit("decl")))) {
          push_string(*c, "struct %s {", node->key);
          loop_list(node->first_child, child) {
            do_codegen_struct_member(c, child, nested+1);
          }
          push_string(*c, "};");
        }
        if(nested == 0) {
          push_string(*c, "MetaVar struct_%s[] = {", node->key);
          loop_list(node->first_child, child) {
            push_string(*c, "{lit(\"%s\"), lit(\"%s\")}", child->s8, child->key);
          }
          push_string(*c, "};\n");
        }
      }
      else if(has_tag(node, lit("enum"))) {
        push_string(*c, "enum %s {", node->key);
        loop_list(node->first_child, child) {
          push_string(*c, "%s_%s, \n", node->key, child->key);
        }
        push_string(*c, "};");
        
        
        push_string(*c, "S8 enum_%s[] = {", node->key);
        loop_list(node->first_child, child) {
          push_string(*c, "lit(\"%s_%s\"), \n", node->key, child->key);
        }
        push_string(*c, "};");
        
      }      
      else if(mapping_enum) {
        push_string(*c, "enum %s {", node->key);
        loop_list(node->first_child, child) {
          push_string(*c, "%s_%s, \n", node->key, child->key);
        }
        push_string(*c, "};\n");
        
        push_string(*c, "#define %s_Mapping \\\n", node->key);
        loop_list(node->first_child, child) {
          if(child->s8.len) {
            push_string(*c, "%s(%s, %s) \\\n", mapping_enum->s8, child->key, child->s8);
          }
        }
        push_string(*c, "\n");
        
        push_string(*c, "S8 enum_%s[] = {", node->key);
        loop_list(node->first_child, child) {
          push_string(*c, "lit(\"%s_%s\"), \n", node->key, child->key);
        }
        push_string(*c, "};");
        
      }
    } break;
    case ASTKind_None: break;
    case ASTKind_String: push_string(*c, "%s", node->key); break;
    case ASTKind_Number: break;
    case ASTKind_File: break;
  }
}

fn S8 do_codegen(MetaCtx *c, AST *parent) {
  U8 *string_pointer = begin_string(*c);
  push_string(*c, "struct MetaVar{ S8 type; S8 name; };");
  loop_list(parent, node) {
    _do_codegen(c, node, 0);
  }
  S8 string = end_string(*c, string_pointer);
  return string;
}

fn S8 prettify_code(MArena *arena, S8 code) {
#define indent() for(I64 i = 0; i < indent; i++) push_string(arena, "  ")
  U8 *string_pointer = begin_string(arena);
  MetaCtx dummy_ctx = {};
  begin_lexing(dummy_ctx, code);
  
  I64 indent = 0;
  for(;;) {
    Token token = next_token_raw(dummy_ctx);
    if(token.is_eof()) {
      break;
    }
    else if(token.kind == ';') {
      push_string(arena, ";\n");
      indent();
    }
    else if(token.kind == '{') {
      indent++;
      push_string(arena, "{\n");
      indent();
    }
    else if(token.kind == '\n') {
      push_string(arena, "\n");
      indent();
    }
    else if(token.kind == '}') {
      push_string(arena, "}");
      indent--;
    }
    else {
      push_string_copy(arena, token.string_raw);
    }
  }
  S8 string = end_string(arena, string_pointer);
  return string;
}

int main() {
  Scratch scratch;
  MetaCtx ctx = begin_parse(scratch);
  S8List files = os_list_files(scratch, lit("../code/"));
  loop_list(files.first, node) {
    if(ends_with(node->string, lit(".data"))) {
      S8 data = os_read_file(scratch, node->string);
      if(data.len) {
        console_log("--- Generating code using: %s ---\n", node->string);
        AST *ddl = parse(&ctx, data, node->string);
        populate_sym_table(&ctx, ddl);
        S8 code = do_codegen(&ctx, ddl);
        code = prettify_code(scratch, code);
        os_write_file(push_string(scratch, "%s/%s.h", lit("../generated/"), pop_extension(get_filename(node->string))), code);
      }
    }
  }
}