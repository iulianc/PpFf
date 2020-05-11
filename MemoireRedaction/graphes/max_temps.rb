#!/usr/bin/env ruby
#
# Petit programme pour calculer la valeur maximum apparaissant dans un
# fichier de temps d'execution produits par les benchmarks.
#
# Utile pour produire un graphe bien mis en page.
#

lignes = STDIN.readlines

max = 0.0
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  _, *temps = ligne_temps.split

  max = [max, *temps].map(&:to_f).max
end
puts max * 1.2
