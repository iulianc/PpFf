#!
if [[ $HOST == 'java' ]]; then
    EXP=1001
elif [[ $HOST == 'japet' ]]; then
    EXP=1002
else
    EXP=1003
fi

mk='make -f gt-makefile'

if [[ $host == japet ]]; then
    rvm use ruby-2.2.1
fi

for pgm in WordCount StockPrice WC; do
#for pgm in StockPrice WC; do
    echo "*** ${pgm} ***"
    date

    if [[ $pgm == WordCount ]]; then
        cd WordCountGnuplot
    elif [[ $pgm == StockPrice ]]; then
        cd StockPrice/StockPrice_Ppff_Gnuplot
    else
        cd $pgm
    fi
    ${mk} compile
    ./run_bms.rb ${pgm} ${EXP}
    
    if [[ $pgm == StockPrice ]]; then
        cd ../..
    else
        cd ..
       fi
    date
    echo "---------------------"
    echo ""
done


