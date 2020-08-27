#
# Makefile pour generation des diagrammes UML.
#

default: objets1 objets2 # all #objets1 all

objets1:
	java -jar ~/Software/plantuml.jar objets1.txt
	open objets1.png

objets2:
	java -jar ~/Software/plantuml.jar objets2.txt
	open objets2.png

objets3:
	java -jar ~/Software/plantuml.jar objets3.txt
	open objets3.png

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


clean cleanall:
	\rm -f *~ *.png
