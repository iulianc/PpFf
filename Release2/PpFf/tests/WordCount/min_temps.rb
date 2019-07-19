#
# Petit programme pour calculer la valeur minimum apparaissant dans un
# fichier de temps d'execution produits par les benchmarks.
#
# Utile pour produire un graphe bien mis en page.
#

lignes = STDIN.readlines

while (ligne = lignes.shift) !~ /^#\s*N/i
  # On ignore la ligne
end

min = 10**7
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  _, *temps = ligne_temps.split

  min = [min, *temps].map(&:to_f).min
end
puts 0.5 * min
