#
# makefile for gnu make (gmake)
#

TEXTS= macros.tex remerciements.tex resume.tex  introduction.tex  probleme.tex litterature.tex appendice1.tex appendice2.tex


FIGURES= Figures/cache.eps

MAIN=    memoire
 
all:	$(MAIN).pdf

$(MAIN).dvi:	$(MAIN).tex $(TEXTS) $(FIGURES) memoireuqam1.3.cls
	latex $(MAIN)
	bibtex $(MAIN)
	latex $(MAIN)
	latex $(MAIN)

$(MAIN).ps:	$(MAIN).dvi
	@echo "--- Generating postscript file ---"
	dvips -f < $(MAIN).dvi > $(MAIN).ps


$(MAIN).pdf:	$(MAIN).ps
	@echo "--- Generating pdf file ---"
	ps2pdf $(MAIN).ps

clean:
	rm -f *~ Text/*~ $(MAIN).ps $(MAIN).dvi $(MAIN).log $(MAIN).lot $(MAIN).lof $(MAIN).toc *.aux $(MAIN).aux $(MAIN).blg Text/*.bak $(MAIN).pdf $(MAIN).bbl *.bak

