# Pour utiliser facilement sur diverses machines, dont MacBook, Linux, etc.
server = ENV['HOST']

# NOTE: Les differentes tailles des fichiers de donnes sont les
#   suivantes: 10, 131, 377, 3805, 7610, 78792, 167941, 281307,
#   482636, 752856, 1639684, 2137758, 2614743, 5293812, 10587624

# NOTE: Les elements qui doivent etre utilises dans run_bms.rb sont
# definis ci-bas sous forme de CONSTANTES -- sinon, elles ne
# semblaient pas visibles.

def nb_farm_workers_for( server )
  max_nb_farm_workers =
    case server
    when 'java', 'c34581', 'MacOS' then 4
    when 'japet' then 8
    else %x{nproc}.chomp.to_i / 2
    end

  [1, 2, 4, 8, 16, 32, 64].take_while { |n| n <= max_nb_farm_workers }
end

#
# Les differentes valeurs possibles selon le $niveau.
#
nb_items_ = {
  0 => [3805, 7610, 78792],
  1 => [78792, 167941, 281307, 482636, 752856, 1639684],
  2 => [1639684, 2614743, 5293812, 10587624],
}

nb_repetitions_ = {
  0 => 2,
  1 => 10,
  2 => 30,
}

nb_farm_workers_ = {
  0 => [1, 4],
  1 => nb_farm_workers_for(server),
  2 => [1, 2],
}

nb_items = nb_items_[$niveau]

NB_REPETITIONS = nb_repetitions_[$niveau]
NB_FARM_WORKERS = nb_farm_workers_[$niveau]

#
# Les divers fichiers utilises et generes
#

FICHIERS_DONNEES = nb_items.map { |nb| ["testdata/#{nb}Words.txt", nb] }

FICHIER_INFOS  = "resultats/#{PGM}-infos-#{server}-#{NB_REPETITIONS}.txt"
FICHIER_TEMPS  = "resultats/#{PGM}-temps-#{server}-#{NB_REPETITIONS}.txt"
FICHIER_DEBITS = "resultats/#{PGM}-debits-#{server}-#{NB_REPETITIONS}.txt"


######################################################
# IMPORTANT C'est l'element PGMS qu'il faut modifier si on veut
# ajouter d'autres programmes a benchmarker ou selectionner un
# sous-ensemble de programmes pour le niveau 2.
######################################################

# Les programmes a executer.
tous_les_pgms_java =
  [
   ["java -cp . #{PGM}", 'Java+'],
   ["java -Djava.compiler=NONE -cp . #{PGM}", 'Java-'],
   ["java -cp . #{PGM}Warmup", 'Java*'],
  ]

pgms_java =
  case $niveau
  when 0, 1 then tous_les_pgms_java
  else tous_les_pgms_java[-1..-1] # A VERIFIER!
  end

pgms_ppff =
  NB_FARM_WORKERS.map { |nb_threads| ["./#{PGM} #{nb_threads}", "PpFf-#{nb_threads}"] }

pgms_fastflow =
  NB_FARM_WORKERS.map { |nb_threads| ["./#{PGM}FastFlow #{nb_threads}", "FastFlow-#{nb_threads}"] }


PGMS =
  pgms_java + pgms_ppff + pgms_fastflow
