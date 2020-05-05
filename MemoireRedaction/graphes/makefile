#
# Appel pour une autre machine que la machine courante:
#
# make SERVER=foo

def: test

ff:
	./plot.sh 'WordCount' 'temps' '*' "Programmes FastFlow" 1

test:
	./plot.sh 'WordCount' 'temps' '*' "Tous les programmes" 1
	./plot.sh 'WordCount' 'temps' '1,2,3' "Programmes Java" 1
	./plot.sh 'WordCount' 'temps' '4,5,6' "Programmes PpFF" 1
	./plot.sh 'WordCount' 'temps' '7,8,9' "Programmes FastFlow" 1
	./plot.sh 'WordCount' 'temps' '2,6,7' "Meilleurs programmes" 1
	./plot.sh 'WordCount' 'temps' '3,4,7' "Meilleurs programmes (MacOS)" 1
	./plot.sh 'WordCount' 'temps' '2,3,6' "Java-/Java* PpFF-4" 1

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
	rm -f graphe*.png *c34581*
