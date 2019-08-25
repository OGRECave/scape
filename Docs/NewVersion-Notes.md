# New and Noteworthy in the new Scape version

This is a brief overview over the changes since the original [release by
Giliam de Carpentier](http://www.decarpentier.nl/scape). To see detailed
changes, you need to look in the git change log.

## General improvements

  - It has been updated to work with the latest OGRE version.
  - It now requires C++11.
  - The platform compatibility has been improved. It works on Windows
    and Linux.
  - The documentation has been improved and updated.
  - OIS has been replaced by Qt input.
  - The configuration files which used some custom format have been
    converted to JSON.
  - Support for OpenEXR has been added.
  - The original Visual Studio project files have been replaced by a
    CMake build system.
  - The source code has been formatted and normalized to UNIX-style line
    endings.
  - Much unnecessary/old code has been removed.
  - Many improvements to the code quality/structure have been made.
  - Many small improvements have been made.
  - The use of the shaders has been improved.

## UI improvements

  - The editor has been ported from wxWidgets to Qt.
  - Many dialogs have been re-implemented using Qt.
  - A brush settings widget has been added which lets one change the
    brush size and related settings by a slider.
  - A startup settings dialog has been added which lets one change the
    terrain size and related settings easily.
  - A button definition dialog has been added which lets one change
    shortcuts easily.
