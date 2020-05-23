#
# Appel pour une autre machine que la machine courante ou un autre
# nombre de repetitions
#
# make HOST=autreMachine NB=20 NUM_EXPERIENCE=2

NUM_EXPERIENCE=0
NB=2
EXP_FINALE=5

def: accs_revises

accs_revises:
	./plot.sh 'WordCount' 'accs'  c34581 401 30 '*' _ "Tous les programmes" 1
	./plot.sh 'StockPrice' 'accs'  c34581 401 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WC' 'accs'  c34581 401 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WordCount' 'accs'  c34581 400 30 '*' _ "Tous les programmes" 1
	./plot.sh 'StockPrice' 'accs'  c34581 400 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WC' 'accs'  c34581 41 30 '*' _ "Tous les programmes" 1

accs:
	./plot.sh 'WordCount' 'accs'  java 500 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WordCount' 'accs'  japet 600 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WordCount' 'accs'  c34581 400 30 '*' _ "Tous les programmes" 1
	echo ""
	./plot.sh 'StockPrice' 'accs'  java 700 30 '*' _ "Tous les programmes" 1
	./plot.sh 'StockPrice' 'accs'  japet 700 30 '*' _ "Tous les programmes" 1
	./plot.sh 'StockPrice' 'accs'  c34581 400 30 '*' _ "Tous les programmes" 1
	echo ""
	./plot.sh 'WC' 'accs'  java 5 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WC' 'accs'  japet 6 30 '*' _ "Tous les programmes" 1
	./plot.sh 'WC' 'accs'  c34581 41 30 '*' _ "Tous les programmes" 1

WordCount_preliminaires:
	./plot.sh 'WordCount' 'temps'  $(HOST) 1 10 '*' _   "Programmes Java" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 1 10 '*' log "Programmes Java" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 2 10 '*' _   "Programmes PpFf" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 2 10 '*' log "Programmes PpFf" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 3 10 '*' _   "Programmes FastFlow" 1
	./plot.sh 'WordCount' 'temps'  $(HOST) 3 10 '*' log "Programmes FastFlow" 1

WordCount:
	./plot.sh 'WordCount' 'temps'  $(HOST) 11 20 '*' _   "Tous les programmes"
	./plot.sh 'WordCount' 'debits' $(HOST) 11 20 '*' _   "Tous les programmes"

StockPrice_preliminaires:
	./plot.sh 'StockPrice' 'temps'  $(HOST) 1 10 '*' _   "Programmes Java"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 1 10 '*' log "Programmes Java"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 2 10 '*' _   "Programmes PpFf"
	./plot.sh 'StockPrice' 'temps'  $(HOST) 3 10 '*' _   "Programmes FastFlow"

StockPrice:
	./plot.sh 'StockPrice' 'temps'  $(HOST) 4 30 '*' _   "Tous les programmes"
	./plot.sh 'StockPrice' 'debits' $(HOST) 4 30 '*' _   "Tous les programmes"

WC:
	./plot.sh 'WC' 'temps'  $(HOST) 11 20 '*' _   "Tous les programmes"
	./plot.sh 'WC' 'debits' $(HOST) 11 20 '*' _   "Tous les programmes"


clean cleanxtra:
	rm -f *~
	rm -f script.plot

cleanxxx: cleanxtra
	rm -f *graphe*.png

