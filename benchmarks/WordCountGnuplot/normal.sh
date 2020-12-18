#!

prog=$1
nb_mots=$2
nb=$3
fich="fich_res.txt"

\rm -f $fich
touch $fich

for ((i = 0; i < $nb; i++)); do
    r=$(./$prog testdata/${nb_mots}Words.txt)
    echo $r
    echo $r >> $fich
done

echo "------"
cat $fich
