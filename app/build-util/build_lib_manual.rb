def def_lib_man(name, csources: [], headers: [], warn: WARN, include: [])
  cmd "build_#{name}".to_sym do
    cmd "build_#{name}__mkdirs".to_sym
    cmd "build_#{name}__source".to_sym
    cmd "build_#{name}__lib".to_sym
    cmd "build_#{name}__include".to_sym
  end

  cmd "build_#{name}__mkdirs".to_sym do
    obj_out = File.join(OBJ_OUT, name)
    if !Dir.exist?(obj_out) then Dir.mkdir(obj_out) end
  end

  cmd "build_#{name}__source".to_sym, each(csources) do
    sh "#{CC} -c #{warn} #{CFLAGS} " +
      # Includes
      "#{include.map { |i| "-I#{i}"}.join " "} " +
      # Source
      "#{$file} " +
      # Output
      "-o #{File.join(OBJ_OUT, name, $file.name + ".o")} "
  end

  cmd "build_#{name}__lib".to_sym, all(File.join(OBJ_OUT, name, "*.o")) do
    sh %(#{AR} rc #{File.join(LIB_OUT, "lib#{name}.a")} #{$files})
  end

  cmd "build_#{name}__include".to_sym do
    headers.each do |header|
      if File.exist?(header) then next end
      sh %(ln -s #{File.absolute_path(header)} #{File.join(INCLUDE_OUT, name)})
    end
  end
end
