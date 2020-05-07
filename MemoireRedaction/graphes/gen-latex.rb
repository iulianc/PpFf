#!/usr/bin/env ruby

puts "ARGV = #{ARGV}"

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

puts "On cree le fichier #{fichier_tex}"

machines = {
  "japet" => "M1",
  "java" => "M2",
  "c34581" => "M3"
}

case sorte
when "temps"
  caption = "Les temps d'ex\''ecution pour \\TT{#{pgm}} sur la machine \\#{machines[machine]}."
when "debits"
  caption = "Les d\'ebits pour \\TT{#{pgm}} sur la machine \\#{machines[machine]}."
else
  puts "*** Erreur: La sorte #{sorte} n'est pas valide!"
  exit -1
end

File.open( fichier_tex, mode = "w" ) do |f|
  f.puts <<-EOF
\\begin{figure}[htbp]
  \\centering
  \\includegraphics[width=1.0\\textwidth]{graphes/#{fichier_graphe}}
  \\caption[Les temps d'ex\\'ecution pour \\TT{WordCount} sur la machine \\M2.]{Les temps d'ex\\'ecution pour \\TT{WordCount} sur la machine \\M2.}
  \\label{#{nom_graphe}.fig}
\\end{figure}
EOF
end

puts %x{cat #{fichier_tex}}
