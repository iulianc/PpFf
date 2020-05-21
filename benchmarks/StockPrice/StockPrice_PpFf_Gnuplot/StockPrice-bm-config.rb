################################################################
# Les divers programmmes et commandes pour les executer.
################################################################
Program.define( "Seq", "./#{PGM}Seq" )

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
                   programs: ['Seq',
                              'Java+', 'Java-', 'Java*',
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
                   machines: ['c34581', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['Java+', 'Java-', 'Java*']
                )

Experience.define( 2,
                   machines: ['c34581', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf-4', 'PpFf-6', 'PpFf-8']
                )

Experience.define( 3,
                   machines: ['c34581', 'java', 'japet'],
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

Experience.define( 400,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 401, # Avec nouveaux javac/java et g++
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 7,
                   machines: ['java', 'japet'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )


# Pour voir si ca fait une difference dans le compte tremblay_gu
Experience.define( 71,
                   machines: ['japet'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )


# Experiences avec intervalle de confiance
NB_REPETITIONS_IC = 40
DONNEES_IC = beaucoup_de_donnees

Experience.define( 1001,
                   machines: ['java'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )

Experience.define( 1002,
                   machines: ['japet'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Java*', 'PpFf-4', 'FastFlow-4']
                 )

Experience.define( 1003,
                   machines: ['MacOS', 'c34581'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )
