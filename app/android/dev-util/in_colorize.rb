#!/usr/bin/env ruby
require 'colorize'

while (l = gets) do
  if l.start_with? "+"
    print l.light_black
  else
    print l
  end
end

