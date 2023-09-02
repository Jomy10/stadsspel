cmd :build_renderer do
  call :build_renderer_frontend

  backend = File.join("common", "render_backends")
  extra_flags = ""
  case PLATFORM
  when 'ios', 'iossim'
    backend = File.join backend, "CoreGraphics", "*"
    extra_flags << "-fmodules -fobjc-arc"
  else
    backend = File.join backend, "SDL", "*.c"
  end

  sh %(#{CC} -c #{extra_flags} #{WARN} #{CFLAGS} #{PLATFORM_FLAGS} ) +
    %(-I#{INCLUDE_OUT} ) +
    %(-I/usr/local/include ) +
    %(#{Dir[backend].join " "} ) +
    %(-o #{File.join(LIB_OUT, "librenderer.a")})
end

cmd :build_renderer_frontend do
  include_dir = File.join(INCLUDE_OUT, "renderer")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("common", "render_frontend"))} #{include_dir}) end
end
