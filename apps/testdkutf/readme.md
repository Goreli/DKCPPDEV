# TESTDKUTF.EXE

This is a console application that executes a suite of unit tests to test functionality of the [../../libs/dkutf](../../libs/dkutf) unit test library. It can also be used as a boiler plate set of files for starting new unit test applications.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Set the language standard to the most recent version;
2. Let the c++ compiler know locations of include areas: ../include;../../../libs/dkutf/include
3. Switch precompiled headers off;
4. Let the linker know the location of the dkutf.lib library (add a reference to the project on the Visual C++ platform).
