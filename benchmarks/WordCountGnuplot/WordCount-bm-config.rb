# NOTE: Les differentes tailles des fichiers de donnes sont les
#   suivantes: 10, 131, 377, 3805, 7610, 78792, 167941, 281307,
#   482636, 752856, 1639684, 2137758, 2614743, 5293812, 10587624

# NOTE: Les elements qui doivent etre utilises dans run_bms.rb sont
# definis ci-bas sous forme de CONSTANTES -- sinon, elles ne
# semblaient pas visibles.


################################################################
# Les divers programmmes et commandes pour les executer.
################################################################
Program.define( "Java+", "java -cp . #{PGM} 0" )
Program.define( "Java-", "java -Djava.compiler=NONE -cp . #{PGM} 0" )
Program.define( "Java*", "java -cp . #{PGM} 1" )

[1, 2, 4, 8, 16, 32].each do |k|
  Program.define( "PpFf-#{k}", "./#{PGM} #{k}" )
  Program.define( "FastFlow-#{k}", "./#{PGM}FastFlow #{k}" )
end


################################################################
# Diverses quantites de donnees et la fonction pour les obtenir.
################################################################

def fichier_de_donnees_pour(nb_items)
  "testdata/#{nb_items}Words.txt"
end

peu_de_donnees =
  [3805, 7610]

plus_de_donnees =
  [3805, 7610, 78792]

pas_mal_de_donnees =
  [78792, 167941, 281307, 482636, 752856, 1639684]

beaucoup_de_donnees =
  [1639684, 2614743, 5293812, 10587624]

################################################################
# Les experiences.
#
# Note: Si machines: n'est pas specifie, alors peut s'executer sur
# n'importe quelle machines
#
################################################################

Experience.define( 0,
                   nb_items: peu_de_donnees,
                   nb_repetitions: 2,
                   programs: ["Java+", "Java-", "Java*",
                              "PpFf-1", "PpFf-2", "PpFf-4",
                              "FastFlow-1", "FastFlow-2", "FastFlow-4",
                             ]
                )

Experience.define( 1,
                   machines: ['MacOS'],
                   nb_items: plus_de_donnees,
                   nb_repetitions: 5,
                   programs: ["PpFf-1", "PpFf-2", "PpFf-4"]
                )

Experience.define( 2,
                   machines: ['MacOS'],
                   nb_items: plus_de_donnees,
                   nb_repetitions: 5,
                   programs: ["Java*", "PpFf-2", "FastFlow-1"]
                )

Experience.define( 3,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ["Java*", "PpFf-2", "FastFlow-2"]
                )
