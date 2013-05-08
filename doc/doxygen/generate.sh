#!/bin/bash

set -e

doxygen doxygen.conf
cd latex
pdflatex refman.tex
pdflatex refman.tex
