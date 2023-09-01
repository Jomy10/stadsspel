# Define some commands to build a library
# The :build_name command will create the output directory
# for the object files, compile the source code, package the
# object files into a static library and symlink the include
# path in the output directory
def def_lib(name, path, include_sub_dir: "include", warn: WARN, include: [], includes_overwrite: nil)
  cmd "build_#{name}".to_sym do
    call "build_#{name}__mkdirs".to_sym
    call "build_#{name}__source".to_sym
    call "build_#{name}__lib".to_sym
    call "build_#{name}__include".to_sym
  end

  cmd "build_#{name}__mkdirs".to_sym do
    obj_out = File.join(OBJ_OUT, name)
    if !Dir.exist?(obj_out) then Dir.mkdir(obj_out) end
  end

  cmd "build_#{name}__source".to_sym, each(File.join(path, "**/*.c")) do
    sh "#{CC} -c #{warn} #{CFLAGS} #{PLATFORM_FLAGS} " +
      # Include directories
      "#{include.map { |i| "-I#{i}"}.join " "} " +
      "-I#{File.join(path, include_sub_dir)} " +
      # Source
      "#{$file} " +
      # Output
      "-o #{File.join(OBJ_OUT, name, $file.name + ".o")} "
      # Libraries
      # "#{link.size > 0 ? "-L#{LIB_OUT}" : ""} #{link.map { |d| "-l#{d}" }.join " "} "
  end

  cmd "build_#{name}__lib".to_sym, all(File.join(OBJ_OUT, name, "*.o")) do
    sh %(#{AR} rc #{File.join(LIB_OUT, "lib#{name}.a")} #{$files})
  end

  cmd "build_#{name}__include".to_sym do
    if includes_overwrite != nil
      includes_overwrite.each do |incl_o|
        link_incl_path = File.join(INCLUDE_OUT, File.basename(incl_o))
        if File.exist? (link_incl_path) then next end
        sh %(ln -s #{File.absolute_path(File.join(path, incl_o))} #{link_incl_path})
      end
      next # don't execute the default branch
    end

    include_dir = File.join(INCLUDE_OUT, name)
    if File.exist?(include_dir) then next end
    sh %(ln -s #{File.absolute_path(File.join(path, include_sub_dir))} #{include_dir})
  end
end
