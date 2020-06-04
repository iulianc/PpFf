#!/usr/bin/env ruby

FICH = ARGV[0]

puts lignes = IO.readlines(FICH)

lignes.each do |ligne|
  nb, *vals = ligne.split

  moys = []
  while !vals.empty?
    moy, min, max = vals.shift(3)
    moys << moy.to_f
  end

  min = moys.min
  min2 = moys.reject { |m| (m - min).abs < 0.01 }.min
  moys.each do |moy|
    if (moy - min).abs < 0.01
      printf "%7.1f*   ", moy
    elsif (moy - min2).abs < 0.01
      printf "%7.1f**  ", moy
    else
      printf "%7.1f    ", moy
    end
  end
  puts
end
