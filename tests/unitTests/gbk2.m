|| -*- fundamental -*- 

|| Types synonymes.
streamOf * == [*]
mapFromTo * ** == [(*, **)]


||
|| Les aggregators.
||

aggregator * ::= Aggregator (* -> * -> *) || Binary operator to merge values
                            *             || Unit value
sumAggregator 
  = Aggregator (+) 0

maxAggregator 
  = Aggregator maxValue 0
     where
       maxValue x y = max [x, y]
minAggregator 
  = Aggregator minValue infini
     where
       minValue x y 
          = y, if x = infini
          = x, if y = infini
          = min [x, y], otherwise
       infini = 99912323 || BIDON
   
vectorAggregator 
  = Aggregator (++) []


|| Valeurs et fonctions auxiliaires sur les streams.
emptyStream = []

isEmpty [] = True
isEmpty (x : xs) = False

firstElement = hd
nextElement = tl


|| Auxiliary values and functions on maps.

emptyMap = []

addKeyValue :: ** -> (** -> ** -> **) -> * -> ** -> (mapFromTo * **) -> (mapFromTo * **)
addKeyValue initValue mergeValue k v [] 
   = [(k, mergeValue v initValue)]
addKeyValue initValue mergeValue k v ((k2, v2) : rest) 
   = (k, mergeValue v v2) : rest, if k = k2
   = (k2, v2) : addKeyValue initValue mergeValue k v rest, otherwise


|| Autres definitions auxiliaires.
all [] p = True
all (x : xs) p = (p x) & (all xs p)

all_true xs = all xs is_true
              where is_true x = x


|| groupByKey.

groupByKey = groupByKey2 
             || Par defaut, on doit specifier les deux fonctions (sur cle et valeur).

groupByKey2 :: (aggregator ***) -> (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** ***)
groupByKey2 theAggregator keyFunc valueFunc theStream
   = groupByKey_ keyFunc valueFunc theAggregator emptyMap theStream

groupByKey1 :: (aggregator *) -> (* -> **) -> (streamOf *) -> (mapFromTo ** *)
groupByKey1 aggregator keyFunc
    = groupByKey2 aggregator keyFunc id

groupByKey0 :: (aggregator *) -> (streamOf *) -> (mapFromTo * *)
groupByKey0 aggregator
    = groupByKey2 aggregator id id


groupByKey_ keyFunc valueFunc (Aggregator mergeValue initValue) theMap theStream
   = theMap, if isEmpty theStream
   = groupByKey_ keyFunc valueFunc (Aggregator mergeValue initValue) newMap (nextElement theStream), otherwise
      where
         x = firstElement theStream
         k = keyFunc x
         v = valueFunc x
         newMap = addKeyValue initValue mergeValue k v theMap


|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| TESTS.
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

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
        True
       ]

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res1 = [
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
          = [(1, [20, 10, 10]), (2, [20, 20]), (3, [30])],
        True
       ]

str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res2 
     = [True,
        groupByKey vectorAggregator fst (mkList . inc . snd) str1 
           = [(1,[21,11,11]),(2,[21,21]),(3,[31])],
        groupByKey sumAggregator fst (inc . snd) str1 
           = [(1,43),(2,42),(3,31)],
        sort (groupByKey vectorAggregator fst (mkList . inc . snd) str1)
           = [(1,[21,11,11]),(2,[21,21]),(3,[31])],
        sort (groupByKey vectorAggregator fst (mkList . inc . snd) str1)
          = [(1,[21, 11,11]),(2,[21,21]),(3,[31])],
        sort (groupByKey vectorAggregator (inc . snd) (mkList . inc . inc . fst) str1)
          = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
        sort (groupByKey vectorAggregator fst (mkList . snd) str2)
          = [(1,["20","10","10"]),(2,["20","20"]),(3,["30"])],
        sort (groupByKey (Aggregator (++) "") fst snd str2)
          = [(1,"201010"),(2,"2020"),(3,"30")],
        sort (groupByKey (Aggregator (++) "") fst snd str2) 
          = [(1,"201010"),(2,"2020"),(3,"30")],
        True
        ]

res = [res0, 
       ||res1, 
       ||res2, 
       [all_true res0 & all_true res1 & all_true res2]]

