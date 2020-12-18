#!/usr/bin/env ruby

NB_SLOTS = 25

vals = []
File.open( ARGV[0] ) do |fich|
  fich.each do |x|
    vals << x.to_i
  end
end

min = vals.min
max = vals.max
moy = vals.reduce(:+) / vals.size.to_f

ecart = (max - min) / NB_SLOTS

histo = Array.new(NB_SLOTS+1) { 0 }


std = 0.0
vals.each do |x|
  ind = ((x - min) / ecart.to_f).to_i
  histo[ind] += 1
  std += (x - moy) ** 2
end

histo.each do |n|
  puts "*" * n
end

std = Math.sqrt (std / (vals.size - 1))

puts "----------------"
puts min, max, moy, std
puts "----------------"

std1, std2, std3 = 0, 0, 0

vals.each do |x|
  std1 += 1 if (x - moy).abs < std
  std2 += 1 if (x - moy).abs < 2 * std
  std3 += 1 if (x - moy).abs < 3 * std
end

std1 = 100.0 * std1 / vals.size
std2 = 100.0 * std2 / vals.size
std3 = 100.0 * std3 / vals.size
puts std1, std2, std3
