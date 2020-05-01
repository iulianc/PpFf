#!/usr/bin/env ruby

DEBUG = true

######################################################
# Configuration des parametres
######################################################

if DEBUG
  NB_REPETITIONS = 2
  NB_MOTS = [4 * 1024, 8 * 1024, 16 * 1024]
else
  NB_REPETITIONS = 10
  NB_MOTS = [4 * 1024, 8 * 1024, 16 * 1024]
end


PGM = 'StockPrice'

# Pour utiliser facilement sur diverses machines, dont MacBook, Linux.
SERVER = ENV['HOST']
FICHIER_PGMS = "graphes/pgms-#{SERVER}-#{PGM}.txt"
FICHIER_TEMPS = "graphes/temps-#{SERVER}-#{PGM}.txt"
FICHIER_DEBITS = "graphes/debits-#{SERVER}-#{PGM}.txt"

# Pour le nombre maximum de threads, on utilise un petit nombre de
# processeurs qui depend de la machine.
MAX_THREADS = case SERVER
              when 'java', 'c34581', 'MacOS'
                4
              when 'japet'
                8
              else
                %x{nproc}.chomp.to_i / 2
              end

NB_THREADS = [1, 2, 4, 8, 16, 32, 64].take_while { |n| n <= MAX_THREADS}


######################################################
# IMPORTANT Ce sont les elements ci-bas qu'il faut modifier si on veut
# ajouter d'autres programmes a benchmarker et/ou d'autres fichiers a
# traiter.
######################################################

# Les programmes a executer.
PGMS_JAVA =
  [ ["java -cp . #{PGM}", 'Java+'],
    ["java -Djava.compiler=NONE -cp . #{PGM}", 'Java-'] ]

PGMS_PPFF =
  NB_THREADS
  .map { |nb_threads| ["./#{PGM} #{nb_threads}", "PpFf-#{nb_threads}"] }

PGMS = PGMS_JAVA + PGMS_PPFF

# Les fichiers de donnees
FICHIERS_DONNEES =
  NB_MOTS.map { |nb| [ "../testdata/stock_options_#{nb/1024}K.txt", nb ] }


######################################################
# Constantes et fonctions auxiliaires
######################################################
LARGEUR = 8

FMT = "%#{LARGEUR}.1f"

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

  [moy, le_min, le_max]
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

def imprimer_en_tete( pgms )
  print format("\#%#{LARGEUR}s", "N" )

  pgms.each do |_, label|
    print format(" %#{3*LARGEUR-1}s", label )
  end
  print "\n"
end

def formater_temps( moy, min, max )
  format(FMT, moy) + format(FMT, min) + format(FMT, max)
end

######################################################
######################################################

if DEBUG
  puts "*** Va creer les fichiers #{FICHIER_TEMPS}, #{FICHIER_DEBITS} et #{FICHIER_PGMS} (avec au plus #{MAX_THREADS} threads)"
end

imprimer_en_tete( PGMS )

res_temps = '';
res_debits = '';
FICHIERS_DONNEES.each do  |fichier, nb_items|
  ligne_temps = " #{format("%#{LARGEUR}d", nb_items)}"
  ligne_debits = " #{format("%#{LARGEUR}d", nb_items)}"

  PGMS.each do |cmd, _|
    les_temps = generer_les_temps( "#{cmd} '#{fichier}'" )

    ligne_temps << formater_temps( *temps_moyen( les_temps ) )
    ligne_debits << formater_temps( *debits_moyen( les_temps, nb_items ) )
  end

  print ligne_temps + "\n"
  print ligne_debits + "\n"

  res_temps << ligne_temps << "\n"
  res_debits << ligne_debits << "\n"
end

ligne_pgms = PGMS.map { |_, label| "#{label} " }.join

File.open(FICHIER_PGMS, 'w') { |file| file.write(ligne_pgms) }
File.open(FICHIER_TEMPS, 'w') { |file| file.write(res_temps) }
File.open(FICHIER_DEBITS, 'w') { |file| file.write(res_debits) }
