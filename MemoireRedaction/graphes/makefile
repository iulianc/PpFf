#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20

NB=2

def: test

test:
#	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*'   "Tous les programmes" log
	./plot.sh 'WordCount' 'debits' $(HOST) $(NB) '1,4' "Programmes 1 et 4"   _ 1

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*' "Tous les programmes" _
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*' "Tous les programmes" log
	./plot.sh 'WordCount' 'debits' $(HOST) $(NB) '*' "Tous les programmes" _
	./plot.sh 'WordCount' 'debits' $(HOTS) $(NB) '*' "Tous les programmes" log

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NB) '*' "Tous les programmes"
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NB) '*' "Tous les programmes" log
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NB) '*' "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NB) '*' "Tous les programmes" log

clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

