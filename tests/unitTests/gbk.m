|| -*- fundamental -*- 

|| Types synonymes.
streamOf * == [*]
mapFromTo * ** == [(*, **)]


||
|| Les reducers.
||

reducer * ** ::= Reducer (* -> ** -> **)  || Combine l'element du stream (*) avec l'accumulateur (**)
                         (** -> ** -> **) || Combine les accumulateurs (**) des threads
                         **               || Valeur initiale/element neutre (**)
sumReducer 
  = Reducer (+) (+) 0

maxReducer 
  = Reducer maxValue maxValue 0
     where
       maxValue x y = max [x, y]

minReducer 
  = Reducer minValue minValue infini
     where
       minValue x y = y, if x = infini
                    = x, if y = infini
                    = min [x, y], otherwise
       infini = 99912323 || BIDON
   
vectorReducer 
  = Reducer (++) (++) []

avgReducer 
  = Reducer sumCountAvg sumCountAvg [0, 0]
    where sumCountAvg [x1, y1] [x2, y2] = [x1 + x2, y1 + y2]

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| groupByKey.
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

|| NOTE: id = identity: id x = x
||       foldr = reduce

groupByKey = groupByKey2 || Par defaut, on doit specifier les deux fonctions (sur cle et valeur).

|| Resultat trie selon cles, pour uniformiser le resultat et simplifier les tests.
groupByKey2 :: (reducer *** ***) -> (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** ***)
groupByKey2 (Reducer injectValue combineValues initValue) keyFunc valueFunc theStream
   = aggregatedPairs
     where
       || L'ensemble des differentes cles produites via keyFunc.
       keys            = sort (mkset [keyFunc x | x <- theStream])
       || On associe a chaque cle la liste des differentes valeurs obtenues via valueFunc.
       keyAndValues    = [(k, [valueFunc x | x <- theStream; keyFunc x = k]) | k <- keys]
       || On reduit/combine les valeurs associees a chaque cle.
       aggregatedPairs = [(k, foldr injectValue initValue values) | (k, values) <- keyAndValues]

groupByKey1 :: (reducer * *) -> (* -> **) -> (streamOf *) -> (mapFromTo ** *)
groupByKey1 reducer keyFunc
    = groupByKey2 reducer keyFunc id

groupByKey0 :: (reducer * *) -> (streamOf *) -> (mapFromTo * *)
groupByKey0 reducer
    = groupByKey2 reducer id id


||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| reduce
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

nb_threads = 3

|| Fonction reduce avec pseudo-decomposition en sous-streams traites par des threads distincts.
|| Definie uniquement pour assurer validite des exemples/tests avec les deux fonctions d'un reduecr.
reduce :: (reducer * **) -> (streamOf *) -> **
reduce (Reducer injectValue combineThreadValues initValue) theStream
    = foldr combineThreadValues initValue threadResults
      where
        subStreams = splitIntoSubstreams theStream nb_threads
                     where
                       splitIntoSubstreams theStream nb_threads = split theStream nb_threads [[] | i <- [1..nb_threads]] 0
                       split []       nb_threads sub_streams k = sub_streams
                       split (x : xs) nb_threads sub_streams k = split xs nb_threads (tl sub_streams ++ [x : hd sub_streams]) ((k+1) mod nb_threads)
        threadResults = [foldr injectValue initValue subStream | subStream <- subStreams]


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
        groupByKey sumReducer id one str0
          = [(10, 3), (20, 2), (30, 1)],
        || Max
        groupByKey0 maxReducer str0
          = [(10, 10), (20, 20), (30, 30)],
        || Min
        groupByKey0 minReducer str0
          = [(10, 10), (20, 20), (30, 30)],
        || Sum
        groupByKey0 sumReducer str0
          = [(10, 30), (20, 40), (30, 30)],
        || Vector: (++) = list concatenation
        groupByKey vectorReducer id (mkList . id) str0
          = [(10, [10, 10, 10]), (20, [20, 20]), (30, [30])],
        || Average
        groupByKey avgReducer id id_one str0
          = [(10, [30, 3]), (20, [40, 2]), (30, [30, 1])],
        groupByKey avgReducer moy id_one str0
          = [("moyenne", [100 , 6])],
        True
       ]

id_one x = [x, 1]
moy x = "moyenne"

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res1 = [True,
        || Count
        groupByKey sumReducer fst one str1
          = [(1, 3), (2, 2), (3, 1)],
        || Max
        groupByKey maxReducer fst snd str1
          = [(1, 20), (2, 20), (3, 30)],
        || Min
        groupByKey minReducer fst snd str1
          = [(1, 10), (2, 20), (3, 30)],
        || Sum
        groupByKey sumReducer fst snd str1
          = [(1, 40), (2, 40), (3, 30)],
        || Vector: (++) = list concatenation
        groupByKey vectorReducer fst (mkList . snd)  str1
          = [(1, [10, 10, 20]), (2, [20, 20]), (3, [30])],
        True
       ]

str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res2 = [True,
        groupByKey vectorReducer fst (mkList . inc . snd) str1 
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey sumReducer fst (inc . snd) str1 
           = [(1,43),(2,42),(3,31)],
        groupByKey vectorReducer fst (mkList . inc . snd) str1
           = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey vectorReducer fst (mkList . inc . snd) str1
          = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
        groupByKey vectorReducer (inc . snd) (mkList . inc . inc . fst) str1
          = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
        groupByKey vectorReducer fst (mkList . snd) str2
          = [(1,["10","10","20"]),(2,["20","20"]),(3,["30"])],
        groupByKey (Reducer (++) (++) "") fst snd str2
          = [(1,"101020"),(2,"2020"),(3,"30")],
        groupByKey (Reducer (++) (++) "") fst snd str2
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
        groupByKey sumReducer age one employes 
          = [(22, 3), (33, 2), (44, 1)],
        True
        ]
  
res4 = [True,
       || Count.
        reduce (Reducer plus_1 (+) 0) employes 
          = 6,
       || Somme des ages.
        reduce (Reducer plus_age (+) 0) employes 
          = 176,
       || Moyenne des ages.
        reduce (Reducer avg sum (0, 0)) employes 
          = (176/6, 6),
        True
        ]
        where
          plus_age e n = (age e) + n
          plus_1 e n = n + 1
          avg e (somme, nb)  = ((nb * somme + (age e)) / (nb + 1), nb + 1)
          sum (x1, y1) (x2, y2) = ((x1 * y1 + x2 * y2) / (y1 + y2), y1+y2)

res = [||res0, 
       ||res1, 
       ||res2, 
       ||res3,
       res4,
       [all_true res0 & all_true res1 & all_true res2 & all_true res3 & all_true res4]]
