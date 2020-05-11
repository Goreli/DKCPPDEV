# DKUTIL.LIB - C++ Utilities Library
<p align="center">To use, or to reuse, that is the question.</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Directory Content](#directory-content)
3. [Synopsis](#synopsis)
4. [Build Notes](#build-notes)
5. [Roadmap](#roadmap)
6. [Links](#links)

## Introduction

The purpose of the library is to provide C++ utility solutions for reuse in various applications.

## Directory Content
 The root directory of the DKUTIL.LIB library contains this readme.md file as well as the following subdirectories;

 * [dkutil](dkutil) - contains build artefacts;
 * [include](include) - contains library header files;
 * [review](review) - contains source modules currently under review or testing. These are candidate source modules that haven't made into the current set of library source modules yet;
 * [source](source) - current set of library source modules.


## Synopsis
TBA
 
## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know the location of include area: ../include
4. Provide the _CONSOLE preprocessor definition to enable color output in Windows CLI;


No optimisation has been used in the Debug configuration. Specifically in the Release configuration the "Favour Speed over Size" optimisation option may be desired.

## Roadmap
TBA

## Links
TBA
