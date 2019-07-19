#!

# Pourrait etre passe en argument plutot que hard-code.
fichier="temps-wc.txt"

temps_min=0 #$(ruby min_temps.rb <$fichier)
temps_max=1300000 #$(ruby max_temps.rb <$fichier)

taille_min=0.1 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)


gnuplot -persist <<EOF
set format x "%2.0E"
set format y "%3.1E"
set terminal png
set output 'graphe_debit_WordCount.png'
#set logscale x
#set logscale y
set xlabel "Nombre de mots"
set ylabel "Débit (mots/sec)"
set title "WordCount: Nombre de mots traités vs. Débit"
plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:(1000*\$1/\$2) title "Java+" with linespoints,\
	 "$fichier" using 1:(1000*\$1/\$3) title "Java-" with linespoints,\
	 "$fichier" using 1:(1000*\$1/\$4) title "PpFf-1:" with linespoints,\
	 "$fichier" using 1:(1000*\$1/\$5) title "PpFf-2" with linespoints
EOF

