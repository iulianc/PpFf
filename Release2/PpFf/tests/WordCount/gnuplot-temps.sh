#!

# Pourrait etre passe en argument plutot que hard-code.
fichier="temps-wc.txt"

temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

gnuplot -persist <<EOF
set terminal png
set output 'graphe_temps_WordCount.png'
set logscale y
set xlabel "Nombre de mots"
set ylabel "Temps d'exécution (log ms)"
set title "WordCount: Nombre de mots traités vs. (log) Temps d'exécution\n"
plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:2 title "Java+" with linespoints,\
	 "$fichier" using 1:3 title "Java-" with linespoints,\
	 "$fichier" using 1:4 title "PpFf-1:" with linespoints,\
	 "$fichier" using 1:5 title "PpFf-2" with linespoints
EOF

