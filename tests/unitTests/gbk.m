|| -*- fundamental -*- 

|| Types synonymes.
streamOf * == [*]
mapFromTo * ** == [(*, **)]


||
|| Les aggregators.
||

aggregator * ::= Aggregator (* -> * -> *) || Operateur binaire pour combiner les valeurs.
                            *             || Valeur initiale/element neutre
sumAggregator 
  = Aggregator (+) 0

maxAggregator 
  = Aggregator maxValue 0
     where
       maxValue x y = max [x, y]

minAggregator 
  = Aggregator minValue infini
     where
       minValue x y = y, if x = infini
                    = x, if y = infini
                    = min [x, y], otherwise
       infini = 99912323 || BIDON
   
vectorAggregator 
  = Aggregator (++) []

avgAggregator 
  = Aggregator sumCountAvg [0, 0]
    where sumCountAvg [x1, y1] [x2, y2] = [x1 + x2, y1 + y2]

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| groupByKey.
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

|| NOTE: id = identity: id x = x
||       foldr = reduce

groupByKey = groupByKey2 || Par defaut, on doit specifier les deux fonctions (sur cle et valeur).

|| Resultat trie selon cles, pour uniformiser le resultat et simplifier les tests.
groupByKey2 :: (aggregator ***) -> (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** ***)
groupByKey2 (Aggregator mergeValue initValue) keyFunc valueFunc theStream
   = aggregatedPairs
     where
       || L'ensemble des differentes cles produites via keyFunc.
       keys            = sort (mkset [keyFunc x | x <- theStream])
       || On associe a chaque cle la liste des differentes valeurs obtenues via valueFunc.
       keyAndValues    = [(k, [valueFunc x | x <- theStream; keyFunc x = k]) | k <- keys]
       || On reduit/combine les valeurs associees a chaque cle.
       aggregatedPairs = [(k, foldr mergeValue initValue values) | (k, values) <- keyAndValues]

groupByKey1 :: (aggregator *) -> (* -> **) -> (streamOf *) -> (mapFromTo ** *)
groupByKey1 aggregator keyFunc
    = groupByKey2 aggregator keyFunc id

groupByKey0 :: (aggregator *) -> (streamOf *) -> (mapFromTo * *)
groupByKey0 aggregator
    = groupByKey2 aggregator id id


|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| TESTS.
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

all [] p = True
all (x : xs) p = (p x) & (all xs p)

all_true xs = all xs is_true
              where is_true x = x

inc x = x + 1
mkList x = [x]
one x = 1


str0 = [10, 20, 10, 20, 30, 10]

res0 = [
        || Count
        groupByKey sumAggregator id one str0
          = [(10, 3), (20, 2), (30, 1)],
        || Max
        groupByKey0 maxAggregator str0
          = [(10, 10), (20, 20), (30, 30)],
        || Min
        groupByKey0 minAggregator str0
          = [(10, 10), (20, 20), (30, 30)],
        || Sum
        groupByKey0 sumAggregator str0
          = [(10, 30), (20, 40), (30, 30)],
        || Vector: (++) = list concatenation
        groupByKey vectorAggregator id (mkList . id) str0
          = [(10, [10, 10, 10]), (20, [20, 20]), (30, [30])],
        || Average
        groupByKey avgAggregator id id_one str0
          = [(10, [30, 3]), (20, [40, 2]), (30, [30, 1])],
        groupByKey avgAggregator moy id_one str0
          = [("moyenne", [100 , 6])],
        True
       ]

id_one x = [x, 1]
moy x = "moyenne"

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res1 = [True,
        || Count
        groupByKey sumAggregator fst one str1
          = [(1, 3), (2, 2), (3, 1)],
        || Max
        groupByKey maxAggregator fst snd str1
          = [(1, 20), (2, 20), (3, 30)],
        || Min
        groupByKey minAggregator fst snd str1
          = [(1, 10), (2, 20), (3, 30)],
        || Sum
        groupByKey sumAggregator fst snd str1
          = [(1, 40), (2, 40), (3, 30)],
        || Vector: (++) = list concatenation
        groupByKey vectorAggregator fst (mkList . snd)  str1
          = [(1, [10, 10, 20]), (2, [20, 20]), (3, [30])],
        True
       ]

str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res2 = [True,
        groupByKey vectorAggregator fst (mkList . inc . snd) str1 
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey sumAggregator fst (inc . snd) str1 
           = [(1,43),(2,42),(3,31)],
        groupByKey vectorAggregator fst (mkList . inc . snd) str1
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey vectorAggregator fst (mkList . inc . snd) str1
          = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey vectorAggregator (inc . snd) (mkList . inc . inc . fst) str1
          = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
        groupByKey vectorAggregator fst (mkList . snd) str2
          = [(1,["10","10","20"]),(2,["20","20"]),(3,["30"])],
        groupByKey (Aggregator (++) "") fst snd str2
          = [(1,"101020"),(2,"2020"),(3,"30")],
        groupByKey (Aggregator (++) "") fst snd str2
          = [(1,"101020"),(2,"2020"),(3,"30")],
        True
        ]

employee ::= Employee num [char] num
age (Employee a n s) = a
nom (Employee a n s) = n
salaire (Employee a n s) = s

employes = [Employee 22 "n1" 0,
            Employee 22 "n2" 0,
            Employee 33 "n3" 0,
            Employee 33 "n4" 0,
            Employee 44 "n5" 0,
            Employee 22 "n6" 0
           ]

res3 = [True,
        groupByKey sumAggregator age one employes 
          = [(22, 3), (33, 2), (44, 1)],
        True
        ]
  
res = [res0, 
       ||res1, 
       ||res2, 
       res3,
       [all_true res0 & all_true res1 & all_true res2 & all_true res3]]
