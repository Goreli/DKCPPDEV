# DKCPPDEV - Collection of C++ Libs and Apps
<p align="center">Seen The Matrix?</p><p align="center">Unit test the universe.</p><p align="center">To use, or to reuse, that is the question.</p>

___

## Table of Contents
1. [Repo Directory Structure](#repo-directory-structure)
2. [Build Notes](#build-notes)


## Repo Directory Structure

The root directory contains git specific configuration files e.g. .gitignore and .gitattributes. It also contains a Visual Studio solution file that builds all targets in this repo, a LICENSE file and this README.md file. The root directory contains the following subdirectories:
 * [apps](apps) - for applications;
 * [libs](libs) - for libraries;
 * [util](util) - cmake automation utilities.

Refer individual readme.md files located in subdirectories of the directories listed above for detailed description of the respective content. A high level overview of the content follows.
 
### Content of the [apps](apps) directory 
 * [apps/winbmp2mrx](apps/winbmp2mrx) - a Windows Console application that converts Windows Bitmap files into DKMRX Matrix data files. The data files can then be consumed by the [apps/winraster](apps/winraster) sample application;
 * [apps/wincubes](apps/wincubes) - a sample Windows Desktop application that demonstrates how to use the [libs/dkmrx](libs/dkmrx) matrix library to perform 3D coordinate transformation in **Vector Graphics** applications;
 * [apps/winraster](apps/winraster) - a sample Windows Desktop program that demonstrates how to use the [libs/dkmrx](libs/dkmrx) matrix library to perform 3D coordinate transformation in **Raster Graphics** applications;
 * [apps/testdkmrx](apps/testdkmrx) - A console application that executes a collection of unit tests to test the [libs/dkmrx](libs/dkmrx) library. Uses the [libs/dkutf](libs/dkutf) library;
 * [apps/testdkutf](apps/testdkutf) - A console application that executes a suite of unit tests to test functionality of the [../../libs/dkutf](../../libs/dkutf) unit test library itself. It can also be used as a boiler plate set of files for starting new unit test applications.

### Content of the [libs](libs) directory 
 * [libs/dkmrx](libs/dkmrx) - A C++ library with a simple programming interface for coding matrix operations;
 * [libs/dkutf](libs/dkutf) - A C++ unit testing framework library that allows to keep large suites of unit tests organised and well managed. Offers a collaboration workflow that leads to a shorter time to market. Also, provides an instrumentation tool to monitor balance between the counts of object constructor execution and object destructor execution. Can detect certain cases of object corruption;
 * [libs/dkutil](libs/dkutil) - A library of reusable C++ utility classes and routines.

### Content of the [util](util) directory 
TBA

## Build Notes

This repo has been tested using the following platform(s) and attributes:
1. Visual Studio 2019.
   - OS: Windows 10;
   - Solution Platform: 64 bit;
   - Configurations: Debug and Release;
   - Language Standard: Preview - Features from the Latest C++ Working Draft.

Open the dkcppdev.sln Visual Studio 2019 Solution file located in the root directory to build all the targets.

Earlier versions of the [libs/dkmrx](libs/dkmrx) library are known to compile on non-Windows platforms such as Silicon Graphics, Solaris and Ubuntu. This repo does not contain any build artefacts used in those builds. 

Should there be a need to set up a different build system (CMake?) refer individual readme.md files located in the source code directories. These files contain detail that can be used for the build configuration purposes. Also, here is some interesting information on how to generate makefiles from Visual Studio Solution files:
https://sourceforge.net/projects/vcproj2cmake/ (I haven't tried to use it yet).
