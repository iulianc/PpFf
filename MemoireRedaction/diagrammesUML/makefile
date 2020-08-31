#
# Makefile pour generation des diagrammes UML.
#

default: flow-details # objets3 #objets1 all

objets1:
	AVEC_FF=0 java -jar ~/Software/plantuml.jar objets1.txt
	\mv -f objets1.png objets1-ppff.png
	AVEC_FF=1 java -jar ~/Software/plantuml.jar objets1.txt
	\mv -f objets1.png objets1-ff.png
	open objets1*.png

objets2:
	AVEC_FF=0 java -jar ~/Software/plantuml.jar objets2.txt
	\mv -f objets2.png objets2-ppff.png
	AVEC_FF=1 java -jar ~/Software/plantuml.jar objets2.txt
	\mv -f objets2.png objets2-ff.png
	open objets2*.png

objets3:
	AVEC_FF=0 java -jar ~/Software/plantuml.jar objets3.txt
	\mv -f objets3.png objets3-ppff.png
	AVEC_FF=1 java -jar ~/Software/plantuml.jar objets3.txt
	\mv -f objets3.png objets3-ff.png
	open objets3*.png

objets4:
	java -jar ~/Software/plantuml.jar objets4.txt
	open objets4.png

all:
	java -jar ~/Software/plantuml.jar all.txt
	open all.png

vue:
	java -jar ~/Software/plantuml.jar vueEnsemble.txt
	open vueEnsemble.png

flow:
	java -jar ~/Software/plantuml.jar flow.txt
	open flow.png

flow-details:
	java -jar ~/Software/plantuml.jar flow-details.txt
	open flow-details.png


clean cleanall:
	\rm -f *~ *.png

# LocalWords:  Makefile
