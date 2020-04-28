#To execute script
#chmod +x run_gnuplot.sh
#./run_gnuplot.sh

# Il est preferable de ne pas lancer l'execution des benchmarks dans
# le script des graphiques, pour permettre de plus facilement refaire
# la presentation des graphes (comme j'ai fait) sans devoir lancer a
# nouveau l'execution!
#
# ruby run_bms.rb

server="java"	#"japet"
fichier="test.txt"
output_graph="graph.png"

if [ $server == "java" ]
then
    fichier="temps-java-maps.txt"
    output_graph='graphe_temps_Java_Maps.png'
else
    fichier="temps-japet-maps.txt"	
    output_graph='graphe_temps_Japet_Maps.png'
fi


temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

gnuplot -persist <<EOF
set terminal png
set output '$output_graph'
set logscale y
set xlabel "Nombre de threads"
set ylabel "Temps d'exécution (log ms)"
set title "Maps: Nombre de threads vs. (log) Temps d'exécution\n"
plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:3 title "PpFf-5" with linespoints,\
	 "$fichier" using 1:5 title "FastFlow-5" with linespoints,\
	 "$fichier" using 1:2 title "PpFf-4" with linespoints,\
	 "$fichier" using 1:4 title "FastFlow-4:" with linespoints
EOF

