#!

fichier="temps-wc.txt"
temps_max=$(ruby max_temps.rb <$fichier)
taille_max=$(ruby max_taille.rb <$fichier)
gnuplot -persist <<EOF
set terminal png
set output 'graphe_temps_WordCount.png'
set logscale x
set logscale y
set xlabel "Nombre de mots (log)"
set ylabel "Temps d'exécution (log ms)"
set title "WordCount: Nombre de mots traités vs. Temps d'exécution\n(diagramme log-log)"
plot [0.8:$taille_max][0.1:$temps_max] \
	 "$fichier" using 1:2 title "Java+" with linespoints,\
	 "$fichier" using 1:3 title "Java-" with linespoints,\
	 "$fichier" using 1:4 title "PpFf-1:" with linespoints,\
	 "$fichier" using 1:5 title "PpFf-2" with linespoints
EOF

