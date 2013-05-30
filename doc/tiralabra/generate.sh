#!/bin/bash

set -e

bash -c "cd ../doxygen/; ./generate.sh"
cp ../doxygen/latex/refman.pdf .

for i in 1 2
do
	pdflatex maarittelydokumentti.tex
	pdflatex viikkoraportti1.tex
	pdflatex viikkoraportti2.tex
	pdflatex viikkoraportti3.tex
done
