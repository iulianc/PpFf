#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

DEBUG = true

######################################################
# Configuration des parametres d'execution: arguments du programme et
# variable d'environnement.
######################################################

if ARGV.empty?
  puts "*** #{__FILE__} NomDuProgramme NumExperience"
  exit -1
end

PGM = ARGV[0]
NUM_EXPERIENCE = ARGV[1].to_i || 0

# Pour utiliser facilement sur diverses machines, dont MacBook, Linux, etc.
SERVER = ENV['HOST'] || %x{hostname}.chomp


######################################################
# Classes pour definir les divers programmes et experiences possibles.
######################################################

$experiences = {}
$programs = {}

class Program
  def self.define( nom_pgm, commande )
    $programs[nom_pgm] = commande
  end
end

class Experience
  attr_reader :machines, :nb_items, :nb_repetitions

  def self.define( num_experience,
                   machines: nil,
                   nb_items:,
                   nb_repetitions:,
                   programs:
                   )

    if $experiences[num_experience]
      puts "*** Erreur: experience #{num_experience} definie de facon multiple"
      exit -1
    end

    $experiences[num_experience] = new( num_experience,
                                        machines,
                                        nb_items,
                                        nb_repetitions,
                                        programs )
  end

  def initialize( num_experience, machines, nb_items, nb_repetitions, programs )
    @num_experience = num_experience
    @machines = machines
    @nb_items = nb_items
    @nb_repetitions = nb_repetitions
    @programs = programs
  end

  def programs
    $programs.keys
      .select { |pgm_name| @programs.include?(pgm_name) }
      .map { |pgm_name| [$programs[pgm_name], pgm_name] }
  end
end


# On charge les parametres specifiques a cette serie de benchmarks/experiences.
require "./#{PGM}-bm-config.rb"

######################################################
# Fonctions auxiliaires pour les executions et production des temps et
# debits.
######################################################
def generer_les_temps( cmd, nb_repetitions )
  les_temps = []

  nb_repetitions.times do
    les_temps << %x{#{cmd}}.chomp.to_f
  end

  les_temps
end

def ic( les_valeurs, moy, nb_repetitions )
  variance = les_valeurs.reduce(0.0) { |s, x| s + (x - moy) * (x - moy) } / (nb_repetitions - 1)
  ecart_type = Math.sqrt(variance)

  le_min = moy - 2.0 * ecart_type
  le_max = moy + 2.0 * ecart_type

  [le_min, le_max]
end

def temps_moyen( les_temps, nb_repetitions )
  moy = les_temps.reduce(0.0, &:+) / nb_repetitions

  le_min, le_max = ic( les_temps, moy, nb_repetitions )

  [moy, le_min, le_max]
end

def debits_moyen( les_temps, nb_mots, nb_repetitions )
  les_debits = []
  debit_tot = 0.0
  les_temps.each do |temps|
    debit = nb_mots / temps
    debit_tot += debit
    les_debits << debit
  end

  moy = debit_tot / nb_repetitions
  le_min, le_max = ic( les_debits, moy, nb_repetitions )

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
# LE PROGRAMME PRINCIPAL.
######################################################

experience = $experiences[NUM_EXPERIENCE]

if experience.machines && !experience.machines.include?(SERVER)
  puts "*** Erreur: experience #{NUM_EXPERIENCE} pas definie sur la machine #{SERVER}"
  puts "    Definie sur #{experience.machines} seulement"
  exit -1
end

nb_items = experience.nb_items
nb_repetitions = experience.nb_repetitions

fichiers_donnees = nb_items.map { |nb| [fichier_de_donnees_pour(nb), nb] }

fichier_infos  = "resultats/#{PGM}-infos-#{SERVER}-#{NUM_EXPERIENCE}-#{nb_repetitions}.txt"
fichier_temps  = "resultats/#{PGM}-temps-#{SERVER}-#{NUM_EXPERIENCE}-#{nb_repetitions}.txt"
fichier_debits = "resultats/#{PGM}-debits-#{SERVER}-#{NUM_EXPERIENCE}-#{nb_repetitions}.txt"

programs = experience.programs


if DEBUG
  puts "*** Va creer les fichiers #{fichier_temps}, #{fichier_debits} et #{fichier_infos}"
end


# On verifie que tous les fichiers existent avant de lancer l'execution
erreurs = ''
fichiers_donnees.each do |fichier, nb_items|
  unless File.exist?(fichier)
    erreurs += "- Le fichier #{fichier} n'existe pas\n"
  end
end

unless erreurs.empty?
  puts "*** Erreur:\n" + erreurs
  exit -1
end

# Tout semble ok: on lance l'execution.
imprimer_en_tete( programs )

res_temps = '';
res_debits = '';
fichiers_donnees.each do  |fichier, nb_items|
  fmt = " %#{LARGEUR}d"
  ligne_temps = format(fmt, nb_items)
  ligne_debits = format(fmt, nb_items)

  programs.each do |cmd, _|
    les_temps = generer_les_temps( "#{cmd} '#{fichier}'", nb_repetitions )

    ligne_temps << formater_temps( *temps_moyen( les_temps, nb_repetitions ) )
    ligne_debits << formater_temps( *debits_moyen( les_temps, nb_items, nb_repetitions ) )
  end

  print ligne_temps + "\n"
  print ligne_debits + "\n"

  res_temps << ligne_temps << "\n"
  res_debits << ligne_debits << "\n"
end


# On cree les fichiers de sortie.
ligne_xtics = fichiers_donnees.map { |_, nb| "#{nb}" }.join( ", " )
ligne_labels = programs.map { |_, label| "#{label} " }.join

File.open(fichier_infos, 'w') do |file|
  file.write(ligne_xtics + "\n")
  file.write(ligne_labels + "\n")
end

File.open(fichier_temps, 'w') do |file|
  file.write(res_temps)
end
File.open(fichier_debits, 'w') do |file|
  file.write(res_debits)
end
