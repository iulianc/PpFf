#!
if [[ $HOST == 'java' ]]; then
    EXP=1001
elif [[ $HOST == 'japet' ]]; then
    EXP=1002
else
    EXP=1003
fi

mk='make -f gt-makefile'

for pgm in WordCount StockPrice WC; do
#for pgm in StockPrice WC; do
    echo "*** ${pgm} ***"
    date

    if [[ $pgm == WordCount ]]; then
        pushd WordCountGnuplot
    elif [[ $pgm == StockPrice ]]; then
        pushd StockPrice/StockPrice_PpFf_Gnuplot
    else
        pushd $pgm
    fi
    ${mk} clean
    ${mk} compile
    ./run_bms.rb ${pgm} ${EXP}
    
    popd
    pwd
    ls
    date
    echo "---------------------"
    echo ""
done


