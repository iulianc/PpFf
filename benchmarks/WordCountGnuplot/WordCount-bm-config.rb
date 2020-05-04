if DEBUG
  NB_REPETITIONS = 2
  #NB_ITEMS = [377, 3805, 7610]
  NB_ITEMS = [3805, 7610, 78792, 167941]
else
  NB_REPETITIONS = 10
  NB_ITEMS = [78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743]
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
    ["java -cp . #{PGM}Warmup", 'Java*'],
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
  NB_ITEMS.map { |nb| ["testdata/#{nb}Words.txt", nb] }
