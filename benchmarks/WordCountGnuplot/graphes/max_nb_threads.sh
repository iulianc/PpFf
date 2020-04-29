#!
nbcols=$(cat | head -1 | wc -w)
(( nbmoys = (nbcols - 1) / 2 ))
(( nbt = 2**(nbmoys-3) ))
echo $nbt
