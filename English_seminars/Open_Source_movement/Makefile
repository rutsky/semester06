# Makefile
# Simple building latex document Makefile.
# Vladimir Rutsky <altsysrq@gmail.com>
# 22.03.2009

SOURCE  = Rutsky_FSF_and_OSS_movements_Help_Sheet
TEX2PDF = pdflatex

all: $(SOURCE).pdf

$(SOURCE).pdf: $(SOURCE).tex
	$(TEX2PDF) $(SOURCE).tex

clean:
	rm -f *~
	rm -f *.aux
	rm -f *.dvi
	rm -f *.log

distclean: clean
	rm -f *.pdf
