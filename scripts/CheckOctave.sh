#!/bin/bash

oct=$(which octave | wc -c)
if((oct == 0))
then
  echo "Octave is not installed"
  exit 1
else
  img=$(octave --eval "pkg list image" | grep "not installed" | wc -c)
  sta=$(octave --eval "pkg list statistics" | grep "not installed" | wc -c)
  bad=0
  if(( img > 0 ))
  then
    echo "Image toolbox is not installed"
    bad=1
  fi
  if(( sta > 0 ))
  then
    echo "Statistics toolbox is not installed"
    bad=1;
  fi
  if((bad==1))
  then
    exit 2
  fi
fi
exit 0
