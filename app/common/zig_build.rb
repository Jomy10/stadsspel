cmd :build_zig_all, all("common/**/*.zig") do
  sh "cd common && #{ZIGCC} build " +
    "--prefix #{File.join("..", ZIG_OUT)} " +
    "--cache-dir #{File.join("..", ZIG_CACHE)} " +
    "--global-cache-dir #{File.join("..", ZIG_CACHE)} " +
    "-Dcompile-c=true " +
    "-Doptimize=#{OPT=='0'?'Debug':'ReleaseFast'} "

  Dir[File.join(ZIG_OUT, "lib", "lib*.a")].each do |file|
    if !File.exist?(File.join(LIB_OUT, File.basename(file)))
      sh %(ln -s #{File.absolute_path(file)} #{File.absolute_path(File.join(LIB_OUT, File.basename(file)))})
    end
  end
end
