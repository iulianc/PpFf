#
# Appel pour une autre machine que la machine courante:
#
# make SERVER=foo

WordCount:
	./plot.sh "WordCount" "temps" 0
	./plot.sh "WordCount" "temps" 1
	./plot.sh "WordCount" "debits" 0
	./plot.sh "WordCount" "debits" 1

StockPrice:
	./plot.sh "StockPrice" "temps" 0
	./plot.sh "StockPrice" "temps" 1
	./plot.sh "StockPrice" "debits" 0
	./plot.sh "StockPrice" "debits" 1

clean cleanxtra:
	rm -f *~
	rm -f script.plot
