#!/usr/bin/env ruby

DEBUG = false

def format_ic( ics )
  format( "%8.1f %8.1f %8.1f", ics[0], ics[1], ics[2] )
end

=begin
  ecart_type = Math.sqrt(variance)

  s_m = ecart_type / Math.sqrt(nb_repetitions)

  t_value = case nb_repetitions
            when 30 then 2.045 # DF = 29
            when 40 then 2.023 # DF = 39
            when 50 then 2.010 # DF = 49
            else 2.0
            end

  le_min = moy - t_value * s_m
  le_max = moy + t_value * s_m
=end

def ics( temps )
  temps_moy, temps_min, temps_max = temps

  ecart = temps_moy - temps_min

  ecart_type = Math.sqrt(40) * ecart / 2.023


  ecart_type_2 = 2.0 * ecart_type
  ic_min = temps_moy - ecart_type_2
  ic_max = temps_moy + ecart_type_2

  # Verification
  #ic_min = temps_moy - ((temps_moy - ic_min) / 2.0) / Math.sqrt(40) * 2.023
  #ic_max = temps_moy + ((ic_max - temps_moy) / 2.0) / Math.sqrt(40) * 2.023

  [temps_moy, ic_min, ic_max]
end

if ARGV.empty?
  puts "*** Usage:"
  puts "    #{__FILE__} nomDeFichierTemps"
  exit -1
end

FICH = ARGV[0]

if DEBUG
  puts "*** On traite le fichier #{FICH}"
  puts %x{cat #{FICH}} if DEBUG
  puts
end

fich_ics = FICH.gsub('txt', 'txt.ic')
fich_sortie = File.open( fich_ics, "w" )

File.new(FICH, chomp: true).readlines.each do |ligne|
  taille, *temps = ligne.split

  seq = temps[0..2].map(&:to_f)
  java = temps[3..5].map(&:to_f)
  ppff = temps[6..8].map(&:to_f)
  fastflow = temps[9..11].map(&:to_f)

  seq_ = Array.new(3)
  java_ = Array.new(3)
  ppff_ = Array.new(3)
  fastflow_ = Array.new(3)

  seq_ = ics( seq )
  java_ = ics( java )
  ppff_ = ics( ppff )
  fastflow_ = ics( fastflow )

  fich_sortie.puts format( "%9d ", taille) +
    format_ic( seq_ ) +
    format_ic( java_ ) +
    format_ic( ppff_ ) +
    format_ic( fastflow_ )
end

fich_sortie.close

if DEBUG
  puts "*** On a produit le fichier #{fich_ics}"
  puts %x{cat #{fich_ics}}
  puts
  puts %x{wdiff #{FICH} #{fich_ics}}
end
