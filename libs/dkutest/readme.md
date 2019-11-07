# DKUTEST.LIB
<p align="center">Unit test framework with support for groups of tests</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Build Notes](#build-notes)
3. [Roadmap](#roadmap)

## Introduction

The purpose of the library is to provide a simple convenient programming interface for implementing suites of unit tests. The motivation is to be able to write well understood code that allows to group unit tests.

## Build Notes

Here is the MS Visual Studio configuration log used to build the target in both the Debug and Release configurations:
1. Switch precompiled headers off;
2. Set the language standard to the most recent version;
3. Let the c++ compiler know the location of include area: ../include

## Roadmap

1. Doco;
2. Implement a multilevel hierarchical structure of groups. While the lowest level of the hierarchy is hard coded in the testing application itself, it should be possible to build a custom hierarchy on top of it to enable advanced filtering of tests. This is going to be handy in large scale testing applications involving hundreds of unit tests. The plan looks like this: provide a command line option to start a GUI hierarchy editor to edit the hierarchy. Also, provide a command line option to filter (XSLT/XPATH ?) tests using the hierarchy.

