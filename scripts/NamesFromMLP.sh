#!/bin/bash

# Extract the names from an mlp file

cat $1 | grep fileName | sed 's/.*fileName="\([^"]*\)".*/\1/'
