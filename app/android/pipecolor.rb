#!/usr/bin/env ruby
require 'colorize'

while (l = gets) do
  l = l.sub("\n", "")
  if l.start_with? "+"
    puts l.light_black
  else
    puts l
  end
end
