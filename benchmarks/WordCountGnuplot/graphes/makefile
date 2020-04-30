#
# Appel pour une autre machine que la machine courante:
#
# make SERVER=foo


_def: mk_graphes

test:
	cat temps-MacOS-wc.txt
	ruby min_temps.rb < temps-MacOS-wc.txt
	ruby max_temps.rb < temps-MacOS-wc.txt


SERVER=$(HOST)

mk_graphes:
	./plot_temps.sh 0
	./plot_temps.sh 1


clean cleanxtra:
	rm -f script.plot
