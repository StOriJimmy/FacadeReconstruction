#!/bin/bash

# Extract the number of cameras from an mlp file

cat $1 | grep fileName | wc -l
