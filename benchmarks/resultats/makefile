HOST=java
EXP=1001
def: accs_rel

#HOST=japet
#EXP=1002

#HOST=c34581
#EXP=1003

accs_rel:
	./gen-accsrel.rb WordCount-temps-japet-3002-40.txt

ic:
	./ch-ic.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb WC-temps-$(HOST)-$(EXP)-40.txt
	#
	./ch-ic.rb WordCount-debits-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb StockPrice-debits-$(HOST)-$(EXP)-40.txt
	./ch-ic.rb WC-debits-$(HOST)-$(EXP)-40.txt


accs:
	./gen-accs.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./gen-accs.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./gen-accs.rb WC-temps-$(HOST)-$(EXP)-40.txt
	#
	./gen-accs.rb WordCount-temps-$(HOST)-$(EXP)-40.txt
	./gen-accs.rb StockPrice-temps-$(HOST)-$(EXP)-40.txt
	./gen-accs.rb WC-temps-$(HOST)-$(EXP)-40.txt
