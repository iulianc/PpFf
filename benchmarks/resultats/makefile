HOST=java
EXP=1001

#HOST=japet
#EXP=1002

#HOST=c34581
#EXP=1003

ic:
	./ch-ic.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb WC-temps-$(HOST)-$(EXP)-40.txt
	#
	./ch-ic.rb WordCount-debits-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb StockPrice-debits-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb WC-debits-$(HOST)-$(EXP)-40.txt


accs:
	./gen-acc.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./gen-acc.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./gen-acc.rb WC-temps-$(HOST)-$(EXP)-40.txt
	#
	./gen-acc.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./gen-acc.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./gen-acc.rb WC-temps-$(HOST)-$(EXP)-40.txt
