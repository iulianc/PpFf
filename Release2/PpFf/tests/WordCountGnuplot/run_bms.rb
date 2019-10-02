#!/usr/bin/env ruby

DEBUG = false #true
$server = 'japet' #'java'
$nb_thtreads = 1
$fichier = 'test.txt'

if $server == 'java'
	$fichier = 'temps-java-wc.txt'
	$nb_thtreads = [1, 2]
else
	$fichier = "temps-japet-wc.txt"

	# Sur japet, plante si plus que 64..
	$nb_thtreads = [1, 2, 4, 8, 16]
end

LARGEUR = 8

if DEBUG
  NB_REPETITIONS = 1
else
  NB_REPETITIONS = 10  # Temporaire... sinon 3? 5?
end

if DEBUG
  NB_MOTS = [10, 131, 377, 3805, 7610, 78792, 167941] # Pour debogage du script.
else
  NB_MOTS = [78792, 167941, 281307, 482636, 752856, 1639684, 2137758, 2614743]
end

if DEBUG
  NB_THREADS = [1, 2]
end


def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

print format("\#%#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s",
            "N", "Java+", "Java-")

$nb_thtreads.each do |nb_threads|
  print format(" %#{LARGEUR}s", "PpFf#{nb_threads}" )
end
print "\n"

result = '';
NB_MOTS.each do  |nb_mots|
  partial_result = " #{format("%#{LARGEUR}d", nb_mots)}"

  fichier_mots = "testdata/#{nb_mots}Words.txt"

  temps_java_avec_jit = temps_moyen "java -cp . WordCount '#{fichier_mots}'"
  temps_java_sans_jit = temps_moyen "java -Djava.compiler=NONE -cp . WordCount '#{fichier_mots}'"

  partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_java_avec_jit)}"
  partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_java_sans_jit)}"
  
  $nb_thtreads.each do |nb_threads|
    temps_ppff = temps_moyen "./WordCount #{fichier_mots} #{nb_threads}"
    partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_ppff)}"
  end

  print partial_result
  print "\n"
  result = result + partial_result + "\n"
end

File.open($fichier, 'w') { |file| file.write(result) }
