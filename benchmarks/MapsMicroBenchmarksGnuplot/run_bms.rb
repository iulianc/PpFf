#!/usr/bin/env ruby

DEBUG = false #true
$server = 'japet'	#'java'
$nb_thtreads = 1
$fichier = 'test.txt'

if $server == 'java'
	$fichier = 'temps-java-maps.txt'
	$nb_thtreads = [1, 2, 4, 8, 16, 32]
else
	$fichier = "temps-japet-maps.txt"

	# Sur japet, plante si plus que 64..
	$nb_thtreads = [1, 2, 4, 8, 16, 32]
end

LARGEUR = 10

NB = 100000
NB_MAPS = 1


if DEBUG
  NB_REPETITIONS = 1
else
  NB_REPETITIONS = 10  # Temporaire... sinon 3? 5?
end

if DEBUG
  $nb_thtreads = [1, 2]
end

if DEBUG
  NB_GRANULARITY = [1, 2]
else
  NB_GRANULARITY = [4, 5] # ????
end


def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

print format("\#%#{LARGEUR}s",
            "NB THREADS")

NB_GRANULARITY.each do |nb_granularity|
  print format(" %#{LARGEUR}s", "PpFf#{nb_granularity}" )
end

NB_GRANULARITY.each do |nb_granularity|
  print format(" %#{LARGEUR}s", "FastFlow#{nb_granularity}" )
end

print "\n"

result = '';
$nb_thtreads.each do  |nb_threads|
  partial_result = " #{format("%#{LARGEUR}d", nb_threads)}"

 
  NB_GRANULARITY.each do |nb_granularity|
    temps_ppff = temps_moyen "./Maps #{NB} #{NB_MAPS} #{nb_granularity} #{nb_threads}"
    partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_ppff)}"
  end

  NB_GRANULARITY.each do |nb_granularity|
    temps_ppff = temps_moyen "./FastFlowMaps #{NB} #{NB_MAPS} #{nb_granularity} #{nb_threads}"
    partial_result = partial_result + " #{format("%#{LARGEUR}d", temps_ppff)}"
  end

  print partial_result
  print "\n"
  result = result + partial_result + "\n"
end

File.open($fichier, 'w') { |file| file.write(result) }
