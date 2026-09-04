# Nexis

The Nexis project is made out of the nexis core library and the nexis editor.

# Building

To build the editor and the library:

```bash
cmake -B build
cmake --build build
```

You can disable building the editor with a build options:
```bash
cmake -B build -DNEXIS_BUILD_EDITOR=Off
```

The editor executable will be in build/editor/ called Nexis.
The library will be in build/core/ called nexis_core.

# Dependencies
  - [rlImGui](https://github.com/raylib-extras/rlImGui.git)
  - [ImGui](https://github.com/ocornut/imgui.git)
  - [Raylib](https://github.com/raysan5/raylib.git)
  - [NFD](https://github.com/btzy/nativefiledialog-extended)
