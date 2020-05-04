#!

# Petit script pour verifier si le fait d'avoir une periode de warm-up
# pour la version WordCount permet de reduire, ou pas, le temps d'execution.
#

##########################################

# MacOS: (30 repetitions) pour 78792
#   *** Execution Java sans Warmup ***
#   427 +/-      9,1 (   402  ..    444 )
#  *** Execution Java avec Warmup ***
#  387 +/-     20,5 (   331  ..    412 )

# MacOS: (30 repetitions) pour 281307
#   *** Execution Java sans Warmup ***
#   741 +/-     42,0 (   661  ..    824 )
#   *** Execution Java avec Warmup ***
#   948 +/-     36,4 (   828  ..   1000 )
#   
#   *** Execution Java sans Warmup ***
#   737 +/-     82,8 (   656  ..   1123 )
#   *** Execution Java avec Warmup ***
#   941 +/-     75,3 (   668  ..   1078 )

##########################################

# Linux: (30 repetitions) pour 78792
#   *** Execution Java sans Warmup ***
#   149 +/-      8.9 (   138  ..    168 )
#   *** Execution Java avec Warmup ***
#   129 +/-     15.9 (   111  ..    178 )

# Linux: (30 repetitions) pour 2137758
#   *** Execution Java sans Warmup ***
#   1167 +/-     37.0 (  1072  ..   1231 )
#   *** Execution Java avec Warmup ***
#   1117 +/-     48.1 (  1025  ..   1215 )


##########################################

# Java: (30 repetitions) pour 2137758
#   *** Execution Java sans Warmup ***
#   3153 +/-    426.4 (  2283  ..   4061 )
#   *** Execution Java avec Warmup ***
#   2563 +/-    768.5 (  1826  ..   5809 )

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


