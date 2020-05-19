# TESTDKMRX.EXE

This is a console application that executes a suite of unit tests to test functionality of the [../../libs/dkmrx](../../libs/dkmrx) matrix library. Uses the [../../libs/dkutf](../../libs/dkutf) library for the instrumentation purposes.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Set the language standard to the most recent version;
2. Let the c++ compiler know locations of include areas: ../include;../../../libs/dkmrx/include;../../../libs/dkutf/include;../../../libs/dkutil/include
3. Switch precompiled headers off;
4. Define the real type the same as what has been used to build the dkmrx.lib library e.g. real=double.
5. Let the linker know the location of the dkmrx.lib, dkutf.lib and dkutil.lib libraries (add respective references to the project on the Visual C++ platform).
6. Define _CRT_SECURE_NO_WARNINGS to remove "This function or variable may be unsafe" errors triggered by some string and file functions.


## Roadmap
1. Need command line options to navigate the groups of tests available.
