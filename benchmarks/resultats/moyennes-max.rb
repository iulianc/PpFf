#!/usr/bin/env ruby

FICH = ARGV[0]

lignes = IO.readlines(FICH)

lignes.each do |ligne|
  nb, *vals = ligne.split

  moys = []
  while !vals.empty?
    moy, min, max = vals.shift(3)
    moys << moy.to_f
  end

  max = moys.max
  max2 = moys.reject { |m| (m - max).abs < 0.01 }.max
  moys.each do |moy|
    if (moy - max).abs < 0.01
      printf "%7.1f*   ", moy
    elsif (moy - max2).abs < 0.01
      printf "%7.1f**  ", moy
    else
      printf "%7.1f    ", moy
    end
  end
  puts
end
