#!

taille=$1 max=$2
fichier="fichier_temps_$taille.txt"
gnuplot -persist <<EOF
set terminal png
set output 'graphe_acc_$taille.png'
set logscale x
set xlabel "Nombre de threads"
set ylabel "Acceleration"
set title "ReduceByKey: Acceleration vs. nb. threads -- $taille elements"
set ytics (1, 2, 3, 4, 5, 6, 7, 8)
set xtics (1, 2, 4, 8, 16, 32, 64)
plot [0.8:70][0:$max] \
  "$fichier" using 1:(\$2/\$5) title "Seq/PpFf" with linespoints,\
  "$fichier" using 1:(\$3/\$5) title "JavaSeq/PpFf" with linespoints,\
  "$fichier" using 1:(\$4/\$5) title "JavaPar/PpFf" with linespoints
EOF
