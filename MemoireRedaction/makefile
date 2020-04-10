#
# makefile for gnu make (gmake)
#

MAIN=memoire

# Si COLORED_LINKS=true, alors dans le document PDF, les liens et
# references sont en couleur et, surtout, sont navigables, ce qui est
# bien utile lorsqu'on visualise directement la version PDF. Or,
# depuis l'automne 2019, c'est uniquement la version PDF qui est
# remise aux examinateurs. On peut mettre a false si on veut imprimer
# une version papier/noir et blanc.
# 
COLORED_LINKS=false
COLORED_LINKS=true


$(MAIN): colored_links
	pdflatex $(MAIN)

bib:
	pdflatex $(MAIN)
	bibtex $(MAIN)
	pdflatex $(MAIN)
	pdflatex $(MAIN)

colored_links:
	echo "\setboolean{ColoredLinks}{$(COLORED_LINKS)}" > latex/colored_links.tex

clean cleanxtra:
	rm -f *~ Text/*~ $(MAIN).log $(MAIN).lol $(MAIN).lot $(MAIN).lof $(MAIN).toc *.aux $(MAIN).aux $(MAIN).blg Text/*.bak $(MAIN).pdf $(MAIN).bbl *.bak $(MAIN).out
	rm -f diff.*
	rm -f flat.tex

xv:
	evince $(MAIN).pdf &

