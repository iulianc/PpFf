#!

taille=$1 max=$2
fichier="fichier_temps_$taille.txt"
gnuplot -persist <<EOF
set terminal png
set output 'graphe_acc_MapFilterMap_$taille.png'
set logscale x
set xlabel "Nombre de threads"
set ylabel "Acceleration"
set title "MapFilterMap: Acceleration vs. nb. threads -- $taille elements"
set xtics (1, 2, 4, 8, 16, 32, 64)
plot [0.8:70][0:$max] \
  "$fichier" using 1:(\$2/\$6) title "Seq/PpFf" with linespoints,\
  "$fichier" using 1:(\$3/\$6) title "JavaSeq/PpFf" with linespoints,\
  "$fichier" using 1:(\$4/\$6) title "JavaPar/PpFf" with linespoints,\
  "$fichier" using 1:(\$5/\$6) title "FastFlow/PpFf" with linespoints
EOF
