#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20 NUM_EXPERIENCE=2

NUM_EXPERIENCE=0
NB=2

def: WordCount

test:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*'   log "Tous les programmes" 1
	./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*'   log "Tous les programmes" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '1,4' _   "Programmes Java+ et PpFf-1" 1
	./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '1,4' _   "Programmes Java+ et PpFf-1" 1

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	./plot.sh 'StockPrice' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _    "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"

WordCountBash:
	sh ./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	sh ./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"
	sh ./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	sh ./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"

StockPriceBash:
	sh ./plot.sh 'StockPrice' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _   "Tous les programmes"
	sh ./plot.sh 'StockPrice' 'temps'  $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"
	sh ./plot.sh 'StockPrice' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' _    "Tous les programmes"
	sh ./plot.sh 'StockPrice' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '*' log "Tous les programmes"


clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

