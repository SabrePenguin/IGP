# IGP
Irregular Grid Painting Program written under the Qt Framework

Created in Qt 4.8

First version written using VB.net framework by another author.
More information at the following link:
http://www.zlosk.com/pgmg/igp/downloads.html

The second version written using javascript.
Project at following link:
https://github.com/aganim/canvas-igp

Tips for compiling:
Ubuntu:
Required library: qt-sdk
Run Commands:
qmake
make

Windows:
Use QT Creator: https://www.qt.io/download-open-source/#section-6
Convert QT4 to QT5 libraries: http://qt-project.org/wiki/Transition_from_Qt_4.x_to_Qt5
The first section is all that is really needed.
Remove two function calls the substr() in Pattern.cpp. These are for ignoring windows characters found in the original formats of the .ini pattern files. If you do not remove them, the program will attempt to load .pn files instead of .png files and segfault upon attempting to load any pattern.
