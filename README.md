# Stardust XR Reference Server

This project is a usable Linux display server that reinvents human-computer interaction for all kinds of XR, from putting 2D/XR apps into various 3D shells for varying uses to SDF-based interaction.

## Prerequisites
1. Meson >= 0.55.0
2. Ninja
3. CMake
4. Flatbuffers >= 1.12.0
5. EGL+GLES 3.2
6. GLX+Xlib
7. fontconfig
8. dlopen
9. OpenXR Loader (required even if run in flatscreen mode)

## Build
```bash
meson build --prefix=/usr
cd build
ninja
```

## Install
```bash
ninja install
```