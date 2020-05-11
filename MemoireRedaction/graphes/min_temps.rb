#!/usr/bin/env ruby
#
# Petit programme pour calculer la valeur minimum apparaissant dans un
# fichier de temps d'execution produits par les benchmarks.
#
# Utile pour produire un graphe bien mis en page.
#

lignes = STDIN.readlines
min = 10**7
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  _, *temps = ligne_temps.split

  # On ne conserve que les moyennes.
  moys = []
  temps.each_with_index do |x, i|
    moys << x if i.even?
  end

  min = [min, *moys].map(&:to_f).min
end
puts 0.5 * min
