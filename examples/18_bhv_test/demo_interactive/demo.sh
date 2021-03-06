#!/bin/bash
#
# File:     demo.sh
# Descr:    Runs demo application with different command line arguments
# Author:   Anton Novoselov @ 2017
#
# ############################################################################

# Check which version present

if [[ -e demo ]]; then
  APP=demo
else
  APP=demo_debug
fi

# Middle world with lot of small objects

if [[ "$1" == 1 ]]; then
   ./"${APP}" ../test_object.ply 100 1000 0.5 3

elif [[ "$1" == 2 ]]; then
   ./"${APP}" ../test_object.ply 100 5000 0.2 4

elif [[ "$1" == 3 ]]; then
   ./"${APP}" ../../00_data/objects/cube_texture.ply 100 5000 0.2 4

fi