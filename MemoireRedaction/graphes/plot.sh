#!

DEBUG=0

REP_FINAUX="finaux"

#
# Exemples d'appels -- seul le dernier argument est optionnel (generation du LaTeX):
#
#       ./plot.sh 'WordCount' 'temps'  $(HOST) $(NUM_EXPERIENCE $(NB) '*'   log "Tous les programmes" 1
#       ./plot.sh 'WordCount' 'debits' $(HOST) $(NUM_EXPERIENCE) $(NB) '1,4' _   "Programmes 1 et 4"  
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
NUM_EXPERIENCE="$1"; shift
NB_REPETITIONS="$1"; shift
CHAMPS="$1"; shift
AVEC_LOG=$([[ $1 == log ]] && echo "1" || echo "0"); shift
SOUS_TITRE="$1"; shift
AVEC_LATEX=$( [[ $# == 0 ]] && echo "0" || echo "$1"); shift


# On identifie la sorte d'items traites par le programme
if [[ $PGM == 'WordCount' ]]; then
    ITEMS="mots"
elif [[ $PGM == 'StockPrice' ]]; then
    ITEMS="transactions"
else
    ITEMS="items"
fi

# On identifie La mesure et ses unites.
if [[ $SORTE == "temps" ]]; then
    MESURE="Temps d'ex�cution"
    UNITE="ms"
elif [[ $SORTE == "debits" ]]; then
    MESURE="D�bit"
    UNITE="K-${ITEMS}/s"
elif [[ $SORTE == "accs" ]]; then
    MESURE="Acc�l�ration"
    UNITE="absolue"
elif [[ $SORTE == "accsrel" ]]; then
    MESURE="Acc�l�ration"
    UNITE="relative"
else
    echo "*** Sorte invalide: $sorte"
    exit
fi

# On determine le numero de la machine
if [[ $MACHINE == "java" ]]; then
    NUM_MACHINE="M1"
elif [[ $MACHINE == "japet" ]]; then
    NUM_MACHINE="M2"
elif [[ $MACHINE == "c34581" ]]; then
    NUM_MACHINE="M3"
else
    NUM_MACHINE="M4"
fi
    
########################################################################
# On specificie les noms des fichiers de donnees et de sortie.
########################################################################
suffixe_champs=$( [[ $CHAMPS == '*' ]] && echo '' ||  echo "-$(echo ${CHAMPS} | sed 's/,//g')")
sous_rep_finaux=$( [[ AVEC_LATEX != 0 ]] && echo "${REP_FINAUX}/" || echo "")

fichier_infos="resultats/${PGM}-infos-${MACHINE}-${NUM_EXPERIENCE}-${NB_REPETITIONS}.txt"
fichier_donnees="resultats/${PGM}-${SORTE}-${MACHINE}-${NUM_EXPERIENCE}-${NB_REPETITIONS}.txt"
fichier_infos_finaux="resultats/${REP_FINAUX}/${PGM}-infos-${MACHINE}-${NUM_EXPERIENCE}-${NB_REPETITIONS}.txt"
fichier_donnees_finaux="resultats/${REP_FINAUX}/${PGM}-${SORTE}-${MACHINE}-${NUM_EXPERIENCE}-${NB_REPETITIONS}.txt"

if [[ ${AVEC_LATEX} != 0 ]]; then
    if [[ ! -f ${fichier_infos_finaux} ]] || [[ ! -f ${fichier_donnees_finaux} ]]; then
        read -p "On copie les fichiers de donnees (${fichier_infos} et al.) dans finaux [o/N]?" on_copie
        if [[ ${on_copie} == o || ${on_copie} == O ]]; then
            cp -f ${fichier_infos}  ${fichier_infos_finaux} 
            cp -f ${fichier_donnees}  ${fichier_donnees_finaux}
        fi
    fi
    fichier_infos=${fichier_infos_finaux}
    fichier_donnees=${fichier_donnees_finaux}
fi

avec_sans_log=$( [[ $AVEC_LOG == 1 ]] && echo '-log' )

here=$( [[ $AVEC_LATEX == 2 ]] && echo '-here' || echo '' )

fichier_graphe="${PGM}${avec_sans_log}-${SORTE}-${MACHINE}-${NUM_EXPERIENCE}-${NB_REPETITIONS}${suffixe_champs}${here}.png"

if [[ $DEBUG == 1 ]]; then
    echo "*** fichier_infos = ${fichier_infos}"
    echo "*** fichier_donnees = ${fichier_donnees}"
    echo "*** fichier_graphe = ${fichier_graphe}"
fi

########################################################################
# On definit le script pour la generation du graphe.
########################################################################
if [[ $SORTE == "accs" || $SORTE = "accsrel" ]]; then
    temps_min="0.0"
else
    temps_min=$(ruby min_temps.rb <$fichier_donnees)
fi
temps_max=$(ruby max_temps.rb <$fichier_donnees)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier_donnees)

if [[ $DEBUG == 1 ]]; then
    echo "*** Traitement pour $MACHINE ***"
    echo "taille_min = $taille_min; taille_max = $taille_max"
    echo "temps_min = $temps_min; temps_max = $temps_max"
fi

avec_sans_log1=$([[ $AVEC_LOG == 1 ]] && echo 'log ')
avec_sans_log2=$([[ $AVEC_LOG == 1 ]] && echo '(log) ')

XTICS="$(head -1 ${fichier_infos})"

if [[ $AVEC_LATEX != 2 ]]; then
    TITLE="${PGM} -- ${SOUS_TITRE}\nMachine ${NUM_MACHINE}\nNombre de ${ITEMS} trait�s vs. ${avec_sans_log2}${MESURE}\n"
    #XLABEL="set xlabel 'Nombre de ${ITEMS} trait�s'"
    XLABEL="'Nombre de ${ITEMS} trait�s'"
    OFFSET=''
else
    TITLE="Machine ${NUM_MACHINE}"
    XLABEL=''
    OFFSET='offset 0,-3'
fi

# On genere le script gnuplot.
cat >script.plot <<EOF
set terminal png
set output '$fichier_graphe'
$([[ $AVEC_LOG == 1 ]] && echo "set logscale y")
set format x '%.0f'
set xtics rotate by 310
set xtics font ", 6"
set xtics (${XTICS})
set xlabel ${XLABEL}
set ylabel "${MESURE} (${avec_sans_log1}${UNITE})"
set title "${TITLE}" ${OFFSET}
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
if [[ $DEBUG == 2 ]]; then
    echo "les_labels = $les_labels"
    echo "les_labels_selectionnes = $les_labels_selectionnes"
fi
for item in $les_labels; do
    item_="$(echo $item | sed 's/*/\\*/')"
    [[ $DEBUG == 2 ]] && echo "item_ = $item_"
    if [[ $(echo "$les_labels_selectionnes" | grep "$item_" ) ]]; then
        [[ $DEBUG == 2 ]] && echo "col = $col; item = $item"
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

if [[ $DEBUG == 2 ]]; then
    echo "*** Graphe genere: ${fichier_graphe} -- on appelle open ***"
    if [[ $HOST == MacOS ]]; then
        open ${fichier_graphe}
    elif [[ $HOST == c34581 ]]; then
        gio open ${fichier_graphe}
    elif [[ $HOST == java && $USER == tremblay_gu ]]; then
        cp ${fichier_graphe} ~/public_html/maison
    fi
else
    rm -f script.plot
fi

########################################################################
# On genere les fichiers auxiliaires latex, si demande.
########################################################################

if [[ $AVEC_LATEX != 0 ]]; then
    cp -f ${fichier_graphe} ${REP_FINAUX}
fi
if [[ $AVEC_LATEX == 1 ]]; then
    ruby ./gen-latex.rb "${PGM}" "${SORTE}" "${MACHINE}" "${NUM_EXPERIENCE}" "${NB_REPETITIONS}" "${suffixe_champs}" "${SOUS_TITRE}" "${avec_sans_log}"
fi
