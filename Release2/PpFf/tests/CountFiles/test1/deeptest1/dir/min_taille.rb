
#while (ligne = lignes.shift) !~ /^#\s*N/i
#  # On ignore la ligne
#end

min = ARGV[0].to_i
lignes.each do |ligne_temps|
  # On obtient les informations de la ligne.
  taille, *temps = ligne_temps.split

  min = [min, taille.to_i].min
end
puts 0
