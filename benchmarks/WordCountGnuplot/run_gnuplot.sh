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
   fichier="temps-java-wc.txt"
   output_graph='graphe_temps_Java_WordCount.png'
else
   fichier="temps-japet-wc.txt"
   output_graph='graphe_temps_Japet_WordCount.png'
fi

temps_min=$(ruby min_temps.rb <$fichier)
temps_max=$(ruby max_temps.rb <$fichier)

taille_min=0 # 0 plus simple pcq. non log
taille_max=$(ruby max_taille.rb <$fichier)

if [ $server == "java" ]
then
    gnuplot -persist <<EOF
    set terminal png
    set output '$output_graph'
    set logscale y
    # Petit essai: a voir si c'est preferable une fois dans le memoire?
    set format x '%.0f'
    set xtics rotate by 310
    set xtics font ", 6"
    set xtics (78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743)
    #
    set xlabel "Nombre de mots trait�s"
    set ylabel "Temps d'ex�cution (log ms)"
    set title "WordCount: Nombre de mots trait�s vs. (log) Temps d'ex�cution\n"

    plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:3 title "Java-" with linespoints,\
	 "$fichier" using 1:5 title "PpFf-2" with linespoints,\
	 "$fichier" using 1:4 title "PpFf-1:" with linespoints,\
	 "$fichier" using 1:2 title "Java+" with linespoints
EOF

else
    gnuplot -persist <<EOF
    set terminal png
    set output '$output_graph'
    set logscale y
    # Petit essai: a voir si c'est preferable une fois dans le memoire?
    set format x '%.0f'
    set xtics rotate by 310
    set xtics font ", 6"
    set xtics (78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743)
    #
    set xlabel "Nombre de mots trait�s"
    set ylabel "Temps d'ex�cution (log ms)"
    set title "WordCount: Nombre de mots trait�s vs. (log) Temps d'ex�cution\n"

    plot [$taille_min:$taille_max][$temps_min:$temps_max] \
	 "$fichier" using 1:2 title "Java+" with linespoints,\
	 "$fichier" using 1:4 title "PpFf-1:" with linespoints,\
	 "$fichier" using 1:5 title "PpFf-2" with linespoints,\
	 "$fichier" using 1:6 title "PpFf-4" with linespoints,\
	 "$fichier" using 1:7 title "PpFf-8" with linespoints,\
	 "$fichier" using 1:8 title "PpFf-16" with linespoints
EOF

fi

