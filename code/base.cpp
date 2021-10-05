#pragma once

#if defined(__clang__)
#define COMPILER_CLANG 1

#if defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#elif defined(__gnulinux__) || defined(__linux__) || defined(__gnu_linux__)
#define OS_LINUX 1
#elif defined(_WIN32)
#define OS_WINDOWS 1
#else
#error Platform not supported for this compiler
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM32 1
#else
#error Architecture not supported
#endif

#elif defined(_MSC_VER)
#define COMPILER_MSVC 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#else
#error Platform not supported for this compiler
#endif

#if defined(_M_AMD64)
#define ARCH_X64 1
#elif defined(_M_IX86)
#define ARCH_X86 1
#elif defined(_M_ARM64)
#define ARCH_ARM64 1
#elif defined(_M_ARM)
#define ARCH_ARM32 1
#else
#error Architecture not supported
#endif

#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC 1

#if defined(__gnu_linux__)
#define OS_LINUX 1
#else
#error Platform not supported for this compiler
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM32 1
#else
#error Architecture not supported
#endif

#else
#error Compiler not supported
#endif

#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif

#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif

#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif

#if !defined(OS_MAC)
#define OS_MAC 0
#endif

#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif

#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif

#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif

#if !defined(ARCH_ARM32)
#define ARCH_ARM32 0
#endif

#if !defined(BUILD_DEBUG)
#define BUILD_DEBUG 0
#endif

#if !defined(BUILD_SLOW)
#define BUILD_SLOW 0
#endif

#if COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-function"
#elif COMPILER_MSVC
#pragma warning(disable : 4505)
#pragma warning(disable : 4201)
#endif

#if COMPILER_CLANG
#define crash_app()          __debugbreak()
#else
#define crash_app()          (*(volatile int *)0 = 0)
#endif

/////////////////
// @Section: Macros
#define global               static
#define local_persit         static
#define fn                   static
#define fn_export            __declspec(dllexport) extern "C"
#define KiB(v)               (1024llu*(v))
#define MiB(v)               (KiB(v)*1024llu)
#define GiB(v)               (MiB(v)*1024llu)
#define array_cap(arr)       (sizeof(arr)/sizeof(*arr))
#define lit(s)               s8((U8 *)(s), array_cap(s)-1)
#define minimum(a,b)         ((a) > (b) ? (b) : (a))
#define maximum(a,b)         ((a) > (b) ? (a) : (b))
#define CLAMP(min,val,max)   ((val) > (min) ? ((val) > (max) ? (max) : (val)) : (min))
#define CLAMP_TOP(val, max) ((val) > (max) ? (max) : (val))
#define CLAMP_BOT(min, val) ((val) < (min) ? (min) : (val))
#define swap(a, b) do{ auto temp=a; a=b; b=temp; }while(0)
#define unused(a)            (void)(a)
#define loop_n(n, i) for(I64 i = 0; i < (n); i++) 
#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)

#if DISABLE_ASSERT
#define CB(expr) 
#define assert(expr)         
#define invalid_codepath     
#define invalid_default_case 
#define not_implemented      
#define expect(expr, ...) 
#define expr_expect(expr, ...)
#define debug_panic(...)
#else
#define CB(expr) if((expr)) crash_app() // @Note(Krzosa): Conditional break
#define assert(expr)         if(!(expr)) crash_app()
#define invalid_codepath     expect(0, "Invalid codepath!")
#define invalid_default_case default: fatal_error("Invalid default case!"); break;
#define not_implemented      fatal_error("Not implemented!")
#define expect(expr, ...) if(!(expr)) fatal_error(__VA_ARGS__)
#define expr_expect(expr, ...) ((!(expr)) && fatal_error(__VA_ARGS__))
#define debug_panic(...) fatal_error(__VA_ARGS__)
#endif

#define _dllqueue_push(f,l,n,next,prev) (l) ? ((n)->prev=(l),(l)=(l)->next=(n)):((l)=(f)=(n)) 
#define _dllqueue_remove(f,l,n,next,prev) ((n)==(f)&&(n)==(l) ? (f)=(l)=0:\
(n)==(f) ? ((f)=(n)->next,(f)->prev=(n)->next=0):\
(n)==(l) ? ((l)=(n)->prev,(l)->next=(n)->prev=0):\
((n)->prev->next=(n)->next, ((n)->next->prev=(n)->prev,(n)->prev=0)))
#define dllqueue_push(f,l,n) _dllqueue_push(f,l,n,next,prev)
#define dllqueue_remove(f,l,n) _dllqueue_remove(f,l,n,next,prev)

#define _sllqueue_push(first,last,n,next) ((last) ? (last)=(last)->next=(n):(last)=(first)=(n))
#define _sllqueue_push_queue(f1,l1,f2,l2,next) ((l1) ? ((l1)->next=(f2),(l1)=(l2)):((f1)=(f2),(l1)=(l2)))
#define sllqueue_push(first,last,n) _sllqueue_push(first,last,n,next)
#define sllqueue_push_queue(f1,l1,f2,l2) _sllqueue_push_queue(f1,l1,f2,l2,next)

#define _sllstack_push(l,n,next) ((n)->next=(l), (l)=(n))
#define sllstack_push(l,n)       _sllstack_push(l,n,next) 
#define _sllstack_pop_and_store(l,n,next) (((n)=(l),(l)=(l)->next),((n)->next=0,(n)))
#define sllstack_pop_and_store(l,n) _sllstack_pop_and_store(l,n,next)

#define stack_get_top(stack) (expr_expect(stack##_len > 0, "Getting top of empty stack"), &stack[stack##_len-1])
#define stack_push(stack) (expr_expect(stack##_len < array_cap(stack), "Stack overflow"), &stack[stack##_len++])
#define stack_pop(stack) (expr_expect(stack##_len > 0, "Popping empty stack"), stack[--stack##_len])

#define _loop_list(list, node, next) for(auto node=list; node; node=node->next)
#define loop_list(list, node) _loop_list(list, node, next)
#define loop_stack(array,array_len,element,i) for(U32 i = 0; i < (array_len); i++) \
for(auto element=(array)+i;element;element=0)
#define loop_stack_cap(array,element,i) loop_stack(array,array_cap(array),element,i)
#define loop_stack_len(array,element,i) loop_stack(array,array##_len,element,i)

#define DEFAULT_RESERVE_SIZE GiB(4)
#define DEFAULT_COMMIT_SIZE  KiB(4)

////////////////////
// @Section: Base types
#include <stdint.h>
#include <stdarg.h>

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8;
typedef int64_t  I64;
typedef int32_t  I32;
typedef int16_t  I16;
typedef int8_t   I8;
typedef int64_t  B64;
typedef int32_t  B32;
typedef int16_t  B16;
typedef int8_t   B8;
typedef float    F32;
typedef double   F64;

#define I64Max INT64_MAX
#define U64Max UINT64_MAX
#define I32Max INT32_MAX
#define U32Max UINT32_MAX
#define I16Max INT16_MAX
#define U16Max UINT16_MAX

global F32 PI = 3.1415926535f;

/////////////////
// @Section: Strings
struct S8 {
  U8 *str;
  U64 len;
  U8 operator[](U64 i){return str[i];}
};

struct S16 {
  U16 *str;
  U64 len;
};

struct S8Node {
  S8 string;
  S8Node *next;
};

struct S8List {
  S8Node *first;
  S8Node *last;
  U64 node_count;
};

///////////////
// @Section: Math
union V2 {
  struct { F32 x, y; };
  struct { F32 width, height; };
  F32 v[2];
};

union V3 {
  struct{ F32 x, y, z; };
  struct{ F32 r, g, b; };
  struct{ V2 xy; F32 z_; };
  struct{ F32 x_; V2 yz; };
  F32 v[3];
};

union V4 {
  struct{ F32 x, y, z, w; };
  struct{ F32 r, g, b, a; };
  struct{ V2 xy;  V2 zw;  };
  struct{ V2 xy_; F32 width, height; };
  struct{ V3 xyz; F32 w_; };
  struct{ F32 x_; V3 yzw; };
  struct{ V3 rgb; F32 a_; };
  F32 v[4];
};

struct M4x4 {
  F32 v[4][4];
};

union V2I {
  struct{ I32 x, y; };
  I32 v[2];
};

union V3I {
  struct{ I32 x, y, z; };
  I32 v[3];
};

union V4I {
  struct{ I32 x, y, z, w; };
  I32 v[4];
};

union Rect2 {
  struct {F32 min_x, min_y, max_x, max_y;};
  struct { V2 min; V2 max; };
  F32 v[4];
};

union Rect2I {
  struct { I32 min_x, min_y, max_x, max_y;};
  struct { V2I min; V2I max; };
  I32 v[4];
};

////////////////
// @Section: Arenas
struct MArena {
  U8 *memory;
  U64 len;
  U64 len_commit;
  U64 cap;
};

//////////////////
// @Section: OS Decls
enum CreateDirStatus {
  CreateDirStatus_Success,
  CreateDirStatus_DirExists,
  CreateDirStatus_PathNotFound,
};

typedef U32 ListFilesFlag;
enum {
  ListFilesFlag_Recursive = 1 << 0,
  ListFilesFlag_IncludeDirs = 1 << 1,
};

struct Date {
  U32 year;
  U32 month;
  U32 day;
  U32 hour;
  U32 minute;
  U32 second;
  U32 milliseconds;
};

fn void*os_reserve(U64 size);
fn void os_commit(void *ptr, U64 size);
fn void os_decommit(void *ptr, U64 size);
fn void os_release(void *ptr);

fn CreateDirStatus os_make_dir(S8 path);
fn S8List os_list_files(MArena *arena, S8 path, ListFilesFlag flags=0);
fn I64  os_file_mod_time(S8 file);
fn B32  os_file_exists(S8 path);
fn B32  os_copy_file(S8 from, S8 to, B32 overwrite=false);
fn B32  os_delete_file(S8 path);
fn void os_write_file(S8 file, S8 data);
fn void os_append_file(S8 file, S8 data);
fn S8   os_read_file(MArena *arena, S8 filename);

fn S8   os_get_working_dir(MArena *arena);
fn S8   os_get_executable_dir(MArena *arena);
fn void os_set_working_dir(S8 dir);

fn void os_kill_process(S8 name);
fn I64  os_run_process(S8 console_command);
fn S8   os_get_enviroment_variable(MArena *arena, S8 var);

////////////////////////
// @Section: Filewatcher
struct FilewatcherEntry {
  S8 key;
  I64 time;
  FilewatcherEntry *next;
};

enum FileModStatus {
  FileModStatus_Inserted,
  FileModStatus_Unmodified,
  FileModStatus_Modified,
  FileModStatus_FileNotFound,
};

struct Filewatcher {
  MArena *arena;
  FilewatcherEntry entries[512];
  I32              entries_len;
};

fn S8 s8(U8 *, U64);
#if COMPILER_CLANG
global S8 vars_compiler = lit("clang");
#elif COMPILER_MSVC
global S8 vars_compiler = lit("cl");
#elif COMPILER_GCC
global S8 vars_compiler = lit("gcc");
#else
#error Unknown compiler!
#endif

#if OS_WINDOWS
global S8 vars_os_name = lit("Windows");
global S8 vars_exe_extension = lit(".exe");
global S8 vars_dll_extension = lit(".dll");
#elif OS_MAC
global S8 vars_os_name = lit("Mac OS");
global S8 vars_exe_extension = lit(".app");
global S8 vars_dll_extension = lit(".dylib");
#elif OS_LINUX
global S8 vars_os_name = lit("Linux");
global S8 vars_exe_extension = lit("");
global S8 vars_dll_extension = lit(".so");
#endif

//////////////////////////
// @Section: Logging decls
#define LOG_CALLBACK(name) void name(U64 kind, char *msg, S8 msg_s8, char *file, int line)
typedef LOG_CALLBACK(LogCallback);
fn LOG_CALLBACK(default_log_callback);
LogCallback *log_callback = default_log_callback;

#define push_log(kind, ...) push_log_msg(LogKind_##kind, __FILE__, __LINE__, __VA_ARGS__)
#define console_log(...) push_log(console, __VA_ARGS__)
#define fatal_error(...) ((push_log(error,__VA_ARGS__), crash_app()), 1)
fn void push_log_msg(U64 kind, char *file, int line, char *fmt, ...);

enum {
  LogKind_silent = 1,
  LogKind_error = 2,
  LogKind_console = 4,
};

//////////////////////////////////////
// @Section: Thread context
// Base should not have many dependencies(ideally none), and those should be provided by the 
// OS section ideally. 
struct ThreadCtx {
  I32 thread_index;
  MArena scratch_pool[4];
}; thread_local ThreadCtx thread_ctx;

struct Scratch {
  MArena *arena;
  U64 len;
  
  Scratch();
  Scratch(MArena *conflict_arena);
  Scratch(MArena *conflict_arena1, MArena *conflict_arena2);
  ~Scratch();
  operator MArena*(){ return arena; }
  private: // @Note: Disable copy constructors, cause its error prone
  Scratch(Scratch &arena);
  Scratch(Scratch &arena, Scratch &a2);
};

////////////////////
// @Section: Atomics
#if OS_WINDOWS
#include <intrin.h>
inline I64 atomic_increment(I64 volatile *v) {
  I64 result = _InterlockedIncrement64(v);
  return result;
}

inline I64 atomic_compare_and_swap(I64 volatile *destination, I64 exchange, I64 comperand) {
  I64 result = _InterlockedCompareExchange64(destination,exchange,comperand);
  return result;
}
#else
#error Atomics not supported on this OS
#endif

//////////////////////
// @Section: Memory Arena
#define zero_struct(struct) memory_zero(struct, sizeof(*struct))
#define push_size(arena, size, ...) _push_size(arena, size, __VA_ARGS__)
#define push_struct(arena, struct, ...) (struct *)_push_size(arena, sizeof(struct), __VA_ARGS__)
#define push_array(arena, struct, count, ...) (struct *)_push_size(arena, sizeof(struct) * (count), __VA_ARGS__)
#define push_struct_copy(arena, ptr) push_copy(arena, (ptr), sizeof(*(ptr)))
#define push_array_copy(arena, ptr, count) push_copy(arena, (ptr), sizeof(*(ptr))*(count))

fn void memory_zero(void *mem, U64 size) {
  U8 *m = (U8 *)mem;
  for(U64 i = 0; i < size; i++) {
    *(m + i) = 0;
  }
}

fn void memory_copy(void *from, void *to, U64 size) {
  U8 *f = (U8 *)from; 
  U8 *t = (U8 *)to;
  while(size--) *t++ = *f++;
}

fn void init_arena(MArena *arena, U64 size=DEFAULT_RESERVE_SIZE) {
  arena->memory = (U8 *)os_reserve(size);
  arena->cap = size;
}

fn void commit_arena(MArena *arena, U64 size=DEFAULT_COMMIT_SIZE) {
  U64 commit_size = size + DEFAULT_COMMIT_SIZE - 1;
  commit_size -= commit_size % DEFAULT_COMMIT_SIZE;
  os_commit((U8 *)arena->memory + arena->len_commit, commit_size);
  arena->len_commit += commit_size;
}

fn U8 *_push_size(MArena *arena, U64 size, B32 zero_memory=false) {
  if(arena->len + size > arena->len_commit) {
    if(arena->cap == 0) {
      init_arena(arena, DEFAULT_RESERVE_SIZE);
    }
    expect(arena->len + size < arena->cap, "No more space in arena");
    commit_arena(arena, size);
  }
  
  U8 *ptr = (U8 *)arena->memory + arena->len;
  arena->len += size;
  
  if(zero_memory) {
    memory_zero(ptr, size);
  }
  
  return ptr;
}

fn U8 *get_current_pos(MArena *arena) {
  if(arena->cap == 0) init_arena(arena, DEFAULT_RESERVE_SIZE);
  U8 *result = arena->memory + arena->len;
  return result;
}

fn U8 *get_pos(MArena *arena, U64 len) {
  if(arena->cap == 0) init_arena(arena, DEFAULT_RESERVE_SIZE);
  U8 *result = (U8 *)arena->memory + len;
  return result;
}

fn void pop_to_pos(MArena *arena, U64 len) {
  if(len < arena->len && len < arena->cap) {
    arena->len = len;
  }
}

fn void pop_size(MArena *arena, U64 size) {
  if(size < arena->len) {
    arena->len -= size;
  }
  else {
    arena->len = 0;
  }
}

fn void clear(MArena *arena) {
  arena->len = 0;
}

fn MArena make_subarena(MArena *base, U64 size) {
  MArena result = {};
  result.memory = push_array(base, U8, size);
  result.cap = size;
  result.len_commit = size;
  result.len = 0;
  return result;
}

fn U8 *push_copy(MArena *arena, void *copy, U64 size) {
  U8 *to = _push_size(arena, size);
  memory_copy(copy, to, size);
  return to;
}

fn S8 push_string_copy(MArena *arena, U8 *str, U64 len) {
  S8 result = s8(push_array_copy(arena, str, len), len);
  return result;
}

fn S8 push_string_copy(MArena *arena, char *str, U64 len) {
  return push_string_copy(arena, (U8 *)str, len);
}

fn S8 push_string_copy(MArena *arena, S8 str) {
  S8 result = push_string_copy(arena, (char *)str.str, str.len);
  return result;
}

Scratch::Scratch() {
  ThreadCtx *ctx = &thread_ctx;
  arena = ctx->scratch_pool;
  len   = arena->len;
}

fn MArena *get_scratch_arena(MArena **a, U32 count) {
  MArena *result = 0;
  ThreadCtx *ctx = &thread_ctx;
  for(U64 i = 0; i < array_cap(ctx->scratch_pool); i++) {
    B32 arena_valid = true;
    for(U32 j = 0; j < count; j++) {
      if(&ctx->scratch_pool[i] == a[j]) {
        arena_valid = false;
        break;
      }
    }
    if(arena_valid) {
      result = ctx->scratch_pool + i;
      break;
    }
  }
  return result;
}

Scratch::Scratch(MArena *arena1) {
  MArena *conflict[] = {arena1};
  arena = get_scratch_arena(conflict, 1);
  len = arena->len;
}

Scratch::Scratch(MArena *arena1, MArena *arena2) {
  MArena *conflict[] = {arena1, arena2};
  arena = get_scratch_arena(conflict, 2);
  len = arena->len;
}

Scratch::~Scratch() {
  pop_to_pos(arena, len);
}

//////////////////////////
// @Section: Random generator
struct RandomSeries {
  U32 index;
};

fn RandomSeries get_random_seed(U32 value) {
  RandomSeries result;
  result.index = value;
  return result;
}

fn U32 get_random_u32(RandomSeries *series) { // xorshift32
  U32 result = series->index;
  result ^= result << 13;
  result ^= result >> 17;
  result ^= result << 5;
  series->index = result;
  return result;
}

fn F32 get_random_normal(RandomSeries *series) {
  U32 rnd = get_random_u32(series);
  F32 result = (F32)rnd / (F32)U32Max;
  return result;
}

/////////////////
// @Section: Strings
fn U64 string_length(U8 *str) {
  U64 i = 0;
  while(*str++) i++;
  return i;
}

fn U64 string_length(char *str) {
  return string_length((U8 *)str);
}

fn U64 string_length(wchar_t *str) {
  U64 i = 0;
  while(*str++) i++;
  return i;
}

fn S16 s16(U16 *str, U64 len) {
  S16 result;
  result.str = str;
  result.len = len;
  return result;
}

fn S8 s8() { 
  S8 result = {};
  return result;
}

fn S8 s8(U8 *str, U64 len) {
  S8 result;
  result.str = str;
  result.len = len;
  return result;
}

fn S8 s8(char *str, U64 len) {
  return s8((U8 *)str, len);
}

fn S8 s8(U8 *str) {
  return s8(str, 0);
}

fn S8 s8(char *str) {
  return s8((U8 *)str);
}

fn S8 s8_from_cstring(U8 *str) {
  return s8(str, string_length(str));
}

fn S8 s8_from_cstring(char *str) {
  return s8((U8 *)str);
}

fn U8 to_lower_case(U8 a) {
  if(a >= 'A' && a <= 'Z') {
    a+=32;
  }
  return a;
}

fn U8 to_upper_case(U8 a) {
  if(a >= 'a' && a <= 'z') {
    a-=32;
  }
  return a;
}

fn S8 to_lower_case(MArena *arena, S8 s) {
  S8 copy = push_string_copy(arena, s);
  for(U64 i = 0; i < copy.len; i++) {
    copy.str[i] = to_lower_case(copy.str[i]);
  }
  return copy;
}

fn S8 to_upper_case(MArena *arena, S8 s) {
  S8 copy = push_string_copy(arena, s);
  for(U64 i = 0; i < copy.len; i++) {
    copy.str[i] = to_upper_case(copy.str[i]);
  }
  return copy;
}

fn B32 equals(S8 a, S8 b) {
  if(a.len != b.len) return false;
  for(U64 i = 0; i < a.len; i++) {
    if(a.str[i] != b.str[i]) return false;
  }
  return true;
}

fn B32 equals(char *a, char *b) {
  B32 result = 1;
  while(*a != 0 && *b != 0) {
    if(*a++ != *b++) {
      result = 0;
      break;
    }
  }
  if(result) {
    if(!(*a == 0 && *b == 0)) {
      result = 0;
    }
  }
  return result;
}

fn B32 equals(S16 a, S16 b) {
  if(a.len != b.len) return false;
  for(U64 i = 0; i < a.len; i++) {
    if(a.str[i] != b.str[i]) return false;
  }
  return true;
}

fn B32 match_any_in_list(S8 string, S8 *list, I64 ends_count) {
  B32 result = false;
  for(I64 i = 0; i < ends_count; i++) {
    if(equals(string, list[i])) {
      result = true;
      break;
    }
  }
  return result;
}

fn void normalize_path(S8 s) {
  for(U64 i = 0; i < s.len; i++) {
    if(s.str[i] == '\\') s.str[i] = '/';
  }
}

fn S8 get_filename(S8 s) {
  S8 result = s8();
  if(s.len > 0) {
    for(U64 i = s.len - 1; i > 0; i--) {
      if(s.str[i] == '/') {
        result = s8(s.str+i+1, s.len-i-1);
        break;
      }
    }
  }
  return result;
}

fn S8 get_last_slash(S8 s) { 
  return get_filename(s); 
}

fn S8 get_last_folder_name(S8 s) {
  S8 result = s8();
  if(s.len > 0) {
    int slash = 0;
    for(U64 i = s.len - 1; i > 0; i--) {
      if(s.str[i] == '/') {
        if(slash++) {
          result = s8(s.str+i+1, s.len-i-1);
          break;
        }
      }
    }
  }
  return result;
}

fn B32 starts_with(S8 s, S8 starts) {
  B32 result = false;
  if(s.len >= starts.len) {
    U64 i = 0;
    for(; i < starts.len; i++) {
      if(s.str[i] != starts.str[i]) {
        break;
      }
    }
    if(i == starts.len) {
      result = true;
    }
  }
  return result;
}

fn void pop_path(S8 *s) {
  expect(s->len > 0, "String is of size 0");
  if(s->len == 0) return;
  for(U64 i = s->len-1; i>0; i--) {
    if(s->str[i] == '/') {
      s->len = i;
      break;
    }
  }
}

fn S8 pop_extension(S8 s) {
  expect(s.len > 0, "String is of size 0");
  S8 result = s;
  if(s.len != 0) {
    for(U64 i = s.len-1; i>0; i--) {
      if(s.str[i] == '.') {
        result.len = i;
        break;
      }
    }
  }
  return result;
}

fn B32 ends_with(S8 string, S8 end) {
  expect(string.len > 0, "String is of size 0");
  if(end.len == 0) return false;
  I64 i = (I64)string.len-1;
  I64 end_i = (I64)end.len-1;
  B32 result = true;
  for(; i >= 0 && end_i >= 0;) {
    if(string.str[i--] != end.str[end_i--]) {
      result = false;
      break;
    }
  }
  return result;
}

fn B32 ends_with_any(S8 string, S8 *ends, I64 ends_count) {
  B32 result = false;
  for(I64 i = 0; i < ends_count; i++) {
    if(ends_with(string, ends[i])) {
      result = true;
      break;
    }
  }
  return result;
}

fn B32 is_alphabetic(U8 a) {
  if((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) {
    return true;
  }
  return false;
}

fn B32 is_number(U8 a) {
  switch(a) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0': {
      return true;
    } break;
  }
  return false;
}

fn B32 is_alphanumeric(U8 a) {
  if(is_number(a) || is_alphabetic(a)) {
    return true;
  }
  return false;
}

fn B32 contains(S8 string, S8 sub) {
  B32 result = false;
  for(U64 i = 0; i < string.len; i++) {
    U64 j = 0;
    for(; j < sub.len; j++) {
      if(string.str[i+j] != sub.str[j]) break;
    }
    if(j == sub.len) {
      result = true;
      break;
    }
  }
  return result;
}

fn void push_string(S8List *list, S8Node *node) {
  sllqueue_push(list->first,list->last,node);
  list->node_count++;
}

fn void push_list(S8List *list, S8List *plist) {
  sllqueue_push_queue(list->first, list->last, plist->first, plist->last);
  list->node_count+=plist->node_count;
}

fn S8Node *push_string(MArena *arena, S8List *list, S8 string) {
  S8Node *node = push_struct(arena, S8Node, true);
  node->next = 0;
  node->string = string;
  push_string(list, node);
  return node;
}

fn S8List split_string(MArena *arena, S8 string, S8 split_on) {
  S8List result = {};
  S8Node *top = push_string(arena, &result, s8(string.str, 0));
  for(U64 i = 0; i < string.len; ) {
    S8 check = s8(string.str+i, CLAMP(0,string.len-i,split_on.len));
    if(equals(check, split_on)) {
      i+=split_on.len;
      top = push_string(arena, &result, s8(string.str+i, 0));
    }
    else {
      top->string.len++; i++;
    }
  }
  return result;
}

global char C_bases[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
fn U64 u64_to_string(U64 value, U64 base, char *dst, U64 dst_i, U64 dst_size) {
  expect(base <= 16, "Expecting base lower then 17");
  if(value == 0) {
    if(dst_i < dst_size) dst[dst_i++] = '0';
  }
  else {
    char buff[128];
    I32 buff_len = 0;
    for(; buff_len < 128 && value != 0;) {
      U64  num = value % base;
      value /= base;
      buff[buff_len++] = C_bases[num]; 
    }
    for(I32 i = buff_len-1; i >= 0 && dst_i < dst_size;) {
      dst[dst_i++] = buff[i--];
    }
  }
  return dst_i;
}

fn U64 i64_to_string(I64 value, U64 base, char *dst, U64 dst_i, U64 dst_size) {
  if(value < 0) {
    dst[dst_i++] = '-';
    value = -value;
  }
  dst_i = u64_to_string((U64)value, base, dst, dst_i, dst_size);
  return dst_i;
}

#include <stdlib.h>
fn F64 s8_to_f64(S8 value) {
  char buff[128];
  memory_copy(value.str, buff, value.len);
  buff[value.len] = 0;
  F64 result = strtod(buff, 0); // @Todo: Clib out
  return result;
}

#include <stdio.h>
fn S8 string_format_args(char *dst, U64 dst_size, char *fmt, va_list args) {
#define string_format_s8(str, len) \
assert(len < 5000 && "A bit to much to format! Might be an error"); \
assert(dst_i < dst_size && "iterator is beyond the boundary!"); \
len = minimum(len, dst_size - dst_i); \
for(U64 str_i = 0; str_i < len;) dst[dst_i++] = (char)str[str_i++]; 
  
  U64 dst_i = 0;
  U64 fmt_i = 0;
  while(fmt[fmt_i] && dst_i < dst_size) {
    switch(fmt[fmt_i]) {
      case '%': {
        switch(fmt[++fmt_i]) {
          case '%': goto write; break;
          case '.': {
            if(equals(s8(fmt+fmt_i, 3), lit(".*s"))) {
              fmt_i+=3;
              U64 len = (U64)va_arg(args, int);
              U8 *str = va_arg(args, U8 *);
              string_format_s8(str, len)
            }
            else {
              invalid_codepath;
            }
          } break;
          case 'I': {
            fmt_i++;
            I64 value = va_arg(args, I64);
            dst_i = i64_to_string(value, 10, dst, dst_i, dst_size);
          } break;
          case 'f': {
            // @Todo: get rid of CLib
            fmt_i++;
            F64 value = va_arg(args, F64);
            assert(dst_i < dst_size);
            U64 space = dst_size-dst_i;
            snprintf(dst+dst_i, space, "%f", value); 
            dst_i += string_length(dst+dst_i); // result of snprintf is useless I guess
          } break;
          case 'd': {
            fmt_i++;
            I64 value = va_arg(args, int);
            dst_i = i64_to_string(value, 10, dst, dst_i, dst_size);
          } break;
          case 'u': {
            fmt_i++;
            U64 value = va_arg(args, unsigned);
            dst_i = u64_to_string(value, 10, dst, dst_i, dst_size);
          } break;
          case 'c': {
            fmt_i++;
            int value = va_arg(args, int);
            dst[dst_i++] = (char)value;
          } break;
          case 'b': {
            fmt_i++;
            int value = va_arg(args, int);
            char *bool_str;
            if(value) bool_str = "true";
            else bool_str = "false";
            for(;dst_i < dst_size && *bool_str;) dst[dst_i++] = *bool_str++;
          } break;
          case 'C': {
            fmt_i++;
            char *value = (char *)va_arg(args, char *);
            if(value == 0) value = "null";
            for(;dst_i < dst_size && *value;) dst[dst_i++] = *value++;
          } break;
          case 's': {
            fmt_i++;
            S8 str = va_arg(args, S8);
            expect(str.len < 100000, "string_format var arg received a S8 of size over 100000, likely an error!");
            string_format_s8(str.str, str.len)
          } break;
          default: invalid_codepath;
        }
      } break;
      default: {
        write:
        dst[dst_i++] = fmt[fmt_i++];
      }
    }
  }
#undef string_format_s8
  S8 result = s8(dst, dst_i);
  return result;
}

fn U64 string_format(char *dst, U64 dst_size, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  S8 result = string_format_args(dst, dst_size, fmt, args);
  va_end(args);
  return result.len;
}

#define push_string_vargs(arena, str, result) \
va_list args; \
va_start(args, str); \
S8 result = string_format_args((char *)push_size((arena), 4096), 4096, str, args); \
va_end(args); \
pop_size(arena, 4096-result.len); 

fn S8 push_string(MArena *arena, char *str, ...) {
  push_string_vargs(arena, str, result);
  return result;
}

fn void push_log_msg(U64 kind, char *file, int line, char *fmt, ...) {
  char buff[1024];
  va_list args;
  va_start(args, fmt);
  S8 result = string_format_args(buff, array_cap(buff)-1, fmt, args);
  result.str[result.len] = 0;
  va_end(args);
  
  if(log_callback) {
    log_callback(kind, (char *)result.str, result, file, line);
  }
}

#define end_array(arena, ptr, type) _end_array(arena, (U8 *)ptr, sizeof(type))
#define begin_array(arena, type) (type *)_begin_array(arena)
fn U8 *_begin_array(MArena *arena) {
  U8 *result = get_current_pos(arena);
  return result;
}

fn U64 _end_array(MArena *arena, U8 *ptr, U64 element_size) {
  assert(ptr >= arena->memory);
  U64 len = get_current_pos(arena) - ptr;
  assert(len % element_size == 0);
  U64 elements = len / element_size;
  return elements;
}

fn U8 *begin_string(MArena *arena) {
  return begin_array(arena, U8);
}

fn S8 end_string(MArena *arena, U8 *ptr) {
  U64 len = end_array(arena, ptr, U8);
  return s8(ptr, len);
}

///////////////////////////
// @Section: UTF conversions
fn U32 utf32_to_utf8(U32 codepoint, U8 out_str[4]) {
  U32 advance = 0;
  if (codepoint <= 0x7F) {
    advance = 1;
    out_str[0] = (U8)codepoint;
  } else if (codepoint <= 0x7FF) {
    advance = 2;
    out_str[0] = 0b11000000 | (0b00011111 & (codepoint >> 6));
    out_str[1] = 0b10000000 | (0b00111111 & codepoint);
  } else if (codepoint <= 0xFFFF) { // 16 bit word
    advance = 3;
    out_str[0] = 0b11100000 | (0b00001111 & (codepoint >> 12)); // 4 bits
    out_str[1] = 0b10000000 | (0b00111111 & (codepoint >> 6));  // 6 bits
    out_str[2] = 0b10000000 | (0b00111111 & codepoint);         // 6 bits
  } else if (codepoint <= 0x10FFFF) { // 21 bit word
    advance = 4;
    out_str[0] = 0b11110000 | (0b00000111 & (codepoint >> 18)); // 3 bits
    out_str[1] = 0b10000000 | (0b00111111 & (codepoint >> 12)); // 6 bits
    out_str[2] = 0b10000000 | (0b00111111 & (codepoint >> 6));  // 6 bits
    out_str[3] = 0b10000000 | (0b00111111 & codepoint);         // 6 bits
  } 
  
  return advance;
}

fn U32 utf32_to_utf16(U32 codepoint, U16 out_str[2]) {
  U32 advance = 0;
  
  if (codepoint < 0x10000) {
    out_str[0] = (U16)codepoint;
    out_str[1] = 0;
    advance = 1;
  }
  else if (codepoint <= 0x10FFFF) {
    U32 code = (codepoint - 0x10000);
    out_str[0] = (U16)(0xD800 | (code >> 10));
    out_str[1] = (U16)(0xDC00 | (code & 0x3FF));
    advance = 2;
  }
  
  return advance;
}

fn U32 utf16_to_utf32(U16 *c, U32 *out_str) {
  U32 advance = 1;
  *out_str = c[0];
  if (c[0] >= 0xD800 && c[0] <= 0xDBFF && c[1] >= 0xDC00 && c[1] <= 0xDFFF) {
    *out_str = 0x10000;
    *out_str += (U32)(c[0] & 0x03FF) << 10u | (c[1] & 0x03FF);
    advance = 2;
  }
  return advance;
}

fn U32 utf8_to_utf32(U8 *c, U32 *out_str) {
  U32 advance = 0;
  *out_str = 0;
  if ((c[0] & 0b10000000) == 0) {
    *out_str = c[0];
    advance = 1;
  }
  else if ((c[0] & 0b11100000) == 0b11000000) {
    if ((c[1] & 0b11000000) == 0b10000000) {
      *out_str = (U32)(c[0] & 0b00011111) << 6u | 
      (c[1] & 0b00111111);
      advance = 2;
    }
    
  }
  else if ((c[0] & 0b11110000) == 0b11100000) {
    if ((c[1] & 0b11000000) == 0b10000000 &&
        (c[2] & 0b11000000) == 0b10000000) {
      *out_str = (U32)(c[0] & 0b00001111) << 12u |
      (U32)(c[1] & 0b00111111) << 6u | 
      (c[2] & 0b00111111);
      advance = 3;
    }
  }
  else if ((c[0] & 0b11111000) == 0b11110000) {
    if ((c[1] & 0b11000000) == 0b10000000 &&
        (c[2] & 0b11000000) == 0b10000000 &&
        (c[3] & 0b11000000) == 0b10000000) {
      *out_str = (U32)(c[0] & 0b00001111) << 18u |
      (U32)(c[1] & 0b00111111) << 12u |
      (U32)(c[2] & 0b00111111) << 6u | 
      (U32)(c[3] & 0b00111111);
      advance = 4;
    }
  }
  
  return advance;
}

fn S8 utf16_to_utf8(MArena *arena, U16 *string, I64 string_len) {
  U8 *s = begin_string(arena);
  for(I64 i = 0; i < string_len;) {
    U32 u32;
    U32 advance = utf16_to_utf32(string+i, &u32);
    i += advance;
    if(advance == 0) {
      expect(advance != 0, "Assert: error converting from utf16 to utf8");
      break;
    }
    else {
      U8 u8[4];
      U32 advance8 = utf32_to_utf8(u32, u8);
      push_string_copy(arena, u8, advance8);
    }
  }
  S8 result = end_string(arena, s);
  return result;
}

fn S8 s8_from_widestring(MArena *arena, wchar_t *string) {
  return utf16_to_utf8(arena, (U16 *)string, string_length(string));
}

fn S8 s8_from_widestring(MArena *arena, wchar_t *string, I64 string_len) {
  return utf16_to_utf8(arena, (U16 *)string, string_len);
}

fn wchar_t *widestring_from_s8(MArena *arena, S8 string) {
  U16 *string16 = begin_array(arena, U16);
  for(U64 i = 0; i < string.len;) {
    U32 u32;
    U32 advance = utf8_to_utf32(string.str+i, &u32); 
    i+=advance;
    if(advance == 0) {
      expect(advance != 0, "Assert: error converting from utf8 to utf16");
      break;
    }
    else {
      U16 u16[2];
      U32 advance16 = utf32_to_utf16(u32, u16);
      push_array_copy(arena, u16, advance16);
    }
  }
  end_array(arena, string16, U16);
  push_struct(arena, U16, true); // Null terminate
  return (wchar_t *)string16;
}

fn char *cstring_from_s8(MArena *arena, S8 string) {
  S8 result = push_string_copy(arena, string);
  push_struct(arena, U8, true);
  return (char *)result.str;
}

///////////////////////////////////////
// @Section: Hash 
fn U64 fnv_hash(S8 string) {
  U64 hash = (U64)14695981039346656037ULL;
  loop_stack(string.str, string.len, str, i) {
    hash = hash ^ (U64)(*str);
    hash = hash * (U64)1099511628211ULL;
  }
  return hash;
}

fn U64 fnv_hash(char *string) {
  return fnv_hash(s8(string, string_length(string)));
}

////////////////////
// @Section: OS
#if OS_WINDOWS 
#define NOMINMAX
#include <Windows.h>
global I64 global_counts_per_second;

fn void w32_print_error(DWORD error_code) {
  char buf[256];
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf,(sizeof(buf) / sizeof(wchar_t)), NULL);
  push_log(console, "Error: %C", buf);
}

fn void w32_print_error() {
  w32_print_error(GetLastError());
}

fn F64 os_time() {
  if(global_counts_per_second == 0) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    global_counts_per_second = freq.QuadPart;
  }
  
  LARGE_INTEGER time;
  QueryPerformanceCounter(&time);
  F64 result = (F64)time.QuadPart / (F64)global_counts_per_second;
  return result;
}

fn B32 os_file_exists(S8 file) {
  Scratch scratch;
  DWORD dwAttrib = GetFileAttributesW(widestring_from_s8(scratch, file));
  return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

fn CreateDirStatus os_make_dir(S8 path) {
  Scratch scratch;
  BOOL result = CreateDirectoryW(widestring_from_s8(scratch, path), NULL);
  if(result == 0) {
    DWORD error = GetLastError();
    if(error == ERROR_ALREADY_EXISTS) {
      push_log(silent, "Directory %s already exists", path);
      return CreateDirStatus_DirExists;
    }
    else if(error == ERROR_PATH_NOT_FOUND) {
      push_log(console, "Path %s not found when creating a directory", path);
      return CreateDirStatus_PathNotFound;
    }
  }
  return CreateDirStatus_Success;
}

fn S8List os_list_files(MArena *arena, S8 path, ListFilesFlag flags) {
  S8List list = {};
  S8 path_all = push_string(arena, "%s/*", path);
  
  WIN32_FIND_DATAW data;
  HANDLE handle = FindFirstFileW(widestring_from_s8(arena, path_all), &data);
  if (handle != INVALID_HANDLE_VALUE) {
    while (FindNextFileW(handle, &data) != 0) {
      U64 string_len = string_length(data.cFileName);
      if (!(string_len == 2 && data.cFileName[0] == L'.' && data.cFileName[1] == L'.')) { // skip empty files and ".."
        if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
          if(flags & ListFilesFlag_IncludeDirs) {
            push_string(arena, &list, push_string(arena, "%s/%s/", path, s8_from_widestring(arena, data.cFileName)));
          }
          if(flags & ListFilesFlag_Recursive) {
            S8 dir_path = s8_from_widestring(arena, data.cFileName); 
            S8List l = os_list_files(arena, dir_path, flags); 
            push_list(&list, &l);
          }
        } 
        else {
          push_string(arena, &list, push_string(arena, "%s/%s", path, s8_from_widestring(arena, data.cFileName)));
        }
      }
    }
    
    DWORD message = GetLastError();
    if (message != ERROR_NO_MORE_FILES) {
      w32_print_error();
      push_log(error, "Error listing files in: %s", path);
    }
    FindClose(handle);
    
  }
  else {
    w32_print_error();
    push_log(error, "Directory not found: %s", path);
  }
  
  return list;
}

fn B32 os_copy_file(S8 from, S8 to, B32 overwrite) {
  Scratch scratch;
  wchar_t *fromt = widestring_from_s8(scratch,from);
  wchar_t *tot = widestring_from_s8(scratch,to);
  if(!CopyFileW(fromt, tot, !overwrite)) {
    w32_print_error();
    push_log(console, "Failed to copy %s to %s", from, to);
    return false;
  }
  return true;
}

fn I64 os_file_mod_time(S8 file) {
  FILETIME time = {};
  WIN32_FIND_DATAW data;
  Scratch scratch;
  HANDLE handle = FindFirstFileW(widestring_from_s8(scratch, file), &data);
  if (handle != INVALID_HANDLE_VALUE) {
    FindClose(handle);
    time = data.ftLastWriteTime;
  } 
  else {
    // push_log(console, "File not found! %s", file);
    return -1;
  }
  I64 result = (I64)time.dwHighDateTime << 32 | time.dwLowDateTime;
  return result;
}

fn Date os_date() {
  SYSTEMTIME local;
  GetLocalTime(&local);
  
  Date result;
  result.year = local.wYear;
  result.month = local.wMonth;
  result.day = local.wDay;
  result.hour = local.wHour;
  result.second = local.wSecond;
  result.milliseconds = local.wMilliseconds;
  return result;
}

fn B32 os_delete_file(S8 path) {
  Scratch scratch;
  wchar_t *patht = widestring_from_s8(scratch, path);
  B32 result = DeleteFileW(patht);
  return result;
}

fn S8 os_read_file(MArena *arena, S8 file) {
  S8 result = {};
  Scratch scratch(arena);
  HANDLE handle = CreateFileW(widestring_from_s8(scratch, file), 
                              GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  B32 failed = false;
  if (handle != INVALID_HANDLE_VALUE) {
    LARGE_INTEGER get_file_size;
    if (GetFileSizeEx(handle, &get_file_size)) {
      DWORD read;
      result.len = (U64)get_file_size.QuadPart;
      result.str = (U8 *)push_size(arena, result.len + 1);
      // @Todo: can only read 32 byte size files?
      if (ReadFile(handle, result.str, (U32)result.len, &read, NULL)) {
        if (read != result.len) {
          push_log(error, "Bytes read and bytes expected to read, mismatch when reading %s", file);
          failed = true;
        }
      } 
      else {
        push_log(error, "Failed to read file %s", file);
        failed = true;
      }
    } 
    else {
      push_log(error, "Failed to retrieve file size of %s", file);
      failed = true;
    }
    
    CloseHandle(handle);
  } 
  else {
    push_log(console, "File not found %s\n", file);
    failed = true;
  }
  
  if(failed) {
    w32_print_error();
    pop_size(arena, result.len+1);
    result.str = 0;
    result.len = 0;
  }
  
  return result;
}

fn void w32_write_file(S8 file, S8 data, B32 append=false) {
  DWORD access = GENERIC_WRITE;
  DWORD creation_disposition = CREATE_ALWAYS;
  if(append) {
    access = FILE_APPEND_DATA;
    creation_disposition = OPEN_ALWAYS;
  }
  
  Scratch scratch;
  HANDLE handle = CreateFileW(widestring_from_s8(scratch, file), access, 0, NULL,
                              creation_disposition, FILE_ATTRIBUTE_NORMAL, NULL);
  if(handle != INVALID_HANDLE_VALUE) {
    DWORD bytes_written = 0;
    // @Todo: can only read 32 byte size files?
    B32 error = WriteFile(handle, data.str, (U32)data.len, &bytes_written, NULL);
    if(error == false) {
      w32_print_error();
      push_log(error, "Failed to write to file: %s", file);
    }
    else {
      if(bytes_written != data.len) {
        w32_print_error();
        push_log(error, "Failed to write to file: %s, mismatch between length requested to write and length written", file);
      }
    }
    CloseHandle(handle);
  }
  else {
    w32_print_error();
    push_log(error, "File not found when trying to write: %s", file);
  }
}

fn void os_write_file(S8 file, S8 data) {
  w32_write_file(file, data, false);
}

fn void os_append_file(S8 file, S8 data) {
  w32_write_file(file, data, true);
}

fn S8 os_get_working_dir(MArena *arena) {
  wchar_t buff[1024];
  U64 len = GetCurrentDirectoryW(1024, buff);
  expect(len != 0 && len < array_cap(buff),"Failed to get current working directory");
  
  S8 string = utf16_to_utf8(arena, (U16 *)buff, len);
  normalize_path(string);
  return string;
}

fn S8 os_get_executable_dir(MArena *arena) {
  wchar_t buff[1024];
  U64 len = GetModuleFileNameW(NULL, buff, array_cap(buff));
  expect(len != 0 && len < array_cap(buff),"Failed to retrieve executable directory");
  
  S8 string = utf16_to_utf8(arena, (U16 *)buff, len);
  normalize_path(string);
  pop_path(&string);
  return string;
}

fn void os_set_working_dir(S8 dir) {
  Scratch scratch;
  wchar_t *chars = widestring_from_s8(scratch, dir);
  expect(SetCurrentDirectoryW(chars) != 0, "Failed to set current working dir");
}

fn S8 os_get_enviroment_variable(MArena *arena, S8 var) {
  Scratch scratch(arena);
  U64 buff_size = 8192;
  char *buff = (char *)push_size(scratch, buff_size);
  
  U64 len = GetEnvironmentVariable(cstring_from_s8(scratch, var), buff, (DWORD)buff_size);
  expect(len != 0 && len < buff_size, "Failed to get enviroment variable");
  S8 result = push_string_copy(arena, buff, len);
  normalize_path(result);
  return result;
}

fn void os_kill_process(S8 name) {
  char buff[1024];
  U64 len = string_format(buff,array_cap(buff),"taskkill /f /im %s", name);
  buff[len] = 0;
  system(buff);
}

fn I64 os_run_process(S8 console_command) {
  Scratch scratch;
  wchar_t *_working_dir = NULL; // Set working dir to caller's dir
  wchar_t *console_args = widestring_from_s8(scratch, console_command);
  S8 exe_name = s8(console_command.str, 0);
  for(; exe_name.len < console_command.len; exe_name.len++) {
    if(console_command.str[exe_name.len] == ' ') {
      break;
    }
  }
  STARTUPINFOW startup_info = {};
  // if(!settings.dont_pipe_stdout_to_console) startup_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  // startup_info.hStdOutput = handle;
  // startup_info.dwFlags |= STARTF_USESTDHANDLES;
  startup_info.cb = sizeof(STARTUPINFOW);
  PROCESS_INFORMATION process_info = {};
  
  S8 env_path = os_get_enviroment_variable(scratch, lit("PATH"));
  S8List envs = split_string(scratch, env_path, lit(";"));
  push_string(scratch, &envs, lit("."));
  
  push_log(silent, "Running process: %s", exe_name);
  push_log(silent, "With arguments: %s", console_command);
  I64 result = 1;
  // push_log(console, "--- %s: --- \n", exe_name);
  fflush(stdout);
  for (S8Node *node = envs.first; node; node = node->next) {
    S8 app = push_string(scratch, "%s/%s", node->string, exe_name);
    push_log(silent, "%s\n", app);
    wchar_t *application = widestring_from_s8(scratch, app);
    if (CreateProcessW(application, console_args, NULL, NULL, FALSE,
                       NORMAL_PRIORITY_CLASS, NULL, _working_dir, &startup_info,
                       &process_info)) {
      WaitForSingleObject(process_info.hProcess, INFINITE);
      DWORD exit_code;
      if(GetExitCodeProcess(process_info.hProcess, &exit_code) == 0){
        invalid_codepath;
      }
      CloseHandle(process_info.hProcess);
      CloseHandle(process_info.hThread);
      result = exit_code;
      break;
    }
  }
  if (result != 0) {
    w32_print_error();
    push_log(console, "Failed to launch process! %s\n", console_command);
  } 
  return result;
}

fn void* os_reserve(U64 size) {
  void *result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
  if(result == 0) {
    // DWORD error = GetLastError();
    MessageBoxA(NULL, "Failed to allocate memory!", "Error!", MB_OK);
  }
  return result;
}

fn void os_commit(void *ptr, U64 size) {
  void *result = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
  if(result == 0) {
    // DWORD error = GetLastError();
    MessageBoxA(NULL, "Failed to allocate memory!", "Error!", MB_OK);
  }
}

fn void os_decommit(void *ptr, U64 size) {
  VirtualFree(ptr, size, MEM_DECOMMIT);
}

fn void os_release(void *ptr, U64 size) {
  unused(size);
  VirtualFree(ptr, 0, MEM_RELEASE);
}

/////////////////////////////
// @Section: Default base config
#pragma comment(lib, "user32.lib")
fn LOG_CALLBACK(default_log_callback) {
  unused(msg_s8);
  if (kind == LogKind_error) {
    char buff[1024];
    U64 len = string_format(buff, array_cap(buff)-1, "%C - %C:%d", msg, file, line);
    buff[len] = 0;
    MessageBoxA(NULL, buff, "Error!", MB_OK);
  }
  else if (kind == LogKind_console) {
#if defined(PLATFORM) || defined(PLATFORM_DLL)
    OutputDebugStringA(msg);
#else
    printf("%s", msg);
    fflush(stdout);
#endif
  }
}
#endif

/////////////////////////
// @Section: Build system
#ifdef BUILD_SYSTEM
struct Build;
int build_main(Build *b);

enum BuildKind {
  Build_Debug,
  Build_Release,
};

struct Build {
  BuildKind kind;
  S8 compiler;
  S8 executable_name;
  S8 source_code_directory;
  S8 optimizations;
  
  B32 dll;
  B32 generate_debug_symbols;
  B32 kill_executable;
  B32 enable_avx;
  B32 run_on_success;
  
  S8List defines;
  S8List files; // to compile
  S8List include_dirs;
  S8List libraries;
  
  MArena *arena;
  S8List console_args;
  
  // @Note: Default project book keeping
  B32 default_project_initialized;
  S8 original_user_dir; // Cached cwd for default project setup
  S8 executable_dir;
};

fn S8List parse_console_args(MArena *arena, int arguments_len, char **arguments) {
  S8List result = {};
  for(int i = 0; i < arguments_len; i++) {
    S8Node *node = push_struct(arena, S8Node, true);
    node->string = s8(arguments[i], string_length(arguments[i]));
    push_string(&result, node);
  }
  return result;
}

fn B32 match_console_arg(Build *b, S8 arg) {
  B32 result = false;
  loop_list(b->console_args.first, a) {
    if(equals(arg, a->string)) {
      result = true;
    }
  }
  return result;
}

int main(int arguments_len, char **arguments) {
  Build b = {};
  MArena arena = {};
  b.arena = &arena;
  b.console_args = parse_console_args(b.arena, arguments_len, arguments);
  
  int result = build_main(&b);
  if(b.default_project_initialized)
    os_set_working_dir(b.original_user_dir);
  return result;
}

fn void add_define(Build *args, S8 key, S8 value){
  push_string(args->arena, &args->defines, key);
  push_string(args->arena, &args->defines, value);
}

fn void add_library(Build *args, S8 name) {
  push_string(args->arena, &args->libraries, name);
}

fn void add_file(Build *args, S8 name){
  push_string(args->arena, &args->files, name);
}

fn void add_include_directory(Build *args,S8 name){
  push_string(args->arena, &args->include_dirs, name);
}

fn S8 end_compiler_args(MArena *scratch, Build *args) {
  U8 *s = begin_string(scratch);
  push_string(scratch, "%s%s ", args->compiler, vars_exe_extension);
  if (equals(args->compiler, lit("clang"))) {
    push_string(scratch, "-Wall -Wextra -g -fdiagnostics-absolute-paths ");
    if(args->optimizations.len) push_string(scratch, "-%s ", args->optimizations);
    if(args->enable_avx) push_string(scratch, "-mavx ");
    if(args->dll) push_string(scratch, "-shared -o %s%s ", args->executable_name, vars_dll_extension);
    else push_string(scratch, "-o %s%s ", args->executable_name, vars_exe_extension);
    loop_list(args->libraries.first, node) {
      push_string(scratch, "-l%s ", node->string);
    }
  }
  else if (equals(args->compiler, lit("cl"))) {
    push_string(scratch, "-nologo -Zi -W4 -Fe:%s%s", args->executable_name, vars_exe_extension);
    if(args->optimizations.len) push_string(scratch, "-%s ", args->optimizations);
    if(args->dll) push_string(scratch, "-LD ");
    loop_list(args->libraries.first, node) push_string(scratch, "%s.lib ", node->string);
  }
  else {
    fatal_error("Unsupported compiler!");
  }
  
  loop_list(args->defines.first, node) {
    push_string(scratch, "-D%s=%s ", node->string, node->next->string);
    node = node->next;
  }
  loop_list(args->include_dirs.first, node) {
    push_string(scratch, "-I%s ", node->string);
  }
  loop_list(args->files.first, node) {
    push_string(scratch, "%s/%s ", args->source_code_directory, node->string);
  }
  
  S8 result = end_string(scratch, s);
  
  if(args->kill_executable) os_kill_process(args->executable_name);
  return result;
}

fn B32 compile(Build *args) {
  Scratch scratch;
  S8 arguments = end_compiler_args(scratch, args);
  B32 result = os_run_process(arguments);
  return result;
}

fn void clean_excess_files(S8 executable_dir) {
  push_log(silent, "--- Deleting excess files ---\n");
  
  Scratch scratch;
  os_set_working_dir(executable_dir);
  S8 match_list[] = {lit(".ilk"), lit(".obj")};
  S8List files = os_list_files(scratch, lit("."), true);
  for(S8Node *node = files.first; node; node=node->next){ 
    if(ends_with_any(node->string, match_list, array_cap(match_list))) {
      push_log(silent, "%s", node->string);
      os_delete_file(node->string);
    }
  }
}

fn void setup_default_project(Build *b, BuildKind kind=Build_Debug) {
  b->original_user_dir = os_get_working_dir(b->arena);
  b->executable_dir = os_get_executable_dir(b->arena);
  b->source_code_directory = lit("../code");
  b->compiler = vars_compiler;
  b->generate_debug_symbols = 1;
  b->kind = kind;
  
  os_set_working_dir(b->executable_dir);
  os_make_dir(lit("build"));
  os_make_dir(lit("code"));
  os_make_dir(lit("assets"));
  os_make_dir(lit("generated"));
  add_include_directory(b, lit("../generated"));
  
  if(kind == Build_Debug) {
    add_define(b, lit("BUILD_DEBUG"), lit("1"));
    add_define(b, lit("BUILD_SLOW"), lit("1"));
  }
  else if(kind == Build_Release) {
    b->optimizations = lit("O2");
  }
  b->default_project_initialized = true;
}

fn void clear_files(Build *c) {
  c->files.first = c->files.last = 0;
  c->files.node_count = 0;
}

fn void unity_build(Build *b, S8 *files, U32 file_len=1) {
  Scratch scratch(b->arena);
  os_set_working_dir(b->executable_dir);
  os_set_working_dir(lit("build"));
  for(U32 i = 0; i < file_len; i++) {
    console_log("--- %s ---\n", files[i]);
    clear_files(b);
    add_file(b, push_string(scratch, "%s.cpp", files[i]));
    b->executable_name = files[i];
    if(compile(b) == 0) {
      if(b->run_on_success) {
        os_run_process(push_string(scratch, "%s%s", files[i], vars_exe_extension));
      }
    }
    else {
      console_log("\n--- Failed compilation of %s ---\n", files[i]);
    }
  }
  console_log("\n--- Done compiling! ---\n");
  clean_excess_files(b->executable_dir);
  os_set_working_dir(b->original_user_dir);
}

fn void unity_build(Build *b, S8 file) {
  S8 files[] = {file};
  unity_build(b, files);
}

///////////////////
// @Section: Unit tests
#define UNIT_TEST(name) void name(MArena *arena)
typedef UNIT_TEST(FuncUnitTest);
struct UnitTest {
  char *name;
  FuncUnitTest *func;
};
global UnitTest global_unit_tests[1024];
global I64      global_unit_tests_len;
struct UnitTester {
  UnitTester(char *name, FuncUnitTest *func) {
    UnitTest *test = global_unit_tests + global_unit_tests_len++;
    test->name = name;
    test->func = func;
  }
};
#define unit_test(name) fn UNIT_TEST(name); global UnitTester tester##name(#name, name); \
fn UNIT_TEST(name)

fn void do_tests() {
  console_log("--- Tests ---\n");
  for(I64 i = 0; i < global_unit_tests_len; i++) {
    Scratch scratch;
    UnitTest *test = global_unit_tests + i;
    console_log("%C ... ", test->name);
    test->func(scratch);
    console_log(" Done\n");
  }
}
#endif
