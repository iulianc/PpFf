#!

# Petit script pour verifier si le fait d'avoir une periode de warm-up
# pour la version WordCount permet de reduire, ou pas, le temps d'execution.
#
# MacOS: oui (30 repetitions)
#        sans => 429 ( 403 .. 530 )
#        avec => 389 ( 342 .. 442 )

fichier="$1"; shift
avec_sans_warmup="$1"; shift
nb_fois="$1"; shift

(( tot = 0 ))
(( min =  999999 ))
(( max = 0 ))
for (( i = 0; i < $nb_fois; i = i + 1 )); do
    temps=$(java -cp . WordCountWarmup ${fichier} $avec_sans_warmup)
    echo $temps
    (( tot = tot + temps ))
    if [[ $temps -lt $min ]]; then
        min=$temps
    fi
    if [[ $temps -gt $max ]]; then
        max=$temps
    fi
done
(( moy = tot / nb_fois ))

echo "$moy ($min .. $max)"

