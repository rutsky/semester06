# Makefile
# Simple building latex document Makefile.
# Vladimir Rutsky <altsysrq@gmail.com>
# 22.03.2009

SOURCES = ed_system_reform
DEPENDENCY = 
TEX2PDF = pdflatex

PDF_TARGETS := $(foreach A,$(SOURCES),$(A).pdf)

pdf: $(PDF_TARGETS)

all: pdf

%.pdf: %.tex $(DEPENDENCY)
	$(TEX2PDF) $(basename $@).tex

clean:
	rm -f *~
	rm -f $(foreach A,$(SOURCES),$(A).aux $(A).dvi $(A).tex.backup $(A).log)

distclean: clean
	rm -f $(PDF_TARGETS)
