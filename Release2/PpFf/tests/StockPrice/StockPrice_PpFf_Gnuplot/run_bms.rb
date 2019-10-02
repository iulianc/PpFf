#!/usr/bin/env ruby

$file_options="../testdata/stock_options_64K.txt"

DEBUG = false #true
$server = 'java'	#'japet'
$nb_thtreads = 1
$fichier = 'test.txt'

if $server == 'java'
	$fichier = 'temps-java-stockprice.txt'
	$nb_thtreads = [1, 2, 4, 8, 16]
else
	$fichier = "temps-japet-stockprice.txt"
	$nb_thtreads = [1, 2, 4, 8, 16, 32]
end

LARGEUR = 8


if DEBUG
  NB_REPETITIONS = 1
else
  NB_REPETITIONS = 10  # Temporaire... sinon 3? 5?
end

if DEBUG
  $nb_thtreads = [1, 2]
end

def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

print format("%#{LARGEUR}s %#{LARGEUR}s", "Java+", "Java-")

$nb_thtreads.each do  |nb_threads|
	print format(" %#{LARGEUR}s", "PpFf#{nb_threads}" )
end

print "\n"

result = '';
temps_java_avec_jit = temps_moyen "java -cp . StockPrice #{$file_options}"
temps_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . StockPrice #{$file_options}"

result = result + " #{format("%#{LARGEUR}d", temps_java_avec_jit)}"
result = result + " #{format("%#{LARGEUR}d", temps_java_sans_jit)}"

$nb_thtreads.each do  |nb_threads|
   temps_ppff = temps_moyen "./StockPrice #{$file_options} #{nb_threads}"
   result = result + " #{format("%#{LARGEUR}d", temps_ppff)}"
end

print result
print "\n"

File.open($fichier, 'w') { |file| file.write(result) }
