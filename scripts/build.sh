#!/bin/sh

rm *

qmake $1/stardust-ui.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make
