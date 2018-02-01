|| -*- fundamental -*- 

|| Types synonymes.
streamOf * == [*]
mapFromTo * ** == [(*, **)]

aggregator * ** == (*, ** -> ** -> **)

|| Auxiliary values and functions on streams.
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

groupByKey :: (* -> **) -> (* -> ***) -> *** -> (*** -> *** -> ***) -> (streamOf *) -> (mapFromTo ** ***)
groupByKey keyFunc valueFunc initValue mergeValue theStream
   = groupByKey_ keyFunc valueFunc initValue mergeValue emptyMap theStream

groupByKey_ keyFunc valueFunc initValue mergeValue theMap theStream
   = theMap, if isEmpty theStream
   = groupByKey_ keyFunc valueFunc initValue mergeValue newMap (nextElement theStream), otherwise
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

maxValue x y = max [x, y]

infini = 99912323 || BIDON

minValue x y 
  = y, if x = infini
  = x, if y = infini
  = min [x, y], otherwise
             

str0 = [10, 20, 10, 20, 30, 10]

res0 = [
        || Count
        groupByKey id one 0 (+) str0
          = [(10, 3), (20, 2), (30, 1)],
        || Max
        groupByKey id id 0 maxValue str0
          = [(10, 10), (20, 20), (30, 30)],
        || Min
        groupByKey id id infini minValue str0
          = [(10, 10), (20, 20), (30, 30)],
        || Sum
        groupByKey id id 0 (+) str0
          = [(10, 30), (20, 40), (30, 30)],
        || Vector: (++) = list concatenation
        groupByKey id (mkList . id) [] (++) str0
          = [(10, [10, 10, 10]), (20, [20, 20]), (30, [30])],
        True
       ]

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res1 = [
        || Count
        groupByKey fst one 0 (+) str1
          = [(1, 3), (2, 2), (3, 1)],
        || Max
        groupByKey fst snd 0 maxValue str1
          = [(1, 20), (2, 20), (3, 30)],
        || Min
        groupByKey fst snd infini minValue str1
          = [(1, 10), (2, 20), (3, 30)],
        || Sum
        groupByKey fst snd 0 (+) str1
          = [(1, 40), (2, 40), (3, 30)],
        || Vector: (++) = list concatenation
        groupByKey fst (mkList . snd) [] (++) str1
          = [(1, [20, 10, 10]), (2, [20, 20]), (3, [30])],
        True
       ]

str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res2 
      = [groupByKey fst (mkList . inc . snd) [] (++) str1 
           = [(1,[21,11,11]),(2,[21,21]),(3,[31])],
         groupByKey fst (inc . snd) 0 (+) str1 
           = [(1,43),(2,42),(3,31)],
        sort (groupByKey fst (mkList . inc . snd) [] (++) str1) 
           = [(1,[21,11,11]),(2,[21,21]),(3,[31])],
        sort (groupByKey fst (mkList . inc . snd) [] (++) str1)
          = [(1,[21, 11,11]),(2,[21,21]),(3,[31])],
        sort (groupByKey (inc . snd) (mkList . inc . inc . fst) [] (++) str1) 
          = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
        sort (groupByKey fst (mkList . snd) [] (++) str2) 
          = [(1,["20","10","10"]),(2,["20","20"]),(3,["30"])],
        sort (groupByKey fst snd "" (++) str2) 
          = [(1,"201010"),(2,"2020"),(3,"30")],
        sort (groupByKey fst snd "" (++) str2) 
          = [(1,"201010"),(2,"2020"),(3,"30")],
        True
        ]

res = [res0, 
       ||res1, 
       ||res2, 
       [all_true res0 & all_true res1 & all_true res2]]

