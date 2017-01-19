#!/bin/bash

# OctaveMatrixHeader MatrixName Rows Cols

echo "# name: ${1}" > ${1};
echo "# type: matrix" >> ${1};
echo "# rows: ${2}" >> ${1};
echo "# columns: ${3}" >> ${1};

