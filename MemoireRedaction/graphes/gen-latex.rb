#!/usr/bin/env ruby

DEBUG = true

pgm = ARGV.shift
sorte = ARGV.shift
machine = ARGV.shift
num_experience = ARGV.shift
nb_repetitions = ARGV.shift.to_i
id_fichier = ARGV.shift
sous_titre = ARGV.shift
avec_sans_log = ARGV.shift

nom_graphe = "#{pgm}-graphe#{avec_sans_log}-#{sorte}-#{machine}-#{num_experience}-#{nb_repetitions}#{id_fichier}"

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

info_log = (avec_sans_log == "-log") ? " (avec \\'echelle logarithmique)" : ""

les_versions = sous_titre[0].downcase + sous_titre[1..-1]
#les_versions[0] = les_versions[0].downcase

if sous_titre =~ /Tous les programmes/
  les_versions = "tous les programmes"
elsif sous_titre =~ /^Programmes /
  les_versions = "les programmes " + sous_titre.gsub("Programmes ", "")
else
  les_versions = sous_titre
end

item, de = case pgm
       when 'WordCount' then ['mot', 'de ']
       when 'StockPrice' then ['transaction',  'de ']
       else ['item', 'd\'']
       end

info_moyenne = "obtenus en prenant la moyenne de~#{nb_repetitions} ex\\'ecutions"

case sorte
when "temps"
  info_unites = "Les temps sont en millisecondes (ms)" + info_log

  caption = "Les temps d'ex\\'ecution pour \\TT{#{pgm}} sur la machine \\#{num_machine}, pour #{les_versions}. " +
    info_unites +
    ', ' +
    info_moyenne +
    '.'
when "debits"
  info_unites = "Les d\\'ebits sont en milliers #{de}#{item}s par seconde (K-#{item}s/s)" + info_log

  caption = "Les d\\'ebits pour \\TT{#{pgm}} sur la machine \\#{num_machine}, pour #{les_versions}. " +
    info_unites +
    ', ' +
    info_moyenne +
    '.'
end

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
