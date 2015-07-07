About 24U FM Template 2.2
-------------------------

Important: The latest version of 24U FM Template is available for
download from:

http://24uSoftware.com/FMTemplate  or  http://www.NewMillennium.com/

--------------------------


Introduction
-------------

This product is a template project for developing third-party FileMaker
external function plug-ins. It is intended for beginning plug-in
developers as well as for experienced developers who don't want to spend
time configuring parts of the FileMaker Plug-In SDK, but want to
concentrate on developing new functionality for FileMaker Pro, FileMaker
Pro Advanced, and FileMaker Server.

The following development environments are supported in a form of ready
to use project files by this version of 24U FM Template:

  Apple XCode 2.3
  Microsoft Visual Studio .NET 2003

If you have a different version of one of these environments you may
need to adjust settings to be able to build plug-ins.


Quick Start
------------

To develop your own FileMaker plug-in using this template, just follow
these easy steps:

  1. Make a copy of the template folder and name the copy after your
     new plug-in.
  2. Replace FMPlugInSDK in Support folder with the FMPlugInSDK folder 
     from your FileMaker Pro Advanced CD-ROM.
  3. Open the project file matching your development environment
     and try to build all targets to make sure your development
     environment has all necessary includes and libraries in place.
  4. Open the "FMTConfig.h" header file and setup the constants
     appropriately.
  5. For each external function you want to implement, add a new file
     with the function code, then define the function's name, flags,
     parameters, and prototype in the "FMTConfig.h" header file.
  6. Use the predefined "Debug" targets/styles/configurations to find
     and fix any bugs in your code.
  7. Build the release version of your plug-in using one of the
     predefined "Release" targets/styles/configurations.
  8. Enjoy your new FileMaker external function plug-in.


Finder note:

  Do not rename built plugin in Finder. Binary name can be changed 
  in development environment.

Plugin name note:

  Plugin name should not begin by number (ie. 3DPlugin.fmplugin).
  If so it will have an incorrect behaviour in FileMaker 8.0 on Mac OS X.
  This is probably a bug in FileMaker.


Getting technical support
--------------------------

Neither 24U Software nor New Millennium Communications warrants to 
provide technical support for this product. However, we appreciate 
any feedback you send to us through our on-line technical support 
form, and we will try to answer your questions.

The latest version of 24U FM Template is available for download from:

http://24uSoftware.com/FMTemplate  or  http://www.NewMillennium.com/
