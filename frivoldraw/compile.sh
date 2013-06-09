#!/bin/bash

emcc frivoldraw_lib.cpp -o frivoldraw_lib.js -I.. -Iinclude/ -std=c++0x -s EXPORTED_FUNCTIONS="['_frivoldraw_ComputeVoronoi', '_frivoldraw_FreeVoronoi', '_frivoldraw_GetEdgeCount', '_frivoldraw_GetStartX', '_frivoldraw_GetStartY', '_frivoldraw_GetEndX', '_frivoldraw_GetEndY', '_malloc', '_free']"
