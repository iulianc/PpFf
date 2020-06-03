#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20 NUM_EXPERIENCE=2

def: graphe_h

graphe_h:
	./plot.sh 'WordCount' 'temps' java 2221 20 '2,3,4,5' _  "flatMap vs. flatMap_"

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) 11 20 '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) 11 20 '*' _   "Tous les programmes"

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) 2003 40 '*' _   "Tous les programmes"
	./plot.sh 'StockPrice' 'debits'  $(HOST) 2003 40 '*' _  "Tous les programmes"

WC:
	./plot.sh 'WC' 'temps'  $(HOST) $(EXP) $(NB) '*' _   "Tous les programmes"


clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

