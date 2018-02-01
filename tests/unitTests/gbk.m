|| -*- fundamental -*- 

streamOf * == [*]

mapFromTo * ** == [(*, **)]


groupByKey :: (* -> **) -> (* -> ***) -> (streamOf *) -> (mapFromTo ** [***])
groupByKey keyFunc valueFunc [] = []
groupByKey keyFunc valueFunc (x : xs) = add (keyFunc x) (valueFunc x) (groupByKey keyFunc valueFunc xs)

add k v [] = [(k, [v])]
add k v ((k, vs) : rest) = (k, v : vs) : rest
add k v ((k2, vs) : rest) = (k2, vs) : add k v rest

str1 = [(1, 10), (2, 20), (1, 10), (3, 30), (1, 20), (2, 20)]

res = groupByKey fst (inc . snd) str1

inc x = x + 1


