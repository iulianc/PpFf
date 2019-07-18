#!/usr/bin/env ruby

LARGEUR = 8

NB_REPETITIONS = 5

NB_MOTS =
  [78792, 167941, 281307, 482636, 752856, 2614743, 5247678]
# [78792, 167941] # Pour debogage du script.
# [497,78792,5247678] # Pour debogage du script.

# Sur japet, plante si plus que 64, donc au plus 32... fois 2 -- voir plus bas.
MAX_NB_THREADS =
  # [`cat /proc/cpuinfo | grep processor | wc -l`.to_i, 32].min
  2 # Pour debogage du script


def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

NB_MOTS.each do  |nb_mots|
  fichier_mots = "testdata/#{nb_mots}Words.txt"

  puts "# Nb. mots = #{nb_mots}"
  puts format("\#%#{LARGEUR-1}s %#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s",
              "Nbt", "Java+", "Java-", "PpFf")


  temps_java_avec_jit = temps_moyen "java -cp . WordCount '#{fichier_mots}' 4 #{nb_mots} 0"
  temps_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}' 4 #{nb_mots} 0"

  nb_threads = 1
  while nb_threads <= MAX_NB_THREADS do
    print "#{format("%#{LARGEUR}d", nb_threads)}"
    print " #{format("%#{LARGEUR}d", temps_java_avec_jit)}"
    print " #{format("%#{LARGEUR}d", temps_java_sans_jit)}"

    temps_ppff = temps_moyen "./WordCount #{fichier_mots} 4 #{nb_threads}"
    print " #{format("%#{LARGEUR}d", temps_ppff)}"
    print "\n"
    nb_threads *= 2
  end
end
