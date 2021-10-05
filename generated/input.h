struct MetaVar{
   S8 type;
   S8 name;
   };
MetaVar struct_V2I[] = {
  {
    lit("I32"), lit("x")}{
    lit("I32"), lit("y")}};

MetaVar struct_V3I[] = {
  {
    lit("I32"), lit("x")}{
    lit("I32"), lit("y")}{
    lit("I32"), lit("z")}};

MetaVar struct_V4I[] = {
  {
    lit("I32"), lit("x")}{
    lit("I32"), lit("y")}{
    lit("I32"), lit("z")}{
    lit("I32"), lit("w")}};

MetaVar struct_V2[] = {
  {
    lit("F32"), lit("x")}{
    lit("F32"), lit("y")}};

MetaVar struct_V3[] = {
  {
    lit("F32"), lit("x")}{
    lit("F32"), lit("y")}{
    lit("F32"), lit("z")}};

MetaVar struct_V4[] = {
  {
    lit("F32"), lit("x")}{
    lit("F32"), lit("y")}{
    lit("F32"), lit("z")}{
    lit("F32"), lit("w")}};

struct Bitmap {
  U32 *pixels;
  union {
    struct  {
      F32 x;
      F32 y;
      };
    V2 size;
    };
  V2 align;
  V2 size_world;
  };
MetaVar struct_Bitmap[] = {
  {
    lit("U32"), lit("pixels")}{
    lit("V2"), lit("size")}{
    lit("V2"), lit("align")}{
    lit("V2"), lit("size_world")}};

struct FontGlyph {
  F32 xadvance;
  U32 codepoint;
  union {
    struct  {
      U32 *pixels;
      union {
        struct  {
          F32 x;
          F32 y;
          };
        V2 size;
        };
      V2 align;
      V2 size_world;
      };
    Bitmap bitmap;
    };
  };
MetaVar struct_FontGlyph[] = {
  {
    lit("F32"), lit("xadvance")}{
    lit("U32"), lit("codepoint")}{
    lit("Bitmap"), lit("bitmap")}};

struct Font {
  F32 line_advance;
  F32 ascent;
  F32 descent;
  F32 height;
  FontGlyph *glyphs;
  I32 glyphs_len;
  };
MetaVar struct_Font[] = {
  {
    lit("F32"), lit("line_advance")}{
    lit("F32"), lit("ascent")}{
    lit("F32"), lit("descent")}{
    lit("F32"), lit("height")}{
    lit("FontGlyph"), lit("glyphs")}{
    lit("I32"), lit("glyphs_len")}};

enum EventKind {
  EventKind_None, 
  EventKind_KeyDown, 
  EventKind_KeyUp, 
  EventKind_MouseMove, 
  EventKind_MouseWheel, 
  EventKind_KeyboardText, 
  };
S8 enum_EventKind[] = {
  lit("EventKind_None"), 
  lit("EventKind_KeyDown"), 
  lit("EventKind_KeyUp"), 
  lit("EventKind_MouseMove"), 
  lit("EventKind_MouseWheel"), 
  lit("EventKind_KeyboardText"), 
  };
enum KeyID {
  KeyID_None, 
  KeyID_Up, 
  KeyID_Down, 
  KeyID_Left, 
  KeyID_Right, 
  KeyID_Escape, 
  KeyID_A, 
  KeyID_B, 
  KeyID_C, 
  KeyID_D, 
  KeyID_E, 
  KeyID_F, 
  KeyID_G, 
  KeyID_H, 
  KeyID_I, 
  KeyID_J, 
  KeyID_K, 
  KeyID_L, 
  KeyID_M, 
  KeyID_N, 
  KeyID_O, 
  KeyID_P, 
  KeyID_Q, 
  KeyID_R, 
  KeyID_S, 
  KeyID_T, 
  KeyID_U, 
  KeyID_V, 
  KeyID_W, 
  KeyID_X, 
  KeyID_Y, 
  KeyID_Z, 
  KeyID_0, 
  KeyID_1, 
  KeyID_2, 
  KeyID_3, 
  KeyID_4, 
  KeyID_5, 
  KeyID_6, 
  KeyID_7, 
  KeyID_8, 
  KeyID_9, 
  KeyID_MouseLeft, 
  KeyID_MouseRight, 
  KeyID_MouseMiddle, 
  KeyID_Count, 
  };

#define KeyID_Mapping \
Win32KeyMap(Up, VK_UP) \
Win32KeyMap(Down, VK_DOWN) \
Win32KeyMap(Left, VK_LEFT) \
Win32KeyMap(Right, VK_RIGHT) \
Win32KeyMap(Escape, VK_ESCAPE) \
Win32KeyMap(A, 65) \
Win32KeyMap(B, 66) \
Win32KeyMap(C, 67) \
Win32KeyMap(D, 68) \
Win32KeyMap(E, 69) \
Win32KeyMap(F, 70) \
Win32KeyMap(G, 71) \
Win32KeyMap(H, 72) \
Win32KeyMap(I, 73) \
Win32KeyMap(J, 74) \
Win32KeyMap(K, 75) \
Win32KeyMap(L, 76) \
Win32KeyMap(M, 77) \
Win32KeyMap(N, 78) \
Win32KeyMap(O, 79) \
Win32KeyMap(P, 80) \
Win32KeyMap(Q, 81) \
Win32KeyMap(R, 82) \
Win32KeyMap(S, 83) \
Win32KeyMap(T, 84) \
Win32KeyMap(U, 85) \
Win32KeyMap(V, 86) \
Win32KeyMap(W, 87) \
Win32KeyMap(X, 88) \
Win32KeyMap(Y, 89) \
Win32KeyMap(Z, 90) \
Win32KeyMap(0, 48) \
Win32KeyMap(1, 49) \
Win32KeyMap(2, 50) \
Win32KeyMap(3, 51) \
Win32KeyMap(4, 52) \
Win32KeyMap(5, 53) \
Win32KeyMap(6, 54) \
Win32KeyMap(7, 55) \
Win32KeyMap(8, 56) \
Win32KeyMap(9, 57) \

S8 enum_KeyID[] = {
  lit("KeyID_None"), 
  lit("KeyID_Up"), 
  lit("KeyID_Down"), 
  lit("KeyID_Left"), 
  lit("KeyID_Right"), 
  lit("KeyID_Escape"), 
  lit("KeyID_A"), 
  lit("KeyID_B"), 
  lit("KeyID_C"), 
  lit("KeyID_D"), 
  lit("KeyID_E"), 
  lit("KeyID_F"), 
  lit("KeyID_G"), 
  lit("KeyID_H"), 
  lit("KeyID_I"), 
  lit("KeyID_J"), 
  lit("KeyID_K"), 
  lit("KeyID_L"), 
  lit("KeyID_M"), 
  lit("KeyID_N"), 
  lit("KeyID_O"), 
  lit("KeyID_P"), 
  lit("KeyID_Q"), 
  lit("KeyID_R"), 
  lit("KeyID_S"), 
  lit("KeyID_T"), 
  lit("KeyID_U"), 
  lit("KeyID_V"), 
  lit("KeyID_W"), 
  lit("KeyID_X"), 
  lit("KeyID_Y"), 
  lit("KeyID_Z"), 
  lit("KeyID_0"), 
  lit("KeyID_1"), 
  lit("KeyID_2"), 
  lit("KeyID_3"), 
  lit("KeyID_4"), 
  lit("KeyID_5"), 
  lit("KeyID_6"), 
  lit("KeyID_7"), 
  lit("KeyID_8"), 
  lit("KeyID_9"), 
  lit("KeyID_MouseLeft"), 
  lit("KeyID_MouseRight"), 
  lit("KeyID_MouseMiddle"), 
  lit("KeyID_Count"), 
  };
struct Win32Bitmap {
  Bitmap bitmap;
  HDC dc;
  HBITMAP dib;
  };
MetaVar struct_Win32Bitmap[] = {
  {
    lit("Bitmap"), lit("bitmap")}{
    lit("HDC"), lit("dc")}{
    lit("HBITMAP"), lit("dib")}};

struct Win32FontCtx {
  HFONT font;
  Win32Bitmap bitmap;
  TEXTMETRIC text_metric;
  Font result;
  };
MetaVar struct_Win32FontCtx[] = {
  {
    lit("HFONT"), lit("font")}{
    lit("Win32Bitmap"), lit("bitmap")}{
    lit("TEXTMETRIC"), lit("text_metric")}{
    lit("Font"), lit("result")}};

