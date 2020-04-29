#!
nb=$(cat | head -1 | wc -w)
(( nb = 2**(nb-4) ))
echo $nb
