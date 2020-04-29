#
# Appel pour une autre machine que java, par exemple MacOS:
#
# make SERVER=MacOS


def: mk_graphes

SERVER=MacOS
SERVER=c34581
SERVER=java

mk_graphes:
	./run_gnuplot.sh $(SERVER)


clean cleanxtra:
	rm -f script.plot
