#!/usr/bin/env ruby

DEBUG = true

if DEBUG
  NB_REPETITIONS = 5
  #NB_MOTS = [377, 3805]
  #NB_MOTS = [377, 3805, 7610]
  NB_MOTS = [78792, 167941, 281307, 482636]
else
  NB_REPETITIONS = 10
  NB_MOTS = [78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743]
end

# Pour utiliser facilement sur diverses machines, dont MacBook, Linux.
SERVER = ENV['HOST']
FICHIER_TEMPS = "graphes/temps-#{SERVER}-wc.txt"
FICHIER_DEBITS = "graphes/debits-#{SERVER}-wc.txt"

# Pour le nombre maximum de threads, on utilise un petit nombre de
# processeurs qui depend de la machine.
max_threads = nil
if SERVER == 'japet'
  max_threads = 8
elsif SERVER == 'java'
  max_threads = 4
elsif SERVER == 'c34581'
  max_threads = 4
elsif SERVER == 'MacOS'
  max_threads = 4
else
  max_threads = %x{nproc}.chomp.to_i / 2
end

NB_THREADS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
             .take_while { |n| n <= max_threads}

LARGEUR = 8

FMT = "%#{LARGEUR}.1f"

######################################################
# Fonctions auxiliaires
######################################################
def generer_les_temps( cmd )
  les_temps = []

  NB_REPETITIONS.times do
    les_temps << %x{#{cmd}}.chomp.to_f
  end

  les_temps
end

def temps_moyen( les_temps )
  temps_tot = 0.0
  le_min = le_max = nil

  les_temps.each do |temps|
    temps_tot += temps
    le_min = le_min ? [temps, le_min].min : temps
    le_max = le_max ? [temps, le_max].max : temps
  end

  moy = (temps_tot / NB_REPETITIONS).round(1)

  ecart_type = (Math.sqrt (les_temps.reduce(0.0) { |s, x| (x - moy) * (x - moy) } / (NB_REPETITIONS - 1))).round(1)

  [moy, le_min, le_max]
end

def imprimer_en_tete
  print format("\#%#{LARGEUR}s %#{3*LARGEUR-1}s %#{3*LARGEUR-1}s %#{3*LARGEUR-1}s",
               "N", "Java+", "Java-", "Java*")

  NB_THREADS.each do |nb_threads|
    print format(" %#{3*LARGEUR-1}s", "PpFf#{nb_threads}" )
  end
  print "\n"
end

def formater_temps( moy, min, max )
  format(FMT, moy) + format(FMT, min) + format(FMT, max)
end

def debits_moyen( les_temps, nb_mots )
  debit_tot = 0.0
  le_min = le_max = nil

  les_temps.each do |temps|
    debit = nb_mots / temps

    debit_tot += debit
    le_min = le_min ? [debit, le_min].min : debit
    le_max = le_max ? [debit, le_max].max : debit
  end

  moy = (debit_tot / NB_REPETITIONS).round(1)

  [moy, le_min, le_max]
end

######################################################
######################################################

if DEBUG
  puts "*** Va creer les fichiers #{FICHIER_TEMPS} et #{FICHIER_DEBITS} (avec au plus #{max_threads} threads)"
end

imprimer_en_tete

res_temps = '';
res_debits = '';
NB_MOTS.each do  |nb_mots|
  fichier_mots = "testdata/#{nb_mots}Words.txt"

  ligne_temps = " #{format("%#{LARGEUR}d", nb_mots)}"
  ligne_debits = " #{format("%#{LARGEUR}d", nb_mots)}"

  les_temps = generer_les_temps( "java -cp . WordCount '#{fichier_mots}'" )
  les_temps_java_avec_jit = temps_moyen( les_temps )
  ligne_temps << formater_temps( *les_temps_java_avec_jit )
  les_debits_java_avec_jit = debits_moyen( les_temps, nb_mots )
  ligne_debits << formater_temps( *les_debits_java_avec_jit )

  les_temps = generer_les_temps( "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}'" )
  les_temps_java_sans_jit = temps_moyen( les_temps )
  ligne_temps << formater_temps( *les_temps_java_sans_jit )
  les_debits_java_sans_jit = debits_moyen( les_temps, nb_mots )
  ligne_debits << formater_temps( *les_debits_java_sans_jit )

  les_temps = generer_les_temps( "java -cp . WordCountWarmup '#{fichier_mots}'" )
  les_temps_java_avec_jit = temps_moyen( les_temps )
  ligne_temps << formater_temps( *les_temps_java_avec_jit )
  les_debits_java_avec_jit = debits_moyen( les_temps, nb_mots )
  ligne_debits << formater_temps( *les_debits_java_avec_jit )

  NB_THREADS.each do |nb_threads|
    les_temps = generer_les_temps( "./WordCount #{fichier_mots} #{nb_threads}" )
    les_temps_ppff = temps_moyen( les_temps )
    ligne_temps << formater_temps( *les_temps_ppff )
    les_debits_ppff = debits_moyen( les_temps, nb_mots )
    ligne_debits << formater_temps( *les_debits_ppff )
  end

  print ligne_temps + "\n"
  print ligne_debits + "\n"

  res_temps << ligne_temps << "\n"
  res_debits << ligne_debits << "\n"
end

File.open(FICHIER_TEMPS, 'w') { |file| file.write(res_temps) }
File.open(FICHIER_DEBITS, 'w') { |file| file.write(res_debits) }
