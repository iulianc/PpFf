#!

taille=$1
fichier="fichier_temps_$taille.txt"
max=$(ruby temps_max.rb <$fichier)
gnuplot -persist <<EOF
set terminal png
set output 'graphe_temps_$taille.png'
set logscale x
set xlabel "Nombre de threads"
set ylabel "Temps d'execution (ms)"
set title "ReduceByKey: Temps vs. nb. threads -- $taille elements"
set xtics (1, 2, 4, 8, 16, 32, 64)
plot [0.8:70][0:$max] \
	 "$fichier" using 1:2 title "Seq" with linespoints,\
	 "$fichier" using 1:3 title "JavaSeq" with linespoints,\
	 "$fichier" using 1:4 title "JavaPar" with linespoints,\
	 "$fichier" using 1:5 title "FfPp" with linespoints
EOF

