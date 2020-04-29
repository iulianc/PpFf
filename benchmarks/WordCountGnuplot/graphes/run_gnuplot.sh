#To execute script
#chmod +x run_gnuplot.sh
#./run_gnuplot.sh nom_du_server

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
server="$1"

fichier="temps-${server}-wc.txt"
output_graph="graphe_temps_${server}_WordCount.png"

if [[ $DEBUG == 1 ]]; then
  echo "fichier = $fichier"
  echo "output_graph = $output_graph"
fi

temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

if [[ $DEBUG ]]; then
    echo "*** Traitement pour $server ***"
    echo "taille_min = $taille_min; taille_max = $taille_max"
    echo "temps_min = $temps_min; temps_max = $temps_max"
fi

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
/bin/echo -n "\"$fichier\" using 1:2 title 'Java+' with linespoints, " >>script.plot
/bin/echo -n "\"$fichier\" using 1:3 title 'Java-' with linespoints, " >>script.plot
/bin/echo -n "\"$fichier\" using 1:4 title 'PpFf-1-' with linespoints, " >>script.plot
/bin/echo -n "\"$fichier\" using 1:5 title 'PpFf-2' with linespoints" >>script.plot


if [[ $server == 'MacOS' || $server == 'java' ]]; then
    /bin/echo "" >>script.plot

elif [[ $server == 'japet' ]]; then
    /bin/echo -n ", \"$fichier\" using 1:6 title 'PpFf-4' with linespoints, " >>script.plot
    /bin/echo -n "\"$fichier\" using 1:7 title 'PpFf-8' with linespoints, " >>script.plot
    /bin/echo "\"$fichier\" using 1:8 title 'PpFf-16' with linespoints" >>script.plot

else
    /bin/echo ", \"$fichier\" using 1:6 title 'PpFf-4' with linespoints" >>script.plot

fi

if [[ $DEBUG == 1 ]]; then
    echo "*** Script genere ***"
    cat script.plot
fi 

gnuplot -persist <script.plot

if [[ $DEBUG == 1 ]]; then
    echo "*** Graphe genere! ***"
    #open $output_graph
fi
