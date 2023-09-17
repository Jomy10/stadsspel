require 'colorize'

colorize_list = ["Unable to open", "stadsspel"]
colors = [:red, :cyan]

while (l = gets) do
  str = l
  colorize_list.each_with_index do |item, i|
    if l.downcase.include? item.downcase
      idx = l.downcase.index item.downcase
      str =
        l[0...idx] +
        l[idx...(idx + item.size)].colorize(colors[i]) +
        l[(idx + item.size)...l.size]
    end
  end
  print str
end

