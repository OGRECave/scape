# Programmer's Guide - Scape

**Giliam de Carpentier**
<http://www.decarpentier.nl/scape>
June 9th, 2012

## Build instructions for CMake

Scape uses CMake as its build system. You may follow the instructions
below to build Scape.

### Dependencies

To build Scape you will need OGRE and Qt. You can download both as
prebuilt packages or compile it yourself. Especially for Qt it may be
easier to download them. For OGRE, however, it can be helpful to build
it yourself to customize it.

  - OGRE: <https://github.com/OGRECave/ogre>
  - Qt: <https://www.qt.io/download>

### Preparing the build environment

To build Scape you will obviously need the source code. You can download
it or use *git* to obtain it from [the
repository](https://github.com/OGRECave/scape).

To clone with git use the following command: `git clone
https://github.com/OGRECave/scape.git`

After obtaining the source code you have to create a build directory.
You may create it outside of the source directory to keep it clean.

### Running CMake

Since there are not much options to configure it may be easier to run
CMake from the command line. Just go into your build directory and run
`cmake <source directory>`.

You could also use cmake-gui and configure the source and build
directories there. Afterwards you can click *Configure* followed by
*Generate*.

### Building

Go to your build directory. CMake has generated a build system which can
be used dependent on your platform. For Visual Studio you can open a
solution `Scape.sln` and build the target *BUILD\_ALL*. For Makefile
based generators you can just run the `make` command.

### Installing

Once the build is complete, you can optionally have the build system
copy the executable and config files to a clean location. For Visual
Studio you can build the *INSTALL* target. For Makefile based generators
you can just run the `make install` command.

## C++ architecture

The codebase is split into two major components: The Engine library and
the qtEditor application. All UI-related code can be found in the
qtEditor project. Everything else is in Engine. The Engine project is
further grouped by functionality into subfolders. A functional
description of each source code file in these subfolders can be found in
the corresponding header file.

  - */qtEditor* - GUI/Qt-related code
      - */res* - Icons / Images
  - */Engine* - 2D/3D-related code
      - */EngineCore* - Management and control of world, sky and
        cameras, as well as (UI) properties and settings
      - */GPU2DOperation* - 2D GPGPU bitmap / texture / rendertarget
        container and operation classes
      - */HeightfieldBrush* - User-controlled brush widget
      - */HeightfieldBuffer* - Heightfield / -set / -page data container
      - */HeightfieldFileCodecs* - Heightfield import / export class
        interfaces and implementations
      - */HeightfieldGeom* - Heightfield rendering
      - */HeightfieldOperation* - Brush operation implementations
      - */HeightfieldSerializer* - Heightfield serialization/compression
      - */Input* - Mouse/Keyboard/Wacom mapping and handling
      - */Utils* - Math and miscellaneous functions and classes
      - */Wintab* - Wacom driver headers

## Materials and shaders

The heightfield material is defined in
/Bin/Materials/Scripts/HeightfieldMaterial.material. The Ogre materials
for the GPU-based brushes are defined in
/Bin/Materials/Scripts/GPU2DOperation.material. These materials refer to
the following vertex and fragment shader code files in
/Bin/Materials/Programs.

  - *GPU2DOperation.cg* - The generic 1:1 vertex shader, and simple 1:1
    'copy' pixel shader
  - *GPU2DOperationAddMask.cg* - Experimental smoothing pixel shader
  - *GPU2DOperationNoiseDrag.cg* - Directional noise pixel shader
  - *GPU2DOperationNoiseMask.cg* - Procedural noise brush pixel shaders
  - *GPU2DOperationPencil.cg* - Smooth additive/subtractive brush pixel
    shader
  - *HeightfieldShaderCg.cg* - Heightfield terrain and brush widget
    rendering
  - *SkyBox.cg* - Simple shader for the sky box

## Settings

Many settings are stored in the following config files in /Bin/Settings.

  - *config.json* - Contains the size of the heightfield to be
    loaded/saved/edited and the mouse/keyboard mappings/shortcuts
    (read/write)
  - *Presets.json* - Tool presets set by user (read/write)
  - *UI.json* - Names, descriptions and types of UI widgets and
    properties (read-only)

## License

See */Docs/ScapeLicense.txt*

Copyright (c) 2007-2012, Giliam de Carpentier. All rights reserved.
