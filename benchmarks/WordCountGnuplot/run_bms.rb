#!/usr/bin/env ruby

DEBUG = true

######################################################
# Configuration des parametres d'execution
######################################################

PGM = ARGV[0]

# Pour utiliser facilement sur diverses machines, dont MacBook, Linux, etc.
SERVER = ENV['HOST']

FICHIER_INFOS = "graphes/infos-#{SERVER}-#{PGM}.txt"
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

# On charge les parametres specifiques a cette serie de benchmarks.
require "./#{PGM}-bm-config.rb"


######################################################
# Fonctions auxiliaires pour les executions et production des temps et
# debits.
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

######################################################
# Constantes et fonctions auxiliaires pour la sortie.
######################################################
LARGEUR = 8

def imprimer_en_tete( pgms )
  print format("\#%#{LARGEUR}s", "N" )

  pgms.each do |_, label|
    print format(" %#{3*LARGEUR-1}s", label )
  end
  print "\n"
end

def formater_temps( moy, min, max )
  fmt = "%#{LARGEUR}.1f"

  format(fmt, moy) + format(fmt, min) + format(fmt, max)
end

######################################################
# Le programme principal.
######################################################

if DEBUG
  puts "*** Va creer les fichiers #{FICHIER_TEMPS}, #{FICHIER_DEBITS} et #{FICHIER_INFOS} (avec au plus #{MAX_THREADS} threads)"
end

imprimer_en_tete( PGMS )

res_temps = '';
res_debits = '';
FICHIERS_DONNEES.each do  |fichier, nb_items|
  fmt = " %#{LARGEUR}d"
  ligne_temps = format(fmt, nb_items)
  ligne_debits = format(fmt, nb_items)

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


# On cree les fichiers de sortie.
ligne_xtics = FICHIERS_DONNEES.map { |_, nb| "#{nb}" }.join( ", " )
ligne_labels = PGMS.map { |_, label| "#{label} " }.join

File.open(FICHIER_INFOS, 'w') do |file|
  file.write(ligne_xtics + "\n")
  file.write(ligne_labels + "\n")
end

File.open(FICHIER_TEMPS, 'w') do |file|
  file.write(res_temps)
end
File.open(FICHIER_DEBITS, 'w') do |file|
  file.write(res_debits)
end
