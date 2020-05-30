#!/usr/bin/env ruby

DEBUG = false

def format_acc( accs )
  format( "%7.2f %7.2f %7.2f", accs[0], accs[1], accs[2] )
end

def accs( temps, seqs )
  temps_moy, temps_min, temps_max = temps
  seq_moy, seq_min, seq_max = seqs

  acc_moy = seq_moy / temps_moy
  acc_min = seq_min / temps_max
  acc_max = seq_max / temps_min

  [acc_moy, acc_min, acc_max]
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

fich_accs = FICH.gsub('temps', 'accs')
fich_sortie = File.open( fich_accs, "w" )

File.new(FICH, chomp: true).readlines.each do |ligne|
  taille, *temps = ligne.split

  seq = temps[0..2].map(&:to_f)
  pgm1 = temps[3..5].map(&:to_f)
  pgm2 = temps[6..8].map(&:to_f)
  pgm3 = temps[9..11].map(&:to_f)

  seq_ = Array.new(3)
  pgm1_ = Array.new(3)
  pgm2_ = Array.new(3)
  pgm3_ = Array.new(3)

  seq_ = accs( seq, seq )
  pgm1_ = accs( pgm1, seq )
  pgm2_ = accs( pgm2, seq )
  pgm3_ = accs( pgm3, seq )

  fich_sortie.puts format( "%9d ", taille) +
    format_acc( seq_ )  +
    format_acc( pgm1_ ) +
    format_acc( pgm2_ ) +
    format_acc( pgm3_ )
end

fich_sortie.close

if DEBUG
  puts "*** On a produit le fichier #{fich_accs}"
  puts %x{cat #{fich_accs}}
end
