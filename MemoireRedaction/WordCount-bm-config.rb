################################################################
# Les divers programmmes et commandes pour les executer.
################################################################
Program.define( 'Seq', "./#{PGM}Seq" )

Program.define( 'Java+', "java -cp . #{PGM} 0" )
Program.define( 'Java-', "java -Djava.compiler=NONE -cp . #{PGM} 0" )
Program.define( 'Java*', "java -cp . #{PGM} 1" )

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
  "testdata/#{nb_items}Words.txt"
end

peu_de_donnees = [3805, 7610]

donnees_preliminaires =  [78792, 281307, 752856, 1639684]

pas_mal_de_donnees = [78792, 281307, 752856, 1639684, 2137758]

beaucoup_de_donnees = [752856, 1639684, 2614743, 5293812, 10587624]
#
################################################################
# Les experiences.
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
# plus performants.
####################################################
Experience.define( 1,
                   machines: ['c34581', 'java', 'japet'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['Java+', 'Java-', 'Java*']
                )

Experience.define( 2,
                   machines: ['c34581', 'java'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf-3']
                )

Experience.define( 3,
                   machines: ['c34581', 'java'],
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 10,
                   programs: ['FastFlow-1', 'FastFlow-2', 'FastFlow-3']
                )

Experience.define( 30,
                   machines: ['japet'],
                   nb_items: pas_mal_de_donnees,
                   nb_repetitions: 10,
                   programs: ['FastFlow-2', 'FastFlow-4', 'FastFlow-6', 'FastFlow-8']
                )
# [...]

NB_REPETITIONS_IC = 40
DONNEES_IC = beaucoup_de_donnees

Experience.define( 1001,
                   machines: ['java'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 1002,
                   machines: ['japet'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-8', 'FastFlow-8']
                )

Experience.define( 1003,
                   machines: ['MacOS', 'c34581'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-1', 'FastFlow-1']
                )
