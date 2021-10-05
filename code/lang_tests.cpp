#include "base.cpp"
#include "lang.cpp"

#if 0
@struct Lex: {
  filename: S8,
  stream: S8,
  I64 stream_i,
  I64 line,
  I64 column,
}

@data
data: {
  "Cool data string": 123.51525,
  "Object thing": {
    Nice: 123,
    Nicee: "12242",
  },
  List: {1, 2, 3, 4}
}

@function
main: {arg1, arg2}
{
  a: (12*4)+4,
  if: {123 == 123} 
  {
    
  }
  return: a,
}
#endif

unit_test(test_lexing_identifiers) {
  MetaCtx ctx = begin_parse(arena);
  begin_lexing(ctx, lit("TestThing _Test_Thing Test_Thing Test12345"
                        " qwertyuiopzxcvbnm ASDHJKLQUIE125125jfd9fjmn {}:,() // Comment\n Test"
                        "/* Comment */ Test2 'Literal' \"LIT\" 1234.1235"));
  Token token;
#define assert_b(str, _kind) token = next_token(ctx); \
assert(token.kind == TokenKind_##_kind && equals(token.s, lit(str)));
#define assert_identifier(str) assert_b(str, Identifier)
#define assert_is(name) assert(next_token(ctx).is(name));
  // #define assert_comment(str) assert_b(str, Comment)
#define assert_literal(str) assert_b(str, Identifier)
#define assert_num(str) assert_b(str, Number);
#define assert_eof() assert(next_token(ctx).kind == (TokenKind)0);
  assert_identifier("TestThing");
  assert_identifier("_Test_Thing");
  assert_identifier("Test_Thing");
  assert_identifier("Test12345");
  assert_identifier("qwertyuiopzxcvbnm");
  assert_identifier("ASDHJKLQUIE125125jfd9fjmn");
  assert_is('{');
  assert_is('}');
  assert_is(':');
  assert_is(',');
  assert_is('(');
  assert_is(')');
  assert_identifier("Test");
  assert_identifier("Test2");
  assert_literal("Literal");
  assert_literal("LIT");
  assert_num("1234.1235");
  assert_eof();
}

unit_test(test_lexing_numbers) {
  MetaCtx ctx = begin_parse(arena);
  begin_lexing(ctx, lit("1234 15123 98302 1235 123.231, 111.111111"));
  Token token;
  assert(equals(peek_token(ctx).s, lit("1234")));
  assert_num("1234");
  assert_num("15123");
  assert_num("98302");
  assert_num("1235");
  assert_num("123.231");
  assert_is(',');
  assert_num("111.111111");
  assert_eof();
}

unit_test(test_lang) {
  // while(1);
  MetaCtx ctx = begin_parse(arena);
  begin_lexing(ctx, lit("thing: { 'string': inside } // Comment\n asdf /* asdf"));
  Token token;
  assert_identifier("thing");
  assert_is(':');
  assert_is('{');
  assert_literal("string");
  assert_is(':');
  assert_identifier("inside");
  assert_is('}');
  assert_identifier("asdf");
  assert_eof();
}

unit_test(test_ast) {
  AST *object = new_object(arena, lit("Thing"));
  push_tag(object, new_string(arena, lit(""), lit("Tag")));
  push_child(object, new_number(arena, lit("Test"), 1234.424));
  push_child(object, new_string(arena, lit("Test"), lit("Thing")));
  AST *sub = push_child(object, new_object(arena, lit("Test")));
  push_tag(sub, new_string(arena, lit("Cool"), lit("Tag")));
  push_tag(sub, new_object(arena, lit("two")));
  push_child(sub, new_string(arena, lit(""), lit("el1")));
  push_child(sub, new_string(arena, lit(""), lit("el2")));
  push_child(sub, new_string(arena, lit(""), lit("el4")));
  //pretty_print(object);
}

unit_test(test_parse_identifier) {
  S8 data[] = {
    lit("list: {element1, element2, element3}"),
    lit("object: { memes: thing, memes: thing2 }"),
    lit("object: { '123141': 1230123, numbers: 12351.5124 }"),
    lit("object: { '123141': { thing, thing2, thing3: 'asdafa asdas' }, numbers: 12351.5124 }"),
    lit("object: { '123141': { thing, thing2, thing3: 'asdafa asdas' }, @thing:{1,2,3} numbers: 12351.5124 }"),
  };
  {
    MetaCtx c = begin_parse(arena);
    begin_lexing(c, data[0]);
    AST *ast = new_ast(c);
    parse_identifier(&c, ast, next_token(c));
    assert(ast->kind == ASTKind_Object && equals(ast->key, lit("list")));
    assert(ast->first_child->kind == ASTKind_String && equals(ast->first_child->key, lit("element1")));
    assert(ast->first_child->next->kind == ASTKind_String && equals(ast->first_child->next->key, lit("element2")));
    assert(ast->first_child->next->next->kind == ASTKind_String);
  }
  
  for(U64 i = 0; i < array_cap(data); i++) {
    MetaCtx c = begin_parse(arena);
    begin_lexing(c, data[i]);
    AST *ast = new_ast(c);
    parse_identifier(&c, ast, next_token(c));
    //pretty_print(ast);
  }
}

unit_test(test_parse) {
  S8 data[] = {
    lit("list: {element1, element2, element3}"),
    lit("object: { memes: thing, memes: thing2 }"),
    lit("object: { '123141': 1230123, numbers: 12351.5124 }"),
    lit("object: { '123141': { thing, thing2, thing3: 'asdafa asdas' }, numbers: 12351.5124 }"),
    lit("@tag @thing object: { '123141': { thing, thing2, thing3: 'asdafa asdas' }, @thing:{1,2,3} numbers: 12351.5124 }"),
    lit(R"FOO(
                                                                                                          @tag
                                                                                                          @thing
                                                                                                          object: {
                                                                                                            123141: {
                                                                                                              thing,
                                                                                                              thing2,
                                                                                                              thing3: "asdafa asdas"
                                                                                                            },
                                                                                                            @thing: {
                                                                                                              1.000000,
                                                                                                              2.000000,
                                                                                                              3.000000
                                                                                                            }
                                                                                                            numbers: 12351.512400
                                                                                                          }
                                                                                                        
                                                                                                        
                                                                                                            @CoolTag
                                                                                                            "Test second thing yes": {
                                                                                                            }
                                                                                                        
                                                                                                           )FOO"),
  };
  
  for(U64 i = 0; i < array_cap(data); i++) {
    MetaCtx c = begin_parse(arena);
    unused(c);
    //AST *ast = parse(&c, data[i], lit("Test"));
    //pretty_print(ast->next);
    //if(ast->next->next) pretty_print(ast->next->next);
  }
}


unit_test(symbol_table_test) {
  MetaCtx ctx = begin_parse(arena);
  
  AST thing[32] = {};
  thing[0].key = lit("Thing");
  thing[1].key = lit("Not thing");
  thing[2].key = lit("Yes");
  thing[3].key = lit("No!");
  thing[4].key = lit("No!");
  put_sym(&ctx, &thing[0]);
  put_sym(&ctx, thing + 1);
  put_sym(&ctx, thing + 2);
  put_sym(&ctx, thing + 3);
  put_sym(&ctx, thing + 4);
  
  
  assert(equals(get_sym(&ctx, lit("Thing"))->key, lit("Thing")));
  assert(equals(get_sym(&ctx, lit("Not thing"))->key, lit("Not thing")));
  assert(equals(get_sym(&ctx, lit("Yes"))->key, lit("Yes")));
  assert(equals(get_sym(&ctx, lit("No!"))->key, lit("No!")));
  assert(get_sym(&ctx, lit("No!")) == &thing[4]);
}
