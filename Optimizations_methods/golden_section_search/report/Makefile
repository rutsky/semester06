# Makefile
# Simple building latex document Makefile.
# Vladimir Rutsky <altsysrq@gmail.com>
# 22.03.2009

SOURCE  = golden_section_search
DEPENDENCY =
TEX2PDF = pdflatex

all: $(SOURCE).pdf

$(SOURCE).pdf: $(SOURCE).tex $(DEPENDENCY)
	$(TEX2PDF) $(SOURCE).tex

clean:
	rm -f *~
	rm -f *.aux
	rm -f *.dvi
	rm -f $(SOURCE).log

distclean: clean
	rm -f $(SOURCE).pdf
