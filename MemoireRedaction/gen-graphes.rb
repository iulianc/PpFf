#!/usr/bin/env ruby

DEBUG = false

FICHIERS = [
            'experiences.tex',
            'appendice-wordcount.tex',
            'appendice-stockprice.tex',
            'appendice-java.tex',
            'appendice-ppff-merged.tex',
           ]

SCRIPT_TMP = 'gen-g.sh'


def split_arguments_sous_titre( ligne )
  ligne
    .split( '}{' )
    .map { |x| x.gsub('{', '') }
    .map { |x| x.gsub('}', '') }
end

graphes = FICHIERS
  .map { |f| %x{grep '^[^%]*\\graphe{.*}' #{f}} }
  .flat_map { |l| l.split("\n") }
  .map(&:chomp)
  .reject(&:empty?)
  .map { |x| x.gsub('\\graphe', '') }
  .map { |x| split_arguments_sous_titre(x) }


graphes_h = FICHIERS
  .map { |f| %x{grep '^[^%]*\\grapheH{.*}' #{f}} }
  .flat_map { |l| l.split("\n") }
  .map(&:chomp)
  .reject(&:empty?)
  .map { |x| x.gsub('\\grapheH', '') }
  .map { |x| split_arguments_sous_titre(x) }


script = "\#!\n" + "cd graphes\n"

graphes.each do |g, sous_titre|
  avec_log = '_'
  if g =~ /log-/
    avec_log = 'log'
    g.gsub!( 'log-', '' )
  end
  pgm, sorte, machine, num_experience, nb_repetitions, champs = g.split('-')

  if champs
    champs = champs.split('').join(',')
  else
    champs = '*'
  end

  cmd = "./plot.sh '#{pgm}' '#{sorte}' '#{machine}' '#{num_experience}' '#{nb_repetitions}' '#{champs}' '#{avec_log}' '#{sous_titre}' '1'"
  script += "echo \"** On execute <<#{cmd}>>\"\n" + cmd + "\n"
end

graphes_h.each do |g, sous_titre|
  avec_log = '_'
  if g =~ /log-/
    avec_log = 'log'
    g.gsub!( 'log-', '' )
  end
  pgm, sorte, machine, num_experience, nb_repetitions, champs = g.split('-')

  if champs
    champs = champs.split('').join(',')
  else
    champs = '*'
  end

  cmd = "./plot.sh '#{pgm}' '#{sorte}' '#{machine}' '#{num_experience}' '#{nb_repetitions}' '#{champs}' '#{avec_log}' '#{sous_titre}' '2'"
  script += "echo \"** On execute <<#{cmd}>>\"\n" + cmd + "\n"
end

script += "cd .."

File.open(SCRIPT_TMP, 'w') do |f|
  f.puts script
end

puts %x{cat #{SCRIPT_TMP}} if DEBUG

puts %x{bash #{SCRIPT_TMP}}

%x{rm -rf #{SCRIPT_TMP}} unless DEBUG
