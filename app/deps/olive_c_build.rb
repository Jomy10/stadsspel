cmd :build_olive do
  call :build_olive__mkdirs
  call :build_olive__include
  call :build_olive__objs
  call :build_olive__lib
end

cmd :build_olive__mkdirs do
  obj_out = File.join(OBJ_OUT, "olive")
  if !Dir.exist?(obj_out) then Dir.mkdir(obj_out) end
end

cmd :build_olive__objs, each("deps/olive.c/olive.c") do
  sh %(#{CC} -c #{CFLAGS} #{PLATFORM_FLAGS} ) +
    %(-DOLIVEC_IMPLEMENTATION= ) +
    %(#{$file} ) +
    %(-o #{File.join(OBJ_OUT, "olive", $file.name + ".o")})
end

cmd :build_olive__lib, all(File.join(OBJ_OUT, "olive", "*.o")) do
  sh %(#{AR} rc #{File.join(LIB_OUT, "libolive.a")} #{$files})
end

cmd :build_olive__include do
  include_dir = File.join(INCLUDE_OUT, "olive.c")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("deps", "olive.c", "olive.c"))} #{include_dir}) end
end
