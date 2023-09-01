#!/usr/bin/env ruby

require 'beaver'
require_relative 'build-util/build_lib.rb'
require_relative 'build-util/build_lib_manual.rb'
require_relative 'build-util/platform_flags.rb'
require_relative 'build-util/make_xcframework.rb'
# TODO: in beaver, add option for $beaver.no_color = true -> doesn't output grey
require_relative 'build-util/xcode.rb'

# TODO: in beaver, add BUILD_OPTION, which will cache separately
# it will keep cache for each confiiguation of build optons and rebuild
# when those options are passed in and the files in their cache has changed

# OPT:
# --enable-optimized --disable-assertions
#   --enable-optimized \
# --disable-assertions \
# --disable-debug --enable-optimized --disable-threads \
# --enable-shared=no --disable-doxygen --disable-expensive-checks

MACOS_VERSION="13.4"
IOS_VERSION=ENV['IOS_VERSION']||"14.0" # defined in XCode

PLATFORM=ENV['PLATFORM']||'macos'
PLATFORM_FLAGS=get_platform_flags(PLATFORM)
ZIG_PLATFORM_FLAGS=get_platform_zig_flags(PLATFORM)

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
OPT=(ENV['OPT']||'DEBUG').upcase
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
# def_lib("arena", "common/arena", include: [INCLUDE_OUT])
def_lib("util", "common/util", include: [INCLUDE_OUT])
def_lib("vec", "common/vec", include: [INCLUDE_OUT])
def_lib("renderer", "common/renderer", include: [INCLUDE_OUT])

cmd :build_renderer_frontend do
  include_dir = File.join(INCLUDE_OUT, "render_frontend")
  if !File.exist?(include_dir) then sh %(ln -s #{File.absolute_path(File.join("common", "render_frontend"))} #{include_dir}) end
end

def_lib("mercator", "deps/mercator", include_sub_dir: "", includes_overwrite: ["mercator.h"])
def_lib("hashmap", "deps/hashmap.c", include_sub_dir: "", includes_overwrite: ["hashmap.h"])
def_lib("o5mreader", "deps/o5mreader/src", include_sub_dir: "", warn: "-Wno-everything", includes_overwrite: ["o5mreader.h"])
#def_lib_man("mercator", csources: ["deps/memphis/memphis/libmercator.c"], headers: ["deps/memphis/memphis/libmercator.h"])

require_relative "tests/make.rb"

def_xcframework("o5mreader", File.join(INCLUDE_OUT, "o5mreader.h"), copy_headers: true)
def_xcframework("hashmap", File.join(INCLUDE_OUT, "hashmap.h"), copy_headers: true)
def_xcframework("map_data", File.join(INCLUDE_OUT, "map_data/parse.h"), copy_headers: true)
def_xcframework("render_objects", File.join(INCLUDE_OUT, "map_data/render_objects.h"), copy_headers: true)
def_xcframework("mercator", File.join(INCLUDE_OUT, "mercator.h"), copy_headers: true)
def_xcframework("util")
def_xcframework("vec")

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
    :renderer => :build_renderer,
    :renderer_frontend => :build_renderer_frontend,
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
  if (force)
    cmds_to_execute.each do |c|
      if (c == :build_renderer && PLATFORM =~ /ios.*/) then next end
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
  zig_libs = [
    #"render_objects",
    ].map { |l| "-l#{l}" }
    .join " "
  # objs = Dir["out/obj/**/*.o"].join " "
  objs = ""

  call :build
  puts "Building test file".blue
  sh %(#{CC} #{PLATFORM_FLAGS} #{CFLAGS} test.c #{objs} -Iout/include -L#{LIB_OUT} #{libraries} -L#{ZIG_LIB} #{zig_libs} -mmacosx-version-min=#{MACOS_VERSION})
  #{Dir["out/obj/map_data/*.o"].join " "}
    #%(-Iout/include -Lout/lib -lSDL3 -lutil -lrenderer -lvec -larena -lhashmap -lo5mreader ) +
    #%(-lmap_data)
end

cmd :build_xcframeworks do
  [
    "o5mreader", "hashmap", "map_data",
    "mercator", "render_objects", "util",
    "vec"
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
