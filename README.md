# Nexis

The Nexis project is made out of the nexis core library and the nexis editor.
Note this project is in development right now so it may not compile!

# Building

To build the editor and the library:

```bash
cmake -B build
cmake --build build
```

You can disable building the editor with a build options:
```bash
cmake -B build -DNEXIS_BUILD_EDITOR=OFF
```

The editor executable will be in build/editor/ called Nexis.
The library will be in build/core/ called nexis_core.

To build the tests of some of the libraries features you need to have the clang
compiler available on your system. Then you can configure the cmake build to include tests

```bash
cmake -B build -DCMAKE_C_COMPILER=clang -DNEXIS_ENABLE_FUZZING=ON -DNEXIS_BUILD_EDITOR=OFF
```

To build the examples in the root examples/ folder you can do
```bash
cmake -B build -DNEXIS_BUILD_EXAMPLES=ON
```

# Dependencies
Core:
  - libc
  - libm

Editor:
  - [rlImGui](https://github.com/raylib-extras/rlImGui.git)
  - [ImGui](https://github.com/ocornut/imgui.git)
  - [Raylib](https://github.com/raysan5/raylib.git)
  - [NFD](https://github.com/btzy/nativefiledialog-extended)
# Nexis
