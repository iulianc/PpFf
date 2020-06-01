#!
if [[ $HOST == 'java' ]]; then
    EXP=3001
elif [[ $HOST == 'japet' ]]; then
    if [[ $(ruby --version) =~ jruby ]]; then
        echo "*** Sur japet, il faut appeler 'rvm use 2.2.1'"
        exit -1
    fi
    EXP=3002
else
    EXP=3003
fi

mk='make -f gt-makefile'

echo "*** Execution sur ${HOST} ***"
for pgm in WordCount StockPrice; do
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
echo "*** Fin execution ${HOST} ***"


