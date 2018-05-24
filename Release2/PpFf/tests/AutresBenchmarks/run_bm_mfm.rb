#!/usr/bin/env ruby

LARGEUR = 8

# Sur japet, plante si plus que 64, donc au plus 32... fois 2 -- voir plus bas.
MAX_NB_THREADS = [`cat /proc/cpuinfo | grep processor | wc -l`.to_i, 32].min

NB_REPETITIONS = 3

taille = ARGV[0] ? ARGV[0].to_i : 10

puts "# N = #{taille}"
puts format("\#   %#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s %#{LARGEUR}s",
            "Seq", "JavaSeq", "JavaPar", "FastFlow", "PpFf")


def temps_moyen( cmd )
  temps_tot = 0.0

  NB_REPETITIONS.times do
    temps = %x{#{cmd}}.chomp.to_f
    temps_tot += temps
  end

  (temps_tot / NB_REPETITIONS).to_i
end

temps_seq = temps_moyen "./MapFilterMap #{taille} 0"
temps_java_seq = temps_moyen "java MapFilterMapJava #{taille} 0"
temps_java_par = temps_moyen "java MapFilterMapJava #{taille} 1"
temps_fastflow = temps_moyen "./MapFilterMapFastFlow #{taille}"

nb_threads = 1
while nb_threads <= 2 * MAX_NB_THREADS do
  print "#{format("%3d", nb_threads)}"
  print " #{format("%#{LARGEUR}d", temps_seq)}"
  print " #{format("%#{LARGEUR}d", temps_java_seq)}"
  print " #{format("%#{LARGEUR}d", temps_java_par)}"
  print " #{format("%#{LARGEUR}d", temps_fastflow)}"

  temps_par = temps_moyen "./MapFilterMap #{taille} #{nb_threads}"
  print " #{format("%#{LARGEUR}d", temps_par)}"
  print "\n"
  nb_threads *= 2
end