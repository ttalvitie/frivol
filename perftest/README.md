Performance tester for frivol.

Compile with cmake . && make, then run ./perftest.

The program will compute Voronoi diagrams of uniform random sets of point sites with both default Policy and Policy using DummyPriorityQueue and DummySeachTree. The output will be written to files default_out.txt and dummy_out.txt as site count - run time -pairs.

If you want to choose which compiler you want to use for compiling, you can select it with the CMAKE_CXX_COMPILER variable. For example to select g++-4.7 (for example on University of Helsinki CS department computers):
cmake . -DCMAKE_CXX_COMPILER=g++-4.7 && make
