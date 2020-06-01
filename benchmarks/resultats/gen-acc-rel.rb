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

fich_accs = FICH.gsub('temps', 'accs-rel')
fich_sortie = File.open( fich_accs, "w" )

File.new(FICH, chomp: true).readlines.each do |ligne|
  taille, *temps = ligne.split

  seq = temps.shift(3).map(&:to_f)
  java = temps.shift(3).map(&:to_f)

  java_plus = temps.shift(3).map(&:to_f)
  ppff = temps.shift(3).map(&:to_f)
  fastflow = temps.shift(3).map(&:to_f)

  seq_ = accs( seq, seq )
  java_ = accs( java, java )
  java_plus_ = accs( java_plus, java )
  ppff_ = accs( ppff, seq)
  fastflow_ = accs( fastflow, seq)

  pgms_ = [seq_, java_, java_plus_, ppff_, fastflow_]

  fich_sortie.puts format( "%9d ", taille) +
    pgms_.map { |pgm| format_acc( pgm ) }.join
end

fich_sortie.close

if DEBUG
  puts "*** On a produit le fichier #{fich_accs}"
  puts %x{cat #{fich_accs}}
end
