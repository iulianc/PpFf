#
# Appel pour une autre machine que java, par exemple MacOS:
#
# make SERVER=MacOS


_def: mk_graphes

test:
	cat temps-MacOS-wc.txt
	ruby min_temps.rb < temps-MacOS-wc.txt
	ruby max_temps.rb < temps-MacOS-wc.txt


SERVER=c34581
SERVER=java
SERVER=MacOS

mk_graphes:
	./run_gnuplot.sh $(SERVER)


clean cleanxtra:
	rm -f script.plot
