#!/bin/bash
PROJ=randomtools
VERSION=1.0
BUILD=$PROJ-$VERSION
if [ -d ../$BUILD ] ; then rm -Rf ../$BUILD; fi
hg archive ../$BUILD
cd ..
tar -cjvf $BUILD.tar.bz2 $BUILD
pwd
cd $BUILD
pwd
dh_make -m -c bsd -e oliver@ersatzworld.net -f ../$BUILD.tar.bz2
