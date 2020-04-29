#!/usr/bin/env ruby

DEBUG = true

# Pour utiliser facilement sur diverses machines, donc MacBook et
# Linux.
SERVER = ENV['HOST']
FICHIER = "graphes/temps-#{SERVER}-wc.txt"

# Pour le nombre maximum de threads, on utilise un petit nombre de
# processeurs qui depend de la machine.
max_threads = nil
if SERVER == 'japet'
  max_threads = 8
elsif SERVER == 'java'
  max_threads = 4
elsif SERVER == 'c34581'
  max_threads = 4
else
  max_threads = %x{nproc}.chomp.to_i / 2
end

NB_THREADS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
             .take_while { |n| n <= max_threads}

if DEBUG
  puts "*** Cree le fichier #{FICHIER} (avec au plus #{max_threads} threads)"
end

LARGEUR = 8

if DEBUG
  NB_REPETITIONS = 3
  #NB_MOTS = [10, 131, 377, 3805]
  NB_MOTS = [78792, 167941, 281307, 482636]
else
  NB_REPETITIONS = 10
  NB_MOTS = [78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743]
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

result = '';
NB_MOTS.each do  |nb_mots|
  partial_result = " #{format("%#{LARGEUR}d", nb_mots)}"

  fichier_mots = "testdata/#{nb_mots}Words.txt"

  temps_java_avec_jit = temps_moyen "java -cp . WordCount '#{fichier_mots}'"
  temps_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}'"

  partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_java_avec_jit)}"
  partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_java_sans_jit)}"

  NB_THREADS.each do |nb_threads|
    temps_ppff = temps_moyen "./WordCount #{fichier_mots} #{nb_threads}"
    partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_ppff)}"
  end

  print partial_result
  print "\n"
  result = result + partial_result + "\n"
end

File.open(FICHIER, 'w') { |file| file.write(result) }
