#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20

NB=2

def: test

test:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*'   log "Tous les programmes" 1
	./plot.sh 'WordCount' 'debits'  $(HOST) $(NB) '*'   log "Tous les programmes" 1
	./plot.sh 'WordCount' 'temps' $(HOST) $(NB) '1,4' _   "Programmes Java+ et PpFf-1"   1
	./plot.sh 'WordCount' 'debits' $(HOST) $(NB) '1,4' _   "Programmes Java+ et PpFf-1"   1

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*' log "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOTS) $(NB) '*' log "Tous les programmes"

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NB) '*' log "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NB) '*' log "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NB) '*' log "Tous les programmes"

clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

