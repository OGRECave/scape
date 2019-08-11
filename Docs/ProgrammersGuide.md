# Programmer's Guide - Scape

**Giliam de Carpentier**
<http://www.decarpentier.nl/scape>
June 9th, 2012

## Build instructions for Microsoft Visual C++ 2008 (Express)

Currently, Scape only comes with project and solution files for
Microsoft Visual Studio C++ 2008 (Express). Follow the instructions
below to build Scape's dependencies, to build Scape itself, and finally
to run it.

1.  Download and install a graphics driver and Microsoft DirectX 9.0, if
    not already done so
      - Install a recent graphics driver for your specific hardware
      - Download and install the Microsoft DirectX 9.0c runtime from
        <http://www.microsoft.com/en-us/download/details.aspx?id=35>
2.  Download and install the Microsoft C++ redistributables, if not
    already done so
      - Download and install the Microsoft Visual C++ 2008
        Redistributable Package (x86) package from
        <http://www.microsoft.com/en-us/download/details.aspx?id=5582>
3.  Extract Scape
      - Download the Scape source package, if not already done so. See
        <http://www.decarpentier.nl/scape> for details on how to do
        this.
      - Extract the ZIP to your preferred destination base folder,
        called *\<PROJECT\>* for the remainder of this document
4.  Install and build wxWidgets 2.8.3
      - Download
        <http://sourceforge.net/projects/wxwindows/files/wxAll/2.8.3/wxWidgets-2.8.3.zip/download>
      - Extract the download to *\<PROJECT\>/Dependencies/*
      - Copy *\<PROJECT\>/Dependencies/wxWidgets-2.8.3-overwrite/lib*
        over the *\<PROJECT\>/Dependencies/wxWidgets-2.8.3/lib* folder
      - Open
        *\<PROJECT\>/Dependencies/wxWidgets-2.8.3/build/msw/wx\_dll.dsw*
        with Visual Studio 2008 (Express) and convert all projects
      - Set the current configuration to *DLL Debug* and only build
        project *aui*
      - Set the current configuration to *DLL Release* and only build
        project *aui*
5.  Install and build wxPropertyGrid 1.2.12
      - Download
        <http://sourceforge.net/projects/wxpropgrid/files/wxPropertyGrid/1.2.12/wxpropgrid-1.2.12-win.exe/download>
      - Start the installer and set the destination folder to
        *\<PROJECT\>/Dependencies/wxWidgets-2.8.3*. (don't worry about a
        potential windows compatibility warning when the installer is
        finished)
      - Open
        *\<PROJECT\>/Dependencies/wxWidgets-2.8.3/contrib/build/propgrid/propgrid.dsw*
        with Visual Studio 2008 (Express) and convert project
      - Set the current configuration to *DLL Debug* and build project
        *propgrid*
      - Set the current configuration to *DLL Release* and build project
        *propgrid*
      - Copy the following 8 built DLLs from
        *\<PROJECT\>/Dependencies/wxWidgets-2.8.3/lib/vc\_dll/* to
        *\<PROJECT\>/Bin/* : *wxbase28\_vc\_custom.dll,
        wxmsw28\_aui\_vc\_custom.dll, wxmsw28\_core\_vc\_custom.dll,
        wxmsw28\_propgrid\_vc\_custom.dll wxbase28d\_vc\_custom.dll,
        wxmsw28d\_aui\_vc\_custom.dll, wxmsw28d\_core\_vc\_custom.dll
        and wxmsw28d\_propgrid\_vc\_custom.dll* (so, basically every DLL
        built in step 4 and 5 except for
        *wxmsw28(d)\_adv\_vc\_custom.dll*)
6.  Install Ogre3D SDK 1.8.0
      - Download
        <https://sourceforge.net/projects/ogre/files/ogre/1.8/1.8.0/OgreSDK_vc9_v1-8-0.exe/download>
      - Start the install and chooise any destination folder (for
        example, *\<PROJECT\>/Dependencies/OgreSDK*). The remainder of
        this document will call the chosen Ogre destination folder
        *\<OGRE\_HOME\>*, after the environment variable *OGRE\_HOME*
        that will be set automatically by the installer.
      - Copy *OIS\_d.dll, OgreMain\_d.dll,
        Plugin\_CgProgramManager\_d.dll,
        Plugin\_OctreeSceneManager\_d.dll* and
        *RenderSystem\_Direct3D9\_d.dll* from
        *\<OGRE\_HOME\>/bin/Debug/* to *\<PROJECT\>/Bin/*
      - Copy *cg.dll, OIS.dll, OgreMain.dll,
        Plugin\_CgProgramManager.dll, Plugin\_OctreeSceneManager.dll*
        and *RenderSystem\_Direct3D9.dll* from
        *\<OGRE\_HOME\>/bin/Release/* to *\<PROJECT\>/Bin/*
7.  Build and Run Scape
      - Open *\<PROJECT\>/Scape.sln*
      - Set the current configuration to *Debug* and only build project
        *Editor*
      - Set the current configuration to *Release* and only build
        project *Editor*
      - For Debug: Run *\<PROJECT\>/Scape\_d.exe*. For Release: Run
        *\<PROJECT\>/Scape.exe*. Alternatively, to run from within
        Visual Studio, set the 'working directory' under 'Debugging' in
        the 'Editor' project properties to *$(TargetDir)* and Run.
8.  OPTIONAL: Install Windows Sysinternals DebugView, displaying all
    (Ogre) log/debug output in real time when Scape is running
      - Download <http://download.sysinternals.com/files/DebugView.zip>
      - Extract the files from *DebugView.zip* and run *DbgView.exe*

## C++ architecture

The codebase is split into two major components: The Engine library and
the Editor application. All UI-related code can be found in the Editor
project. Everything else is in Engine. Each project is further grouped
by functionality into subfolders. A description of the function of each
source code file in these subfolders written in their header file.

  - */Editor* - GUI/wxWidgets-related code
      - */Application* - 'main' function and application class
      - */Controls* - Custom drag / spinner wxWidget implemention
      - */PCH* - Precompiled headers
      - */Resources* - Icons
      - */Utils* - Miscellaneous helper functions and classes
      - */Windows* - GUI windows, dialogs and panels
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
      - */PCH* - Precompiled headers
      - */Tinyxml* - XML parsing (third party)
      - */Utils* - Math and miscellaneous functions and classes
      - */Wintab* - Wacom driver headers

## Materials and shaders

The heightfield material is defined in
/Bin/Materials/Scripts/HeightfieldMaterial.material. The Ogre materials
for the GPU-based brushes are defined in
/Bin/Materials/Scripts/GPU2DOperation.material. These materials refer to
the following vertex and fragment shader code files in
/Bin/Materials/Programs.

  - *GPU2DOperation.cg* - The generic 1:1 vertex shader, and aimple 1:1
    'copy' pixel shader
  - *GPU2DOperationAddMask.cg* - Experimental smoothing pixel shader
  - *GPU2DOperationNoiseDrag.cg* - Directional noise pixel shader
  - *GPU2DOperationNoiseMask.cg* - Procedural noise brush pixel shaders
  - *GPU2DOperationPencil.cg* - Smooth additive/subtractive brush pixel
    shader
  - *HeightfieldShaderCg.cg* - Heightfield terrain and brush widget
    rendering

## Settings

Many settings are store in the following config files in /Bin/Settings.

  - *ButtonDefinitions.xml* - Mouse button and keyboard key to virtual
    'buttons' (read)
  - *Perspective.cfg* - Current UI layout set by user (read/write)
  - *Presets.conf* - Tool presets set by user (read/write)
  - *Startup.conf* - Size of the heightfield to be loaded/saved/edited
    (read-only)
  - *UI.conf* - Names, descriptions and types of UI widgets and
    properties (read-only)

## License

See */Docs/ScapeLicense.txt*

Copyright (c) 2007-2012, Giliam de Carpentier. All rights reserved.
