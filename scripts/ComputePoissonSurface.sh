#!/bin/bash

#
# Use the meshlab server to run a script which
#  - Computes the vertex normals using 50 nearest neighbours and smoothing
#  - Smooths the normals with weighting
#  - Normalises the normals.
#  - Find the Poisson implicit surface with a surface offset of 0.1
#
# It should be run
#  ./ComputePoissonSurface input.ply output.ply

mvs=$(which meshlabserver | wc -c)
if(( mvs==0 ))
then
  echo "Locating valid meshlab"
  echo "* If this fails, add meshlab to the system path, and add the plugins to the DYLD_LIBRARY_PATH"
  base_folder=$(mdfind -name meshlab | grep meshlab.app | head -n 1)
  app_folder=Contents/MacOS
  plugin_folder=Contents/plugins
  echo
  echo "Adding to system path"
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$base_folder/$plugin_folder:$base_folder/$app_folder
  export PATH=$PATH:$base_folder/$app_folder
fi
echo $DYLD_LIBRARY_PATH
echo $PATH
mvs=$(which meshlabserver | wc -c)

a1=$(echo $1 | wc -c)
a2=$(echo $2 | wc -c)
echo "Compute Poisson Surface"
echo 
echo " input: "$1
echo " output: "$2
echo
if(( mvs==0 ))
then
  echo "meshlabserver does not appear to be installed."
  echo "Make sure that the executable, and dynamic libraries, are on the system path."
  exit 1
elif(( a1<=1 || a2<=1 ))
then
  echo "please specify source and target mesh files"
  echo "./ComputePoissonSurface inputfile outputfile"
  exit 2
else
  DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
  meshlabserver -om vn -i $1 -o $2 -s $DIR/ComputePoissonSurface.mlx
  exit $?
fi
