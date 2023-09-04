#!/usr/bin/env ruby

require 'beaver'
require_relative 'build-util/build_lib.rb'
require_relative 'build-util/build_lib_manual.rb'
require_relative 'build-util/platform_flags.rb'
require_relative 'build-util/make_xcframework.rb'
# TODO: in beaver, add option for $beaver.no_color = true -> doesn't output grey
require_relative 'build-util/xcode.rb'

$beaver.set(:e)

# TODO: in beaver, add BUILD_OPTION, which will cache separately
# it will keep cache for each confiiguation of build optons and rebuild
# when those options are passed in and the files in their cache has changed

MACOS_VERSION="13.4"
IOS_VERSION=ENV['IOS_VERSION']||"14.0" # defined in XCode

PLATFORM=ENV['PLATFORM']||'macos'
PLATFORM_FLAGS=get_platform_flags(PLATFORM)
ZIG_PLATFORM_FLAGS=get_platform_zig_flags(PLATFORM)

valid_platforms = ["macos", "ios", "iossim", "android"]
if !(valid_platforms.include? PLATFORM)
  puts "Invalid platform, valid platforms are #{valid_platforms}"
  exit -1;
end

CC=(PLATFORM =~ /ios.*/) ? (
  File.absolute_path(
    File.join(`xcrun --sdk iphoneos --show-sdk-platform-path`.gsub("\n", ""),
      "..", "..", "usr", "bin", "gcc"))
) : (ENV['CC'] || 'clang')
ZIGCC=ENV['ZIGCC']||'zig'
CFLAGS=""
AR=ENV['AR']||(
  (`#{CC} --version`.lines[0].include? 'clang') ?
    (`command -v llvm-ar` == "" ? 'ar' : 'llvm-ar') :
    'ar'
)
WARN='-Wall -Wextra'
OUT_BASE=ENV['OUT']||'out'
OUT_BASE_PLATFORM=File.join(OUT_BASE, PLATFORM)
OPT=(ENV['OPT']||'DEBUG')

case (OPT.upcase)
when 'DEBUG'
  CFLAGS << " -g -O0"
when 'RELEASE'
  CFLAGS << " -O3"
end

OUT=File.join(OUT_BASE_PLATFORM, OPT.downcase)
OBJ_OUT=File.join(OUT, "obj")
LIB_OUT=File.join(OUT, "lib")
INCLUDE_OUT=File.join(OUT_BASE, "include")
BIN_OUT=File.join(OUT, "bin")
ZIG_BASE=File.join(OUT, "zig");
ZIG_OUT=File.join(ZIG_BASE, "out")
ZIG_CACHE=File.join(ZIG_BASE, "cache")
ZIG_LIB=File.join(ZIG_OUT, "lib")
FRAMEWORK_OUT=File.join(OUT_BASE, "frameworks")

force=(!File.exist?(OUT)) ? true : (ARGV.size >= 3 && ARGV[2] == '-f' || ARGV.size >= 2 && ARGV[1] == '-f')

# TODO: Caching last used platform

#== Define build steps ==#
require_relative "common/zig_build.rb"
def_lib("map_data", "common/map_data/src", include: [INCLUDE_OUT])
def_lib("util", "common/util", include: [INCLUDE_OUT])
def_lib("vec", "common/vec", include: [INCLUDE_OUT])
def_lib("uirenderer", "common/uirenderer", include: [INCLUDE_OUT])
require_relative "common/render_backends/make.rb"
require_relative "macOS/make.rb"
def_lib("app", "common/app", include: [INCLUDE_OUT]);

# Dependencies
def_lib("mercator", "deps/mercator", include_sub_dir: "", includes_overwrite: ["mercator.h"])
def_lib("hashmap", "deps/hashmap.c", include_sub_dir: "", includes_overwrite: ["hashmap.h"])
def_lib("o5mreader", "deps/o5mreader/src", include_sub_dir: "", warn: "-Wno-everything", includes_overwrite: ["o5mreader.h"])

cmd :build_copy_sdl_include do
  include_dir = File.join(INCLUDE_OUT, "SDL3")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("deps", "SDL", "include"))} #{include_dir}) end
end

cmd :build_copy_stb_include do
  include_dir = File.join(INCLUDE_OUT, "stb")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("deps", "stb"))} #{include_dir}) end
end

require_relative "tests/make.rb"

# Frameworks for iOS, iOSSimulator and macOS
def_xcframework("o5mreader", File.join(INCLUDE_OUT, "o5mreader.h"), copy_headers: true)
def_xcframework("hashmap", File.join(INCLUDE_OUT, "hashmap.h"), copy_headers: true)
def_xcframework("map_data", File.join(INCLUDE_OUT, "map_data/parse.h"), copy_headers: true)
def_xcframework("render_objects", File.join(INCLUDE_OUT, "map_data/render_objects.h"), copy_headers: true)
def_xcframework("mercator", File.join(INCLUDE_OUT, "mercator.h"), copy_headers: true)
def_xcframework("util")
def_xcframework("vec")
def_xcframework("renderer")
def_xcframework("uirenderer")
def_xcframework("app");
#== End Define build steps ==#

#== commands ==#
cmd :build do
  call :mkdirs

  cmds_to_execute = []
  commands = {
    :hashmap => :build_hashmap,
    :o5mreader => :build_o5mreader,
    :zig_all => :build_zig_all,
    # :arena => :build_arena,
    :mercator => :build_mercator,
    :util => :build_util,
    :vec => :build_vec,
    :map_data => :build_map_data,
    :renderer_frontend => :build_renderer_frontend,
    :sdl => :build_copy_sdl_include,
    :stb => :build_copy_stb_include,
    :app => :build_app,
    :renderer => :build_renderer,
    :uirenderer => :build_uirenderer,
  }
  # Determine commands to run
  if ((ARGV.size < 2) || (ARGV[1].to_sym == :all))
    puts "Building all targets".red
    cmds_to_execute.append(*commands.map do |k, v|
      v
    end)
  else
    cmds_to_execute << commands[ARGV[1].to_sym]
  end

  # Execute commands

  # first copy all includes
  cmds_to_execute.each do |c|
    c_include = (c.to_s + "__include").to_sym
    if $beaver.command_exists? c_include
      call c_include
    end
  end

  if (force)
    cmds_to_execute.each do |c|
      #if (c == :build_renderer && PLATFORM =~ /ios.*/) then next end
      puts c.to_s.blue
      $beaver.must_run c
    end
  else
    cmds_to_execute.each do |c|
      if (c == :build_renderer && PLATFORM =~ /ios.*/) then next end
      puts c.to_s.blue
      call c
    end
  end
end

# builds test.c
cmd :dbg do
  #force = true
  CFLAGS << " -g -Og"
  libraries = [
      "util", "renderer", "vec", "map_data", "mercator", "render_objects",
      "hashmap", "o5mreader"
      #"SDL3"
      # "bgfx"
    ]
    .map { |l| "-l#{l}" }
    .join " "

  call :build
  puts "Building test file".blue
  sh %(#{CC} #{PLATFORM_FLAGS} #{CFLAGS} test.c -Iout/include -L#{LIB_OUT} #{libraries} -mmacosx-version-min=#{MACOS_VERSION})
end

cmd :build_xcframeworks do
  [
    "o5mreader", "hashmap", "map_data",
    "mercator", "render_objects", "util",
    "vec", "renderer", "uirenderer", "app"
  ]
    .each { |n|
      puts "building framework #{n}".blue
      call "build_#{n}_xcframework".to_sym
    }

end

cmd :test do
  call :mkdirs
  call :test_runner
end

cmd :configure_clangd do
  sh %(bear -- ./make.rb build all -f)
end

cmd :mkdirs do
  [OUT_BASE, OUT_BASE_PLATFORM, OUT, OBJ_OUT, LIB_OUT, INCLUDE_OUT, BIN_OUT, ZIG_BASE, ZIG_OUT, ZIG_OUT, FRAMEWORK_OUT]
    .each do |path|
      if !Dir.exist?(path) then Dir.mkdir(path) end
    end
end

$beaver.set_main :build

$beaver.end
