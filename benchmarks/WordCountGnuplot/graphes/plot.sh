#!

#
# ./plot_temps.sh nomDuProgramme (temps|debits) [avec_log [server]]
#
# Utilise aussi le fichier infos-${SERVER}-${PGM}.txt, pour identifier les
# etiquettes a utiliser pour les differentes courbes.
#

# Il est preferable de ne pas lancer l'execution des benchmarks dans
# le script des graphiques, pour permettre de plus facilement refaire
# la presentation des graphes (comme j'ai fait) sans devoir lancer a
# nouveau l'execution!
# 
# C'est pour cette raison que les fichiers sont crees dans le
# sous-repertoire graphes et que les scripts sont places dans ce meme
# repertoire.
#

DEBUG=1


# Pour quel programme on genere les graphes.
PGM="$1"; shift

# La sorte d'items traites par le programme
if [[ $PGM == 'WordCount' ]]; then
    ITEMS="mots"
elif [[ $PGM == 'StockPrice' ]]; then
    ITEMS="records"
else
    ITEMS="items"
fi


# Graphe pour temps d'execution ou pour debits.
SORTE="$1"; shift
if [[ $SORTE == "temps" ]]; then
    TITRE="Temps d'exécution"
    UNITE="ms"
elif [[ $SORTE == "debits" ]]; then
    TITRE="Débit"
    UNITE="K-${ITEMS}/s"
else
    echo "*** Sorte invalide: $sorte"
    exit
fi

# Avec ou sans echelle logarithmique pour les y.
if [[ $# == 0 ]]; then
    AVEC_LOG="1"
else
    AVEC_LOG="$1"
    shift
fi

# Si pas d'argument, on utilise le nom du host courant, qui donne
# acces au fichier de donnees.
if [[ $# == 0 ]]; then
    SERVER="$HOST"
else
    SERVER="$1"
    shift
fi

# Les fichiers d'entree et de sortie.
fichier="${SORTE}-${SERVER}-${PGM}.txt"
avec_sans_log=$([[ $AVEC_LOG == 0 ]] && echo '_nolog')
fichier_graphe="graphe_${SORTE}_${SERVER}_${PGM}${avec_sans_log}.png"

if [[ $DEBUG == 1 ]]; then
    echo "fichier = $fichier"
    echo "fichier_graphe = $fichier_graphe"
fi

# Les parametres pour le graphe.
temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

if [[ $DEBUG ]]; then
    echo "*** Traitement pour $SERVER ***"
    echo "taille_min = $taille_min; taille_max = $taille_max"
    echo "temps_min = $temps_min; temps_max = $temps_max"
fi

avec_sans_log1=$([[ $AVEC_LOG == 1 ]] && echo 'log ')
avec_sans_log2=$([[ $AVEC_LOG == 1 ]] && echo '(log) ')

XTICS="$(head -1 infos-${SERVER}-${PGM}.txt)"

# On genere le script gnuplot.
cat >script.plot <<EOF
set terminal png
set output '$fichier_graphe'
$([[ $AVEC_LOG == 1 ]] && echo "set logscale y")
set format x '%.0f'
set xtics rotate by 310
set xtics font ", 6"
set xtics (${XTICS})

set xlabel "Nombre de ${ITEMS} traités"
set ylabel "${TITRE} (${avec_sans_log1}${UNITE})"
set title "${PGM}: Nombre de ${ITEMS} traités vs. ${avec_sans_log2}${TITRE}\n"
EOF

/bin/echo -n "plot [$taille_min:$taille_max][$temps_min:$temps_max] " >>script.plot


NB_PAR_POINT=3

function ymin_ymax {
    col=$1
    (( min = col + 1 ))
    (( max = col + 2 ))
    echo "1:$col:$min:$max"
}

function line_and_points {
    fichier="$1"
    col="$2"
    title="$3"
    separateur="$4"
    echo "'$fichier' using $(ymin_ymax $col) title '$title' with yerrorlines$separateur"
}


# Note: Il y a un separateur en trop a la fin, mais cela semble quand
# meme fonctionner!
col=2
for item in $(tail -1 infos-${SERVER}-${PGM}.txt); do
    /bin/echo -n $(line_and_points "$fichier" $col $item ", ") >>script.plot
    (( col=col+$NB_PAR_POINT ))
done

if [[ $DEBUG == 1 ]]; then
    echo "*** Script genere ***"
    cat script.plot
fi 

# On genere le graphe.
gnuplot -persist <script.plot

if [[ $DEBUG == 1 ]]; then
    echo "*** Graphe genere! ***"
    if [[ $SERVER == MacOS ]]; then
        open $fichier_graphe
    elif [[ $SERVER == java && $USER == tremblay_gu ]]; then
        cp $fichier_graphe ~/public_html/maison
    fi
fi
