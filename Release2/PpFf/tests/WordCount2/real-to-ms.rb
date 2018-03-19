#!/usr/bin/env ruby

temps = ARGV[0]

/(\d+)m(\d+)[.](\d+)s/ =~ temps

m, s, ms = $1.to_i, $2.to_i, $3.to_i


ms = m * 60 * 1000 * s * 1000 + ms

puts "Temps sh  : #{ms} ms"


