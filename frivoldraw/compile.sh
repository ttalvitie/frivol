#!/bin/bash

emcc frivoldraw_lib.cpp -o frivoldraw_lib.js -I.. -Iinclude/ -std=c++0x -s EXPORTED_FUNCTIONS="['_frivoldraw_ComputeDiagram', '_frivoldraw_FreeDiagram']"
