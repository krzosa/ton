#define BUILD_SYSTEM
#include "code/base.cpp"
#include "code/lang_tests.cpp"
B32 run_tests = true;

int build_main(Build *b) {
  if(run_tests || match_console_arg(b, lit("run_tests"))) {
    do_tests();
  }
  
  setup_default_project(b);
  b->run_on_success = true;
  unity_build(b, lit("main"));
  
  return 0;
}
