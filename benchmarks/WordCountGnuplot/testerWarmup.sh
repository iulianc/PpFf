#!

# Petit script pour verifier si le fait d'avoir une periode de warm-up
# pour la version WordCount permet de reduire, ou pas, le temps d'execution.
#
# MacOS: oui (30 repetitions)
#        sans => 429 ( 403 .. 530 )
#        avec => 389 ( 342 .. 442 )


# Linux: oui (30 repetitions) pour 78792
#   *** Execution Java sans Warmup ***
#   149 +/-      8.9 (   138  ..    168 )
#   *** Execution Java avec Warmup ***
#   129 +/-     15.9 (   111  ..    178 )

# Linux: oui (30 repetitions) pour 2137758
#   *** Execution Java sans Warmup ***
#   1167 +/-     37.0 (  1072  ..   1231 )
#   *** Execution Java avec Warmup ***
#   1117 +/-     48.1 (  1025  ..   1215 )


fichier="$1"; shift
avec_sans_warmup="$1"; shift
nb_fois="$1"; shift

(( tot = 0 ))
(( min =  99999999 ))
(( max = 0 ))
les_temps=()
for (( i = 0; i < $nb_fois; i += 1 )); do
    temps=$(java -cp . WordCountWarmup $fichier $avec_sans_warmup)
    (( tot = tot + temps ))
    les_temps+=( $temps )
    if [[ $temps -lt $min ]]; then
        min=$temps
    fi
    if [[ $temps -gt $max ]]; then
        max=$temps
    fi
done
(( moy = tot / nb_fois ))

sum_std=0
for temps in ${les_temps[@]}; do
    (( sum_std += (temps - moy) * (temps - moy) ))
done
(( sum_std /= (nb_fois - 1) ))

std=$(echo "$sum_std" | awk '{printf "%8.1f\n", sqrt($1)}')

echo "$moy +/- $std ($min .. $max)"


