#!/bin/bash

echo "Cleaning all build files"

rm -rfv build/*
rm -rfv bin/*
rm -rvf scripts/oct
find . -name Makefile -delete -print
find . -regex ".*~" -delete -print
find . -regex ".*pro.user.*" -delete -print

