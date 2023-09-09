cmd :build_renderer do
  call :build_renderer_frontend
  call :build_renderer__objs
  call :build_renderer__lib
end

backends_path = File.join("common", "render_backends")
backend_files = nil
extra_flags = ""
case PLATFORM
when 'ios', 'iossim'
  backend_files = Dir[File.join(backends_path, "CoreGraphics", "*")]
  backend_files.push(*Dir[File.join(backends_path, "common", "truetype", "*.c")])
  extra_flags << "-fmodules -fobjc-arc"
else
  backend_files = Dir[File.join(backends_path, "SDL", "*.c")]
  backend_files.push(*Dir[File.join(backends_path, "common", "truetype", "*.c")])
end

cmd :build_renderer__objs, each(backend_files) do
  obj_out = File.join(OBJ_OUT, "renderer")
  if !Dir.exist?(obj_out) then Dir.mkdir(obj_out) end
  sh %(#{CC} -c #{extra_flags} #{WARN} #{CFLAGS} #{PLATFORM_FLAGS} ) +
    %(-I#{INCLUDE_OUT} ) +
    %(-I/usr/local/include ) +
    "#{$file} " +
    %(-o #{File.join(obj_out, $file.name + ".o")})
end

cmd :build_renderer__lib, all(File.join(OBJ_OUT, "renderer", "*.o")) do
  # TODO
  sh %(#{AR} rc #{File.join(LIB_OUT, "librenderer.a")} #{$files})
  #sh %(#{CC} #{extra_flags} #{WARN} #{CFLAGS} #{PLATFORM_FLAGS} #{$files} -o #{File.join(LIB_OUT, "librenderer.a")})
end

cmd :build_renderer_frontend do
  include_dir = File.join(INCLUDE_OUT, "renderer")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("common", "render_frontend"))} #{include_dir}) end
end
