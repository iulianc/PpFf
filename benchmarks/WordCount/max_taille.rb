#
# Petit programme pour calculer la valeur maximum apparaissant dans un
# fichier de temps d'execution produits par les benchmarks.
#
# Utile pour produire un graphe bien mis en page.
#

lignes = STDIN.readlines

while (ligne = lignes.shift) !~ /^#\s*N/i
  # On ignore la ligne
end

max = 0
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  taille, *temps = ligne_temps.split

  max = [max, taille.to_i].max
end

puts 1.1 * max
