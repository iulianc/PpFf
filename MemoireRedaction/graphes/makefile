#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20 NUM_EXPERIENCE=2

NUM_EXPERIENCE=0
NB=2
EXP_FINALE=5

def: test

test:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _ "Tous les programmes"

WordCount_preliminaires:
	./plot.sh 'WordCount' 'temps'  $(HOST) 1 10 '*' _   "Programmes Java" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 1 10 '*' log "Programmes Java" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 2 10 '*' _   "Programmes PpFf" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 2 10 '*' log "Programmes PpFf" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 3 10 '*' _   "Programmes FastFlow" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 3 10 '*' log "Programmes FastFlow" 1

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(EXP_FINALE) 30 '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) $(EXP_FINALE) 30 '*' _   "Tous les programmes"

StockPrice_preliminaires:
	./plot.sh 'StockPrice' 'temps'  $(HOST) 1 10 '*' _   "Programmes Java"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 1 10 '*' log "Programmes Java"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 2 10 '*' _   "Programmes PpFf"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 3 10 '*' _   "Programmes FastFlow"

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) 4 30 '*' _   "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) 4 30 '*' _   "Tous les programmes"


clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

