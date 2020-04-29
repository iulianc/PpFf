#
# Appel pour une autre machine que java, par exemple MacOS:
#
# make SERVER=MacOS


def: mk_graphes

SERVER=java
SERVER=MacOS

mk_graphes:
	./run_gnuplot.sh $(SERVER)


clean cleanx:
	rm -f script.plot
