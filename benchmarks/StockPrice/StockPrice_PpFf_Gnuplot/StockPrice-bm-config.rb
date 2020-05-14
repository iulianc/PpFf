################################################################
# Les divers programmmes et commandes pour les executer.
################################################################
Program.define( "Java+", "java -cp . #{PGM} 0" )
Program.define( "Java-", "java -Djava.compiler=NONE -cp . #{PGM} 0" )
Program.define( "Java*", "java -cp . #{PGM} 1" )

[*1..8].each do |k|
  Program.define( "PpFf-#{k}", "./#{PGM} #{k}" )
end

[*1..8].each do |k|
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
  [64, 128, 512].map { |n| n * 1024 }

beaucoup_de_donnees_mac =
  [128, 256, 512, 1024].map { |n| n * 1024 }

beaucoup_de_donnees =
  [128, 256, 512, 1024, 2048].map { |n| n * 1024 }


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
                   machines: ['c34581', 'MacOS', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['Java+', 'Java-', 'Java*']
                )

Experience.define( 2,
                   machines: ['c34581', 'MacOS', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf-4', 'PpFf-6', 'PpFf-8']
                )

Experience.define( 3,
                   machines: ['c34581', 'MacOS', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['FastFlow-1', 'FastFlow-2', 'FastFlow-4', 'FastFlow-6', 'FastFlow-8']
                )


# Les plus performants.
Experience.define( 4,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 6,
                   machines: ['MacOS'],
                   nb_items: beaucoup_de_donnees_mac,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-1', 'FastFlow-1']
                )

Experience.define( 7,
                   machines: ['java'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )

Experience.define( 8,
                   machines: ['java'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )
