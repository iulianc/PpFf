#
# Makefile pour generation des diagrammes UML.
#

default: all_new

objets1:
	java -jar ~/Software/plantuml.jar objets1.txt
	open objets1.png

all:
	java -jar ~/Software/plantuml.jar all.txt
	open all.png

all_new:
	java -jar ~/Software/plantuml.jar all_new-version.txt
	open all_new-version.png

vue:
	java -jar ~/Software/plantuml.jar vueEnsemble.txt
	open vueEnsemble.png

flow:
	java -jar ~/Software/plantuml.jar flow.txt
	open flow.png
