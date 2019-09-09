#To execute script
#chmod +x run_gnuplot.sh
#./run_gnuplot.sh

ruby run_bms.rb

fichier="temps-maps.txt"

temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

gnuplot -persist <<EOF
set terminal png
set output 'graphe_temps_Maps.png'
set logscale y
set xlabel "Nombre de threads"
set ylabel "Temps d'exécution (log ms)"
set title "Maps: Nombre de threads vs. (log) Temps d'exécution\n"
plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:2 title "PpFf-4" with linespoints,\
	 "$fichier" using 1:3 title "PpFf-5" with linespoints,\
	 "$fichier" using 1:4 title "FastFlow-4:" with linespoints,\
	 "$fichier" using 1:5 title "FastFlow-5" with linespoints
EOF

