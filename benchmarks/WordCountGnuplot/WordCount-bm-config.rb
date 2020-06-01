# NOTE: Les differentes tailles des fichiers de donnes sont les
#   suivantes: 10, 131, 377, 3805, 7610, 78792, 167941, 281307,
#   482636, 752856, 1639684, 2137758, 2614743, 5293812, 10587624

# NOTE: Les elements qui doivent etre utilises dans run_bms.rb sont
# definis ci-bas sous forme de CONSTANTES -- sinon, elles ne
# semblaient pas visibles.


################################################################
# Les divers programmmes et commandes pour les executer.
################################################################
Program.define( 'Seq', "./#{PGM}Seq" )

Program.define( 'Java-', "java -cp . #{PGM} 10" )  # Seq. sans warmup
Program.define( 'Java',  "java -cp . #{PGM} 11" )  # Seq. avec warmup
Program.define( 'Java+', "java -cp . #{PGM} 20" )  # Par. sans warmup
Program.define( 'Java*', "java -cp . #{PGM} 21" )  # Par. avec warmup

[*1..4].each do |k|
  Program.define( "Java*#{k}", "java -cp . #{PGM} #{k}" )
end

[*1..8].each do |k|
  Program.define( "PpFf-#{k}", "./#{PGM} #{k}" )
end

[*1..8].each do |k|
  Program.define( "PpFf*-#{k}", "./#{PGM}Merged #{k}" )
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

peu_de_donnees =
  [3805, 7610]

donnees_preliminaires =
  [78792, 281307, 752856, 1639684]

pas_mal_de_donnees =
  [78792, 281307, 752856, 1639684, 2137758]

beaucoup_de_donnees =
  [752856, 1639684, 2614743, 5293812, 10587624]

encore_beaucoup_de_donnees =
  [752856, 1639684, 2614743, 5177161, 10185035]


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


# Pour voir les differences entre les diverses versions de Java.
Experience.define( 11,
                   nb_items: donnees_preliminaires,
                   nb_repetitions: 20,
                   programs: ['Java-', 'Java', 'Java+', 'Java*']
                )

# Pour comparer versions PpFf sans vs. avec fusion
Experience.define( 22,
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 10,
                   programs: ['PpFf-2', 'PpFf*-2', 'PpFf*-4']
                )

Experience.define( 23,
                   machines: 'c34581',
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf*-1', 'PpFf*-2', 'PpFf*-3']
                )

# Sur japet, les experiences 2 et 3 donnent les meilleurs resultats
# avec 3, donc on va voir ce qu'il en est si on augmente encore.
Experience.define( 20,
                   machines: ['japet'],
                   nb_items: pas_mal_de_donnees,
                   nb_repetitions: 10,
                   programs: ['PpFf-1', 'PpFf-2', 'PpFf-4', 'PpFf-5', 'PpFf-6', 'PpFf-8']
                )

Experience.define( 30,
                   machines: ['japet'],
                   nb_items: pas_mal_de_donnees,
                   nb_repetitions: 10,
                   programs: ['FastFlow-1', 'FastFlow-2', 'FastFlow-4', 'FastFlow-5', 'FastFlow-6', 'FastFlow-8']
                )

# Les plus performants.
Experience.define( 4,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-1', 'FastFlow-1']
                )

Experience.define( 400,
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-1', 'FastFlow-1']
                )

Experience.define( 401, # Avec nouveaux javac/java et g++
                   machines: ['c34581'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-1', 'FastFlow-1']
                )

Experience.define( 5,
                   machines: ['java'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 500,
                   machines: ['java'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 51,
                   machines: ['java'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java+', 'Java*', 'PpFf-1', 'PpFf-2', 'FastFlow-2', 'FastFlow-1']
                )

Experience.define( 6,
                   machines: ['japet'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Java*', 'PpFf-8', 'FastFlow-8']
                )

Experience.define( 600,
                   machines: ['japet'],
                   nb_items: beaucoup_de_donnees,
                   nb_repetitions: 30,
                   programs: ['Seq', 'Java*', 'PpFf-8', 'FastFlow-8']
                )

# Experiences avec intervalle de confiance
NB_REPETITIONS_IC = 40
DONNEES_IC = beaucoup_de_donnees

Experience.define( 1001,
                   machines: ['java'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 10010,
                   machines: ['java'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 10011,
                   machines: ['java'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java+', 'Java*', 'PpFf-2', 'FastFlow-2']
                )

Experience.define( 1002,
                   machines: ['japet'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-8', 'FastFlow-8']
                )

Experience.define( 10020,
                   machines: ['japet'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-8', 'FastFlow-5']
                )

Experience.define( 1003,
                   machines: ['MacOS', 'c34581'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-1', 'FastFlow-1']
                )

Experience.define( 2001,
                   machines: ['java', 'MacOS'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java+', 'Java*', 'PpFf-2', 'PpFf*-4']
                   )

Experience.define( 20010,
                   machines: ['java', 'MacOS'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java*', 'PpFf-1', 'PpFf-2', 'PpFf*-1', 'PpFf*-2']
                   )

Experience.define( 2003,
                   machines: ['c34581', 'MacOS'],
                   nb_items: DONNEES_IC,
                   nb_repetitions: NB_REPETITIONS_IC,
                   programs: ['Seq', 'Java+', 'Java*', 'PpFf-1', 'PpFf*-2']
                   )
