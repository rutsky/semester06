# Makefile
# Simple building latex document Makefile.
# Vladimir Rutsky <altsysrq@gmail.com>
# 22.03.2009

SOURCE  = linear_programming_problem
DEPENDENCY = matrix_A.tex vector_b.tex vector_c.tex
TEX2PDF = pdflatex

all: $(SOURCE).pdf

$(SOURCE).pdf: $(SOURCE).tex $(DEPENDENCY)
	$(TEX2PDF) $(SOURCE).tex

clean:
	rm -f *~
	rm -f *.aux
	rm -f $(SOURCE).log

distclean: clean
	rm -f $(SOURCE).pdf
