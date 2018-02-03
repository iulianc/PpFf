|| -*- fundamental -*- 

|| Types de base.
streamOf * == [*]

mapFromTo * ** == [(*, **)]

groupByKeyResult * ::= Value * | Values [*]

value  (Value r)  = r
values (Values r) = r

getValue  xs = [(k, value v)  | (k, v) <- xs]
getValues xs = [(k, values v) | (k, v) <- xs]


||
|| Les aggregators.
||

aggregator * ::= Aggregator (* -> * -> *) || Operateur binaire pour combiner les valeurs.
                            *             || Valeur initiale/element neutre
                 |
                 NoAggregator
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

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| groupByKey.
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

|| NOTE: id = identity: id x = x
||       foldr = reduce

groupByKey = groupByKey2 || Par defaut, on doit specifier les deux fonctions (sur cle et valeur).

|| Resultat trie selon cles, pour uniformiser le resultat et simplifier les tests.
groupByKey2 :: (aggregator ***) -> (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** (groupByKeyResult ***))
groupByKey2 NoAggregator keyFunc valueFunc theStream
   = keyAndValues
     where
       || L'ensemble des differentes cles produites via keyFunc.
       keys            = sort (mkset [keyFunc x | x <- theStream])
       || On associe a chaque cle la liste des differentes valeurs obtenues via valueFunc.
       keyAndValues    = [(k, Values [valueFunc x | x <- theStream; keyFunc x = k]) | k <- keys]
groupByKey2 (Aggregator mergeValue initValue) keyFunc valueFunc theStream
   = aggregatedPairs
     where
       || L'ensemble des differentes cles produites via keyFunc.
       keys            = sort (mkset [keyFunc x | x <- theStream])
       || On associe a chaque cle la liste des differentes valeurs obtenues via valueFunc.
       keyAndValues    = [(k, [valueFunc x | x <- theStream; keyFunc x = k]) | k <- keys]
       || On reduit/combine les valeurs associees a chaque cle.
       aggregatedPairs = [(k, Value (foldr mergeValue initValue values)) | (k, values) <- keyAndValues]

||groupByKey1 :: (aggregator *) -> (* -> **) -> (streamOf *) -> (mapFromTo ** *)
groupByKey1 aggregator keyFunc
    = groupByKey2 aggregator keyFunc id

||groupByKey0 :: (aggregator *) -> (streamOf *) -> (mapFromTo * *)
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
        getValue (groupByKey sumAggregator id one str0)
          = [(10, 3), (20, 2), (30, 1)],
        || Max
        getValue (groupByKey0 maxAggregator str0)
          = [(10, 10), (20, 20), (30, 30)],
        || Min
        getValue (groupByKey0 minAggregator str0)
          = [(10, 10), (20, 20), (30, 30)],
        || Sum
        getValue (groupByKey0 sumAggregator str0)
          = [(10, 30), (20, 40), (30, 30)],
        || Vector: (++) = list concatenation
        getValue (groupByKey vectorAggregator id (mkList . id) str0)
          = [(10, [10, 10, 10]), (20, [20, 20]), (30, [30])],
        getValues (groupByKey NoAggregator id id str0)
          = [(10, [10, 10, 10]), (20, [20, 20]), (30, [30])],
        True
       ]

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res1 = [
        || Count
        getValue (groupByKey sumAggregator fst one str1)
          = [(1, 3), (2, 2), (3, 1)],
        || Max
        getValue (groupByKey maxAggregator fst snd str1)
          = [(1, 20), (2, 20), (3, 30)],
        || Min
        getValue (groupByKey minAggregator fst snd str1)
          = [(1, 10), (2, 20), (3, 30)],
        || Sum
        getValue (groupByKey sumAggregator fst snd str1)
          = [(1, 40), (2, 40), (3, 30)],
        || Vector: (++) = list concatenation
        getValue (groupByKey vectorAggregator fst (mkList . snd)  str1)
          = [(1, [10, 10, 20]), (2, [20, 20]), (3, [30])],
        || Vector: (++) = list concatenation
        getValues (groupByKey NoAggregator fst snd  str1)
          = [(1, [10, 10, 20]), (2, [20, 20]), (3, [30])],
        True
       ]

str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res2 
     = [True,
        getValue (groupByKey vectorAggregator fst (mkList . inc . snd) str1 )
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        getValue (groupByKey sumAggregator fst (inc . snd) str1 )
           = [(1,43),(2,42),(3,31)],
        getValue (groupByKey vectorAggregator fst (mkList . inc . snd) str1)
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        getValue (groupByKey vectorAggregator fst (mkList . inc . snd) str1)
          = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        getValue (groupByKey vectorAggregator (inc . snd) (mkList . inc . inc . fst) str1)
          = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
        getValue (groupByKey vectorAggregator fst (mkList . snd) str2)
          = [(1,["10","10","20"]),(2,["20","20"]),(3,["30"])],
        getValue (groupByKey (Aggregator (++) "") fst snd str2)
          = [(1,"101020"),(2,"2020"),(3,"30")],
        getValue (groupByKey (Aggregator (++) "") fst snd str2)
          = [(1,"101020"),(2,"2020"),(3,"30")],
        True
        ]

res = [||res0, 
       ||res1, 
       ||res2, 
       [all_true res0 & all_true res1 & all_true res2]]
