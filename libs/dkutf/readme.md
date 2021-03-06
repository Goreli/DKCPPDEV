# DKUTF.LIB - Unit Testing Framework
<p align="center">C++ library with support for collaboration and large suites of tests.</p><p align="center">Unit test the universe.</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Build Notes](#build-notes)
3. [Roadmap](#roadmap)

## Introduction

The purpose of the library is to provide a simple convenient programming interface in order to efficiently implement large suites of organised and well managed unit tests.

The library enables collaboration between several unit test developers. This is achieved by offering a workflow that consists of several independent tasks performed while working directly on the source code. Such separation of acitivities leads to a shorter time to market:
 - Plan and document groups of unit tests;
 - Plan, document and lay out unit tests;
 - Code unit tests;
 - Execute unit tests.

The library tracks relationship between group entities and unit tests pertaining to those entities. If there are any referential integrity issues in this relationship the library will identify and report them accordingly. 

Up to a git commit# 101b80e76fe509b8616938fe7e126085d29d41d5 it provided an instrumentation tool to monitor balance between the counts of object constructor execution and object destructor execution. Also, it used to detect certain cases of object corruption. This functionality has been removed in the recent versions because it's orthogonal to the main purpose and shouldn't be there in the library. The source code can still be found in the review folder.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the C++ compiler know the location of include areas: ../include;../../dkutil/include

## Roadmap

1. Standard CLI options;
2. Add support for Standard Error Output and Standard Log Output;
3. Implement a multilevel hierarchical structure of groups. While the lowest level of the hierarchy is hard coded in the testing application itself, it should be possible to build custom hierarchies on top of it to enable advanced filtering of tests. This is going to be handy in large scale testing applications involving hundreds/thousands of unit tests. The plan looks like this: provide a command line option to start a GUI hierarchy editor and edit the hierarchies. Also, provide a command line option to filter (XSLT/XPATH ?) tests using the hierarchies;
4. GUI to support the hierarchical structure. Generate source file templates. IDE add-ons;
5. Dynamic link libraries of unit tests. The user to choose wich dynamic link library of unit tests to load and execute;
6. Multithreading.
7. Doco.
