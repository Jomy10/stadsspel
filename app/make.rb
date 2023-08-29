#!/usr/bin/env ruby
require 'beaver'
require 'colorize'
require_relative 'build-util/build_lib.rb'
require_relative 'build-util/build_lib_manual.rb'

MACOS_VERSION="13.4"
CC=ENV['CC']||'clang'
ZIGCC=ENV['ZIGCC']||'zig'
CFLAGS=""
AR=ENV['AR']||(
  (`#{CC} --version`.lines[0].include? 'clang') ?
    (`command -v llvm-ar` == "" ? 'ar' : 'llvm-ar') :
    'ar'
)
WARN='-Wall -Wextra'
OUT=ENV['OUT']||'out'
OBJ_OUT=File.join(OUT, "obj")
LIB_OUT=File.join(OUT, "lib")
INCLUDE_OUT=File.join(OUT, "include")
BIN_OUT=File.join(OUT, "bin")
ZIG_BASE=File.join(OUT, "zig");
ZIG_OUT=File.join(ZIG_BASE, "out")
ZIG_CACHE=File.join(ZIG_BASE, "cache")
ZIG_LIB=File.join(ZIG_OUT, "lib")
OPT=ENV['OPT']||'0'

force=(!File.exist?(OUT)) ? true : (ARGV.size >= 3 && ARGV[2] == '-f' || ARGV.size >= 2 && ARGV[1] == '-f')

#== Define build steps ==#
require_relative "common/zig_build.rb"
def_lib("map_data", "common/map_data/src", include: [INCLUDE_OUT])
# def_lib("arena", "common/arena", include: [INCLUDE_OUT])
def_lib("util", "common/util", include: [INCLUDE_OUT])
def_lib("vec", "common/vec", include: [INCLUDE_OUT])
def_lib("renderer", "common/renderer", include: [INCLUDE_OUT])

def_lib("mercator", "deps/mercator", include_sub_dir: "", includes_overwrite: ["mercator.h"])
def_lib("hashmap", "deps/hashmap.c", include_sub_dir: "", includes_overwrite: ["hashmap.h"])
def_lib("o5mreader", "deps/o5mreader/src", include_sub_dir: "", warn: "-Wno-everything", includes_overwrite: ["o5mreader.h"])
#def_lib_man("mercator", csources: ["deps/memphis/memphis/libmercator.c"], headers: ["deps/memphis/memphis/libmercator.h"])

require_relative "tests/make.rb"
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
      puts c.to_s.blue
      $beaver.must_run c
    end
  else
    cmds_to_execute.each do |c|
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
      "hashmap", "o5mreader",
      "SDL3"
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
  sh %(#{CC} #{CFLAGS} test.c #{objs} -Iout/include -L#{LIB_OUT} #{libraries} -L#{ZIG_LIB} #{zig_libs} -mmacosx-version-min=#{MACOS_VERSION})
  #{Dir["out/obj/map_data/*.o"].join " "}
    #%(-Iout/include -Lout/lib -lSDL3 -lutil -lrenderer -lvec -larena -lhashmap -lo5mreader ) +
    #%(-lmap_data)
end

cmd :test do
  call :mkdirs
  call :test_runner
end

cmd :configure_clangd do
  sh %(bear -- ./make.rb build all -f)
end

cmd :mkdirs do
  [OUT, OBJ_OUT, LIB_OUT, INCLUDE_OUT, BIN_OUT, ZIG_BASE, ZIG_OUT, ZIG_OUT]
    .each do |path|
      if !Dir.exist?(path) then Dir.mkdir(path) end
    end
end

$beaver.set_main :build

$beaver.end
