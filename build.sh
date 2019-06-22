#!/bin/sh

rm *

$HOME/Qt/5.13.0/gcc_64/bin/qmake $1/stardust-ui.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make
