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
  java = temps[3..5].map(&:to_f)
  ppff = temps[6..8].map(&:to_f)
  fastflow = temps[9..11].map(&:to_f)

  seq_ = Array.new(3)
  java_ = Array.new(3)
  ppff_ = Array.new(3)
  fastflow_ = Array.new(3)

  seq_ = accs( seq, seq )
  java_ = accs( java, seq )
  ppff_ = accs( ppff, seq )
  fastflow_ = accs( fastflow, seq )

  fich_sortie.puts format( "%9d ", taille) +
    format_acc( seq_ ) +
    format_acc( java_ ) +
    format_acc( ppff_ ) +
    format_acc( fastflow_ )
end

fich_sortie.close

if DEBUG
  puts "*** On a produit le fichier #{fich_accs}"
  puts %x{cat #{fich_accs}}
end
