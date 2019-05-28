#!
# Sinon des erreurs lorsque le programme se termine :(
export MALLOC_CHECK_=0

n=100000
nb_maps=5

echo "Effet de la granularite et nombre de threads"
#for grain in 1 2 3 4; do
for grain in 1 2 3; do
    echo "# Granularite = $grain"
    echo "# Sequentiel"
    ./Maps $n $nb_maps $grain 0
    for nb_threads in 1 2 4; do
        echo "# $nb_threads threads"
        ./Maps $n $nb_maps $grain 0
    done
    echo
done

