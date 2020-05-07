#!

DEBUG=0

REP_FINAUX="finaux"

#
# Exemples d'appels -- seul le dernier argument est optionnel (generation du LaTeX):
#
#       ./plot.sh 'WordCount' 'temps'  $(HOST) $(NB) '*'   "Tous les programmes" log 1
#       ./plot.sh 'WordCount' 'debits' $(HOST) $(NB) '1,4' "Programmes 1 et 4"   _
#
# Parametre champs:
#   '*'     => toutes les donnees
#   '1,3,4' => juste les donnees des colonnes 1, 3 et 4
#
# Note: La premiere colonne = 1!
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

########################################################################
# On obtient les divers arguments
########################################################################
PGM="$1"; shift
SORTE="$1"; shift
MACHINE="$1"; shift
NB_REPETITIONS="$1"; shift
CHAMPS="$1"; shift
SOUS_TITRE="$1"; shift
AVEC_LOG=$([[ $1 == log ]] && echo "1" || echo "0"); shift
AVEC_LATEX=$( [[ $# == 0 ]] && echo "0" || echo "1"); shift


# On identifie la sorte d'items traites par le programme
if [[ $PGM == 'WordCount' ]]; then
    ITEMS="mots"
elif [[ $PGM == 'StockPrice' ]]; then
    ITEMS="records"
else
    ITEMS="items"
fi

# On identifie La mesure et ses unites.
if [[ $SORTE == "temps" ]]; then
    MESURE="Temps d'exécution"
    UNITE="ms"
elif [[ $SORTE == "debits" ]]; then
    MESURE="Débit"
    UNITE="K-${ITEMS}/s"
else
    echo "*** Sorte invalide: $sorte"
    exit
fi

########################################################################
# On specificie les noms des fichiers de donnees et de sortie.
########################################################################
suffixe_champs=$( [[ $CHAMPS == '*' ]] && echo '' ||  echo "-$(echo ${CHAMPS} | sed 's/,//g')")
sous_rep_finaux=$( [[ AVEC_LATEX == 1 ]] && echo "${REP_FINAUX}/" || echo "")

fichier_infos="resultats/${PGM}-infos-${MACHINE}-${NB_REPETITIONS}.txt"
fichier_donnees="resultats/${PGM}-${SORTE}-${MACHINE}-${NB_REPETITIONS}.txt"
fichier_infos_finaux="resultats/${REP_FINAUX}/${PGM}-infos-${MACHINE}-${NB_REPETITIONS}.txt"
fichier_donnees_finaux="resultats/${REP_FINAUX}/${PGM}-${SORTE}-${MACHINE}-${NB_REPETITIONS}.txt"

if [[ ${AVEC_LATEX} == 1 ]]; then
    if [[ ! -f ${fichier_infos_finaux} ]] || [[ ! -f ${fichier_donnees_finaux} ]]; then
        read -p "On copie les fichiers de donnees dans finaux [o/N]?" on_copie
        if [[ ${on_copie} == o || ${on_copie} == O ]]; then
            cp -f ${fichier_infos}  ${fichier_infos_finaux} 
            cp -f ${fichier_donnees}  ${fichier_donnees_finaux}
        fi
    fi
    fichier_infos=${fichier_infos_finaux}
    fichier_donnees=${fichier_donnees_finaux}
fi

avec_sans_log=$([[ $AVEC_LOG == 1 ]] && echo '-log')

fichier_graphe="${PGM}-graphe${avec_sans_log}-${SORTE}-${MACHINE}-${NB_REPETITIONS}${suffixe_champs}.png"


########################################################################
# On definit le script pour la generation du graphe.
########################################################################
temps_min=$(ruby min_temps.rb <$fichier_donnees)
temps_max=$(ruby max_temps.rb <$fichier_donnees)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier_donnees)

if [[ $DEBUG ]]; then
    echo "*** Traitement pour $MACHINE ***"
    echo "taille_min = $taille_min; taille_max = $taille_max"
    echo "temps_min = $temps_min; temps_max = $temps_max"
fi

avec_sans_log1=$([[ $AVEC_LOG == 1 ]] && echo 'log ')
avec_sans_log2=$([[ $AVEC_LOG == 1 ]] && echo '(log) ')

XTICS="$(head -1 ${fichier_infos})"

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
set ylabel "${MESURE} (${avec_sans_log1}${UNITE})"
set title "${PGM} -- ${SOUS_TITRE}\nNombre de ${ITEMS} traités vs. ${avec_sans_log2}${MESURE}\n"
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

les_labels=$(tail -1 ${fichier_infos})
if [[ $CHAMPS == '*' ]]; then
    les_labels_selectionnes="$les_labels"
else
    les_labels_selectionnes=$(echo "$les_labels" | cut -f "$CHAMPS" -d ' ')
fi
col=2
for item in $les_labels; do
    item_="$(echo $item | sed 's/*/\\*/')"
    if [[ $(echo "$les_labels_selectionnes" | grep "$item_" ) ]]; then
        /bin/echo -n $(line_and_points "$fichier_donnees" $col $item ", ") >>script.plot
    fi
    (( col=col+$NB_PAR_POINT ))
done

if [[ $DEBUG == 2 ]]; then
    echo "*** Script genere ***"
    cat script.plot
fi 

# On genere le graphe.
gnuplot -persist <script.plot

if [[ $DEBUG == 1 ]]; then
    echo "*** Graphe genere: ${fichier_graphe} -- on appelle open ***"
    if [[ $HOST == MacOS ]]; then
        open ${fichier_graphe}
    elif [[ $HOST == c34581 ]]; then
        gio open ${fichier_graphe}
    elif [[ $HOST == java && $USER == tremblay_gu ]]; then
        cp ${fichier_graphe} ~/public_html/maison
    fi
fi

########################################################################
# On genere les fichiers auxiliaires latex, si demande.
########################################################################

if [[ $AVEC_LATEX == 1 ]]; then
    cp -f ${fichier_graphe} ${REP_FINAUX}
    ./gen-latex.rb "${PGM}" "${SORTE}" "${MACHINE}" "${NB_REPETITIONS}" "${suffixe_champs}" "${SOUS_TITRE}" "${avec_sans_log}"
fi
