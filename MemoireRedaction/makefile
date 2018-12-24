#
# makefile for gnu make (gmake)
#

TEXTS= macros.tex remerciements.tex resume.tex  introduction.tex  description.tex appendice1.tex appendice2.tex

FIGURES= Figures/cache.eps

MAIN=    memoire
 
$(MAIN): $(MAIN).tex $(TEXTS) $(FIGURES) memoireuqam1.3.cls
	pdflatex $(MAIN)
	bibtex $(MAIN)
	pdflatex $(MAIN)
	pdflatex $(MAIN)

clean cleanxtra:
	rm -f *~ Text/*~ $(MAIN).log $(MAIN).lo[lt] $(MAIN).lof $(MAIN).toc *.aux $(MAIN).aux $(MAIN).blg Text/*.bak $(MAIN).pdf $(MAIN).bbl *.bak


xv:
	evince memoire.pdf
