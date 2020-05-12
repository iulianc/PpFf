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
  "../testdata/stock_options_#{nb_items/1024}K.txt"
end

peu_de_donnees =
  [4 * 1024, 8 * 1024]

donnees_preliminaires =
  [8, 32, 64].map { |n| n * 1024}

beaucoup_de_donnees =
  [16, 32, 64, 128, 256].map { |n| n * 1024 }


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
                   programs: ['Java+', 'Java-', 'Java*',
                              'PpFf-1', 'PpFf-2', 'PpFf-4',
                              'FastFlow-1', 'FastFlow-2', 'FastFlow-4',
                             ]
                )

####################################################
# Experiences faites sur les machines indiquees pour identifier les
# programmes de chaque groupe (Java, PpFf et FastFlow) qui sont les
# plus performants. Ceci permet ensuite d'effectuer la comparaison
# finale, avec beaucoup de donnees, uniquement avec ces programmes, un
# par groupe.
####################################################
Experience.define( 1,
                   machines: ['c34581', 'MacOS'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['Java+', 'Java-', 'Java*']
                )

Experience.define( 2,
                   machines: ['c34581', 'MacOS'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf-3']
                )

Experience.define( 3,
                   machines: ['c34581', 'MacOS'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['FastFlow-1', 'FastFlow-2', 'FastFlow-3']
                )


# Les plus performants.
Experience.define( 4,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-1', 'FastFlow-1']
                )
