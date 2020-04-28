#/usr/bin/env ruby
#
# Petit programme pour calculer la valeur mininum apparaissant dans un
# fichier de temps d'execution produits par les benchmarks.
#
# Utile pour produire un graphe bien mis en page.
#

lignes = STDIN.readlines

min = ARGV[0].to_i
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  taille, *temps = ligne_temps.split

  min = [min, taille.to_i].min
end
puts 0
