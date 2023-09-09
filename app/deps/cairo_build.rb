require_relative "../build-util/meson-cross/ios-gen.rb"

CAIRO_BUILD_DIR="build"

cmd :build_cairo do
  case PLATFORM
  when "macos"
    if ENV['DONT_SKIP_CAIRO_MACOS'] == 'true'
      Dir.chdir("deps/cairo") do
        sh %(meson setup #{build_dir})
        sh %(ninja -C #{build_dir})
      end
    end
  when 'ios'
    build_cairo_ios("iphoneos")
  when 'iossim'
    build_cairo_ios("iphonesimulator")
  end

  call :build_cairo__include
end

cmd :build_cairo__include do
  if PLATFORM =~ /ios.*/
    include_dir_ios = File.join(INCLUDE_OUT, "cairo-ios")
    if !File.exist?(include_dir_ios)
      sh %(ln -s #{File.absolute_path(File.join("deps", "cairo-ios", "cairo"))} #{include_dir_ios})
    end
    next
  end
  include_dir = File.join(INCLUDE_OUT, "cairo")
  if !File.exist?(include_dir)
    sh %(ln -s #{File.absolute_path(File.join("deps", "cairo", "src"))} #{include_dir})
  end
end

def build_cairo_ios(sdk)
  libfile = ""
  Dir.chdir("deps/cairo-ios") do
    sh %(xcodebuild -sdk #{sdk})
    libfile = File.absolute_path(File.join("build", "Release-#{sdk}", "libcairo.a"))
  end

  outlibfile = File.absolute_path(File.join(LIB_OUT, "libcairo.a"))
  if File.exists?(outlibfile)
    sh %(rm #{outlibfile})
  end
  sh %(cp #{libfile} #{outlibfile})
end

##==== Pixman ====##

cmd :build_pixman do
  call :build_pixman__include

  if (PLATFORM =~ /ios.*/)
    case PLATFORM
    when 'ios'
      build_cairo_ios('iphoneos')
    when 'iossim'
      build_cairo_ios('iphonesimulator')
    end

    call :build_pixman__include

    next
  end

  if PLATFORM == 'macos'
    if ! ENV['DONT_SKIP_CAIRO_MACOS'] then next end
  end

  call "gen_meson_cross_#{PLATFORM}"

  cross_file = File.absolute_path File.join(OUT, "meson-cross-#{PLATFORM}.toml")
  Dir.chdir("deps/pixman") do
    sh %(meson setup --cross-file #{cross_file} build-#{PLATFORM})
    sh %(ninja -C build-#{PLATFORM})
  end

  lib_out = File.join(LIB_OUT, "libpixman.a")
  if File.exist?(lib_out)
    sh %(rm #{lib_out})
  end
  lib_in = File.join("deps", "pixman", "build-#{PLATFORM}", "pixman", "libpixman*.a")
  sh %(cp #{Dir[lib_in][0]} #{lib_out})
end

cmd :build_pixman__include do
  if PLATFORM =~ /ios.*/
    include_dir_ios = File.join(INCLUDE_OUT, "pixman-ios")
    if !File.exist?(include_dir_ios)
      sh %(ln -s #{File.absolute_path(File.join("deps", "pixman-ios", "pixman"))} #{include_dir_ios})
    end
    next
  end
  include_dir = File.join(INCLUDE_OUT, "pixman")
  if !File.exist?(include_dir)
    sh %(ln -s #{File.absolute_path(File.join("deps", "pixman", "src"))} #{include_dir})
  end
end

def build_pixman_ios(sdk)
  libfile = ""
  Dir.chdir("deps/pixman-ios") do
    sh %(xcodebuild -sdk #{sdk})
    libfile = File.absolute_path(File.join("build", "Release-#{sdk}", "libpixman.a"))
  end

  outlibfile = File.absolute_path(File.join(LIB_OUT, "libpixman.a"))
  if File.exists?(outlibfile)
    sh %(rm #{outlibfile})
  end
  sh %(cp #{libfile} #{outlibfile})
end
