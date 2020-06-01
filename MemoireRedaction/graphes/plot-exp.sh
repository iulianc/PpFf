#!

if [[ $# == 0 ]]; then
    echo "usage:"
    echo "  $0 PGM EXPR NB"
    exit -1
fi

PGM=$1; shift
EXP=$1; shift
NB=$1; shift

TITRE="Tous les programmes"

./plot.sh $PGM temps  $HOST $EXP $NB '*' _   "$TITRE"
./plot.sh $PGM debits $HOST $EXP $NB '*' _   "$TITRE"

if [[ $HOST == 'java' || $HOST == 'japet' ]]; then
    cp "$PGM-temps-$HOST-$EXP-$NB.png" ~/public_html/maison
    cp "$PGM-debits-$HOST-$EXP-$NB.png" ~/public_html/maison
fi

