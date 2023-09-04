cmd :build_macos_main, all(["macOS/macosmain.c", "common/**/*"]) do
  puts "Building libraries".yellow
  call :build

  puts "Building app".yellow
  sh %(#{CC} #{WARN} #{CFLAGS} #{PLATFORM_FLAGS} ) +
    %(-I#{INCLUDE_OUT} ) +
    %(-I/usr/local/include ) +
    %(-L#{LIB_OUT} -L/usr/local/lib ) +
    ["hashmap", "SDL3", "render_objects", "map_data", "renderer", "vec", "uirenderer", "o5mreader", "mercator", "app"].map { |l| "-l#{l} " }.join(" ") +
    %(#{File.join("macOS", "macosmain.c")} ) +
    %(-o #{File.join(OUT, "app")})
end
