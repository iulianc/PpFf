#!/usr/bin/env ruby

DEBUG = true

LARGEUR = 8

if DEBUG
  NB_REPETITIONS = 1 # Pour debogage: sinon 5!
else
  NB_REPETITIONS = 5
end

if DEBUG
  NB_MOTS = [10, 131, 377, 3805, 7610, 78792, 167941] # Pour debogage du script.
else
  NB_MOTS = [78792, 167941, 281307, 482636, 752856, 2614743, 5247678]
end

if DEBUG
  NB_THREADS = [1, 2]
else
  # Sur japet, plante si plus que 64..
  NB_THREADS = [1, 2, 4] # ????
end


def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

print format("\#%#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s",
            "N", "Java+", "Java-")

NB_THREADS.each do |nb_threads|
  print format(" %#{LARGEUR}s", "PpFf#{nb_threads}" )
end
print "\n"

NB_MOTS.each do  |nb_mots|
  print " #{format("%#{LARGEUR}d", nb_mots)}"

  fichier_mots = "testdata/#{nb_mots}Words.txt"

  temps_java_avec_jit = temps_moyen "java -cp . WordCount '#{fichier_mots}' 4 #{nb_mots} 0"
  temps_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}' 4 #{nb_mots} 0"

  print " #{format("%#{LARGEUR}d", temps_java_avec_jit)}"
  print " #{format("%#{LARGEUR}d", temps_java_sans_jit)}"
  NB_THREADS.each do |nb_threads|
    temps_ppff = temps_moyen "./WordCount #{fichier_mots} 4 #{nb_threads}"
    print " #{format("%#{LARGEUR}d", temps_ppff)}"
  end
  print "\n"
end
