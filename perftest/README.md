Performance tester for frivol.

Compile with cmake . && make, then run ./perftest.

The program will compute Voronoi diagrams of uniform random sets of point sites with both default Policy and Policy using DummyPriorityQueue and DummySeachTree. The output will be written to files default_out.txt and dummy_out.txt as site count - run time -pairs.