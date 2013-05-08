#!/bin/bash

doxygen doxygen.conf
cd latex
pdflatex refman.tex
pdflatex refman.tex
