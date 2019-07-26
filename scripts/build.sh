#!/bin/sh

rm *

qmake $1/stardust-xr.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make
