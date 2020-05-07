#
# Appel pour une autre machine que la machine courante:
#
# make SERVER=foo

def: test

debits:
	./plot.sh 'WordCount' 'debits' '3,4,7' "Meilleurs programmes MacBook" 1
	./plot.sh 'WordCount' 'debits' '3,4,7' "Meilleurs programmes MacBook" 0
#	./plot.sh 'WordCount' 'temps' '3,4,7' "Meilleurs programmes MacBook" 1
#	./plot.sh 'WordCount' 'temps' '*' "Meilleurs programmes MacBook" 0
#	./plot.sh 'WordCount' 'temps' '3,4,7' "Meilleurs programmes MacBook" 0

test:
#	./plot.sh 'WordCount' 'temps' 2 '*' "Tous les programmes"
	./plot.sh 'WordCount' 'temps'  MacOS 2 '*'   "Tous les programmes" 1
	./plot.sh 'WordCount' 'debits' MacOS 2 '1,4' "Programmes 1 et 4"
#	./plot.sh 'WordCount' 'debits' 2 '1,4' "programmes 1 et 4" 1
#	./plot.sh 'Foo' 'temps' 5 '1,4' "programmes 1 et 4" 1 BAR
#	./plot.sh 'WordCount' 'temps' '1,2,3' 2 "Programmes Java" 1
#	./plot.sh 'WordCount' 'temps' '4,5,6' "Programmes PpFF" 1
#	./plot.sh 'WordCount' 'temps' '7,8,9' "Programmes FastFlow" 1
#a	./plot.sh 'WordCount' 'temps' '2,6,7' "Meilleurs programmes" 1
#	./plot.sh 'WordCount' 'temps' '3,4,7' "Meilleurs programmes (MacOS)" 1
#	./plot.sh 'WordCount' 'temps' '2,3,6' "Java-/Java* PpFF-4" 1

WordCount:
	./plot.sh 'WordCount' 'temps' '*' "Tous les programmes" 0
	./plot.sh 'WordCount' 'temps' '*' "Tous les programmes" 1
	./plot.sh 'WordCount' 'debits' '*' "Tous les programmes" 0
	./plot.sh 'WordCount' 'debits' '*' "Tous les programmes" 1

StockPrice:
	./plot.sh 'StockPrice' 'temps' '*' "Tous les programmes" 0
	./plot.sh 'StockPrice' 'temps' '*' "Tous les programmes" 1
	./plot.sh 'StockPrice' 'debits' '*' "Tous les programmes" 0
	./plot.sh 'StockPrice' 'debits' '*' "Tous les programmes" 1

clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

