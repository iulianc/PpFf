#
# Makefile pour generation des diagrammes UML.
#

default: AllNew

Objets1:
	java -jar ~/Software/plantuml.jar diagrammeObjets1.txt
	open diagrammeObjets1.png

All:
	java -jar ~/Software/plantuml.jar diagrammeAll.txt
	open diagrammeAll.png

AllNew:
	java -jar ~/Software/plantuml.jar diagrammeAll_new-version.txt
	open diagrammeAll_new-version.png

Vue:
	java -jar ~/Software/plantuml.jar diagrammeVueEnsemble.txt
	open diagrammeVueEnsemble.png

Flow:
	java -jar ~/Software/plantuml.jar diagrammeFlow.txt
	open diagrammeFlow.png
