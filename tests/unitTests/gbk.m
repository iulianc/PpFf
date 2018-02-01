|| -*- fundamental -*- 


|| Type synonyms.
streamOf * == [*]
mapFromTo * ** == [(*, **)]


|| Auxiliary values and functions on streams.
emptyStream = []

isEmpty [] = True
isEmpty (x : xs) = False

firstElement = hd
nextElement = tl

|| Auxiliary values and functions on maps.

emptyMap = []


addKeyValue :: * -> ** -> (mapFromTo * [**]) -> (mapFromTo * [**])
addKeyValue k v [] = [(k, [v])]
addKeyValue k v ((k, vs) : rest) = (k, v : vs) : rest
addKeyValue k v ((k2, vs) : rest) = (k2, vs) : addKeyValue k v rest


|| groupByKey definitions.

groupByKey0 :: (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** [***])
groupByKey0 keyFunc valueFunc theStream
   = groupByKey0_ keyFunc valueFunc emptyMap theStream

groupByKey0_ keyFunc valueFunc theMap theStream
   = theMap, if isEmpty theStream
   = groupByKey0_ keyFunc valueFunc newMap (nextElement theStream), otherwise
      where
         x = firstElement theStream
         k = keyFunc x
         v = valueFunc x
         newMap = addKeyValue k v theMap

groupByKey1 :: (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** [***])
groupByKey1 keyFunc valueFunc 
   = reduce addKV []
     where
         addKV x = addKeyValue (keyFunc x) (valueFunc x)

reduce = foldr



|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

inc x = x + 1

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]
str2 = [(1, "10"), (2, "20"), (1, "10"), (3, "30"), (1, "20"), (2, "20")]

res 
    = [sort (groupByKey0 fst (inc . snd) str1) 
         = [(1,[21,11,11]),(2,[21,21]),(3,[31])],
       sort (groupByKey1 fst (inc . snd) str1) 
         = [(1,[11,11,21]),(2,[21,21]),(3,[31])],
       sort (groupByKey1 (inc . snd) (inc . inc . fst) str1) 
         = [(11,[3,3]), (21,[4,3,4]), (31,[5])],
       sort (groupByKey1 fst snd str2) 
         = [(1,["10","10","20"]),(2,["20","20"]),(3,["30"])]
      ]




||


