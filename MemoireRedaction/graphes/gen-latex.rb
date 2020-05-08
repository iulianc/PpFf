#!/usr/bin/env ruby

DEBUG = false

pgm = ARGV.shift
sorte = ARGV.shift
machine = ARGV.shift
nb_repetitions = ARGV.shift.to_i
id_fichier = ARGV.shift
sous_titre = ARGV.shift
avec_sans_log = ARGV.shift

nom_graphe = "#{pgm}-graphe#{avec_sans_log}-#{sorte}-#{machine}-#{nb_repetitions}#{id_fichier}"

fichier_graphe = "finaux/#{nom_graphe}.png"

unless File.exist?(fichier_graphe)
  puts "*** Erreur: Le fichier #{fichier_graphe} n'existe pas!"
  exit -1
end

fichier_tex = "finaux/#{nom_graphe}.tex"

puts "On cree le fichier #{fichier_tex}" if DEBUG

machines = {
  "japet" => "M1",
  "java" => "M2",
  "c34581" => "M3",
  "MacOS" => "M4",
}

num_machine = machines[machine] || "M{?}"

case sorte
when "temps"
  shortcaption = "Les temps d'ex\\'ecution pour \\TT{#{pgm}} sur la machine \\#{num_machine}."
when "debits"
  shortcaption = "Les d\\'ebits pour \\TT{#{pgm}} sur la machine \\#{num_machine}."
else
  puts "*** Erreur: La sorte #{sorte} n'est pas valide!"
  exit -1
end

caption = shortcaption # POUR L'INSTANT... IL FAUDRA RAFFINER POUR
                       # INDIQUER L'UTLISATION DE L'ECHELLE LOG,
                       # L'UNITE DE MESURE, ETC.

File.open( fichier_tex, mode = "w" ) do |f|
  f.puts <<-EOF
\\begin{figure}[htbp]
  \\centering
  \\includegraphics[width=1.0\\textwidth]{graphes/#{fichier_graphe}}
  \\caption[#{shortcaption}]{#{caption}}
  \\label{#{nom_graphe}.fig}
\\end{figure}
EOF
end

puts %x{cat #{fichier_tex}} if DEBUG
