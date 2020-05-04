if DEBUG
  NB_REPETITIONS = 2
  NB_ITEMS = [4 * 1024, 8 * 1024, 16 * 1024]
else
  NB_REPETITIONS = 10
  NB_ITEMS = [4 * 1024, 8 * 1024, 16 * 1024]
end


######################################################
# IMPORTANT Ce sont les elements PGMS et FICHIERS_DONNEES qu'il faut
# modifier si on veut ajouter d'autres programmes a benchmarker et/ou
# d'autres fichiers a traiter.
######################################################

# Les programmes a executer.
pgms_java =
  [ ["java -cp . #{PGM}", 'Java+'],
    ["java -Djava.compiler=NONE -cp . #{PGM}", 'Java-'],
  ]

pgms_ppff =
  NB_THREADS.map { |nb_threads| ["./#{PGM} #{nb_threads}", "PpFf-#{nb_threads}"] }

pgms_fastflow =
	[
		["./#{PGM}FastFlow", "FastFlow"]
	]

PGMS =
  pgms_java + pgms_ppff + pgms_fastflow

FICHIERS_DONNEES =
  NB_ITEMS.map { |nb| ["../testdata/stock_options_#{nb/1024}K.txt", nb] }
