Small data description language similar to JSON made for fun.

Sample from input.data file: 
``` c
@data cool_data: {
  "Cool data string": 123.51525,
  "Object": {
    Nice: 123,
    Nicee: "12242",
  },
  List: {1, 2, 3, 4}
}

@struct Bitmap: {
  pixels: @ptr   U32,
  size  : @embed V2,
  align :        V2,
  size_world :   V2,
}

@enum EventKind:{
  None,
  KeyDown,
  KeyUp,
  MouseMove,
  MouseWheel,
  KeyboardText,
}
```

Example of generated code using the AST can be found in the generated/ folder.

## To compile you need:
* Windows
* Visual Studio installed
* Clang / LLVM installed

## Build command: 
```clang build.cpp && a.exe```
