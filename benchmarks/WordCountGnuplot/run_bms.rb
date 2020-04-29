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
  NB_REPETITIONS = 4
  NB_MOTS = [377, 3805, 7610]
  #NB_MOTS = [78792, 167941, 281307, 482636]
else
  NB_REPETITIONS = 10
  NB_MOTS = [78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743]
end

def temps_moyen( cmd )
  les_temps = []
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
    les_temps << temps
  end

  moy = (temps_tot / NB_REPETITIONS).round(1)

  ecart_type = (Math.sqrt (les_temps.reduce(0.0) { |s, x| (x - moy) * (x - moy) } / (NB_REPETITIONS - 1))).round(1)

  [moy, ecart_type]
end

print format("\#%#{LARGEUR}s %#{2*LARGEUR-1}s %#{2*LARGEUR-1}s",
             "N", "Java+", "Java-")

NB_THREADS.each do |nb_threads|
  print format(" %#{2*LARGEUR-1}s", "PpFf#{nb_threads}" )
end
print "\n"

FMT_MOY = "%#{LARGEUR}.1f"
FMT_ET = "%#{LARGEUR}.1f"

result = '';
NB_MOTS.each do  |nb_mots|
  partial_result = " #{format("%#{LARGEUR}d", nb_mots)}"

  fichier_mots = "testdata/#{nb_mots}Words.txt"

  temps_java_avec_jit, et_java_avec_jit = temps_moyen "java -cp . WordCount '#{fichier_mots}'"
  temps_java_sans_jit, et_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}'"

  partial_result = partial_result + format(FMT_MOY, temps_java_avec_jit) + format(FMT_ET, et_java_avec_jit)
  partial_result = partial_result + format(FMT_MOY, temps_java_sans_jit) + format(FMT_ET, et_java_sans_jit)

  NB_THREADS.each do |nb_threads|
    temps_ppff, et_ppff = temps_moyen "./WordCount #{fichier_mots} #{nb_threads}"
    partial_result = partial_result + format(FMT_MOY, temps_ppff) + format(FMT_ET, et_ppff)
  end

  print partial_result
  print "\n"
  result = result + partial_result + "\n"
end

File.open(FICHIER, 'w') { |file| file.write(result) }
