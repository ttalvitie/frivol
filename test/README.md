Library test suites using Boost Test Framework.

Compile with cmake . && make, then run ./test.

If you want to choose which compiler you want to use for compiling, you can select it with the CMAKE_CXX_COMPILER variable. For example to select g++-4.7 (for example on University of Helsinki CS department computers):
cmake . -DCMAKE_CXX_COMPILER=g++-4.7 && make
