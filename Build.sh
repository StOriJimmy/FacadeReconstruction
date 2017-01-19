#!/bin/bash

echo "Checking pre-requisites"
if [ ! -e  /usr/local/lib/libopencv_core.dylib ]
then 
  echo "OpenCV is not installed in /usr/local"
  echo "Install from homebrew using the command:"
  echo "  brew install opencv"
  exit 1
fi

echo "Building..."
ver=$(qmake --version | grep "version 5" | wc -c)

if((ver == 0))
then
  echo "Searching for valid qmake"
  qtpath=$( mdfind -name "qmake" | grep "bin/qmake" | grep 5 | tail -n 1)
  echo $qtpath
  th=$(echo $qtpath | wc -c)
  if((th == 0))
  then
    echo "QMake version >= 5 is required"
    exit 1
  else
    $($qtpath .)
    make -f Makefile
    echo "done"
  fi
else
  $(exec qmake .)
  make -f Makefile
fi

oct=$(which mkoctfile | wc -c)

if((oct == 0))
then
  echo "Octave does not appear to be installed."
  echo "Please ensure that the executable 'mkoctfile' is on the system path".
else
  cd scripts
  make -f makefile
  cd ..
fi



