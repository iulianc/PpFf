#!

# Pour executer le script:
# chmod +x run_gnuplot.sh # 1 seule fois!
# ./run_gnuplot.sh [server]

# Il est preferable de ne pas lancer l'execution des benchmarks dans
# le script des graphiques, pour permettre de plus facilement refaire
# la presentation des graphes (comme j'ai fait) sans devoir lancer a
# nouveau l'execution!
# 
# C'est pour cette raison que le fichier est cree dans le
# sous-repertoire graphes et que les scripts sont places dans ce meme
# repertoire.
#

DEBUG=1


# Si pas d'argument, on utilise le nom du host courant, qui donne
# acces au fichier de donnees.
if [[ $# == 0 ]]; then
    server="$HOST"
else
    server="$1"
fi

# Les fichiers d'entree et de sortie.
fichier="temps-${server}-wc.txt"
output_graph="graphe_temps_${server}_WordCount.png"

if [[ $DEBUG == 1 ]]; then
  echo "fichier = $fichier"
  echo "output_graph = $output_graph"
fi

# Les parametres pour le graphe.
temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

if [[ $DEBUG ]]; then
    echo "*** Traitement pour $server ***"
    echo "taille_min = $taille_min; taille_max = $taille_max"
    echo "temps_min = $temps_min; temps_max = $temps_max"
fi

# On genere le script gnuplot.
cat >script.plot <<EOF
set terminal png
set output '$output_graph'
set logscale y
set format x '%.0f'
set xtics rotate by 310
set xtics font ", 6"
set xtics (78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743)

set xlabel "Nombre de mots traités"
set ylabel "Temps d'exécution (log ms)"
set title "WordCount: Nombre de mots traités vs. (log) Temps d'exécution\n"
EOF

/bin/echo -n "plot [$taille_min:$taille_max][$temps_min:$temps_max] " >>script.plot

col=0
for item in 'Java+' 'Java-' 'PpFf-1'; do
  (( col=col+2 ))
  /bin/echo -n "'$fichier' using 1:$col title '$item' with linespoints, " >>script.plot
done

max_nb_threads=$(./max_nb_threads.sh <$fichier)
for (( i = 2; i < max_nb_threads; i = 2*i )); do
    (( col=col+2 ))
    /bin/echo -n "'$fichier' using 1:$col title 'PpFf-$i' with linespoints, " >>script.plot
done

(( col=col+2 ))
/bin/echo "'$fichier' using 1:$col title 'PpFf-$max_nb_threads' with linespoints" >>script.plot

if [[ $DEBUG == 1 ]]; then
    echo "*** Script genere ***"
    cat script.plot
fi 

# On trace le graphe.

gnuplot -persist <script.plot

if [[ $DEBUG == 1 ]]; then
    echo "*** Graphe genere! ***"
    if [[ $server == MacOS ]]; then
        open $output_graph
    elif [[ $server == java && $USER == tremblay_gu ]]; then
        cp $output_graph ~/public_html/maison
    fi
fi
