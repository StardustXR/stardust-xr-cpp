# Stardust XR Core

This project is a Linux-based Wayland compositor that allows traditionally 2D applications to be interactive in 3D for use in immersive devices, such as HMDs. As well as simple display and interaction, Stardust will allow for "app shells" that can transform 2D app interactions into 3D making said applications much easier to use in XR.

This project is made using Qt 5.13.1 with and Qt 3D Quick.

## Building instructions:

1. Install Qt Quick 3D (https://git.qt.io/annichol/qtquick3d/)
2. Build using the provided scripts (make sure qmake is in your PATH):
```
$ mkdir build && cd build
$ sh ../scripts/build.sh ..
```
