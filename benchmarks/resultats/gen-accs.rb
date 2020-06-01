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
  pgms = []
  temps.shift 3
  while !temps.empty?
    pgms << temps[0..2].map(&:to_f)
    temps.shift 3
  end

  seq_ = accs( seq, seq )
  pgms_ = pgms.map { |pgm| accs(pgm, seq) }

  fich_sortie.puts format( "%9d ", taille) +
    format_acc( seq_ )  +
    (pgms_.map { |pgm| format_acc pgm }).join
end

fich_sortie.close

if DEBUG
  puts "*** On a produit le fichier #{fich_accs}"
  puts %x{cat #{fich_accs}}
end
