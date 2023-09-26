if $beaver.nil?
  require 'beaver'
end
require 'fileutils'
require 'colorize'
require_relative '../build-util/build_lib.rb'

$beaver.set :e

ANDROID_NDK=ENV["ANDROID_NDK_ROOT"]
ANDROID_SDK=ENV["ANDROID_SDK_ROOT"]
ANDROID_TOOLCHAINS=File.join(ANDROID_NDK, "toolchains")
ANDROID_LLVM_BIN=File.join(ANDROID_TOOLCHAINS, "llvm", "prebuilt", "darwin-x86_64", "bin")
ANDROID_ARCH=(ENV["ARCH"]||"x86_64").downcase
ANDROID_SDK_VERSION=33
ANDROID_SYSROOT=File.join(ANDROID_TOOLCHAINS, "darwin-x86_64", "sysroot", "usr", "lib", "#{ANDROID_ARCH}-linux-android", ANDROID_SDK_VERSION.to_s)
ANDROID_CC=File.join(ANDROID_LLVM_BIN, "#{ANDROID_ARCH}-linux-android#{ANDROID_SDK_VERSION}-clang")
ANDROID_CXX=ANDROID_CC+"++"
ANDROID_AR=File.join(ANDROID_LLVM_BIN, "llvm-ar")
ANDROID_C_FLAGS=(ENV["C_FLAGS"]||"") + " -fPIC"
ANDROID_CXX_FLAGS=(ENV["CXX_FLAGS"]||"") + " -fPIC"
ANDROID_OPT=(ENV['OPT']||'DEBUG')
ANDROID_OUT="../out/android-#{ANDROID_ARCH.downcase}/#{ANDROID_OPT.downcase}"
ANDROID_OBJ_OUT=File.join(ANDROID_OUT, "obj")
ANDROID_LIB_OUT=File.join(ANDROID_OUT, "lib")
ANDROID_INCLUDE_OUT="../out/include"

cmd :build do
  puts "Building for #{ANDROID_ARCH}..."
  call :make_dirs
  call :build_native_activity
  call :build_minifb
  call :build_obj
  call :build_shared_lib
end

cmd :make_dirs do
  [ANDROID_OUT, ANDROID_OBJ_OUT, ANDROID_LIB_OUT].each do |dir|
    if !Dir.exist?(dir)
      FileUtils.mkdir_p(dir)
    end
  end
end

cmd :build_obj, each([
  "main.cpp",
]) do
  cc = nil
  cflags = nil
  if $file.ext == ".c"
    cc = ANDROID_CC
    cflags = ANDROID_C_FLAGS
  else
    cc = ANDROID_CXX
    cflags = ANDROID_CXX_FLAGS
  end

  sh %(#{cc} #{cflags} -c #{$file} -I../out/include ) +
    %(-I#{File.join ANDROID_NDK, "sources/android/native_app_glue"} ) +
    %(-I#{"../deps/minifb/include"} ) +
    %(-o #{File.join(ANDROID_OBJ_OUT, $file.name + ".o")})
end

cmd :build_native_activity do
  call :build_native_activity_obj
  call :build_native_activity_archive
end

cmd :build_native_activity_obj, each(
  File.join(ANDROID_NDK, "sources/android/native_app_glue/android_native_app_glue.c")
) do
  sh %(#{ANDROID_CC} #{ANDROID_C_FLAGS} -c #{$file} -I#{File.join ANDROID_NDK, "sources/android/native_app_glue"} -o #{File.join(ANDROID_OBJ_OUT, $file.name + ".o")})
end

cmd :build_native_activity_archive, all(File.join(ANDROID_OBJ_OUT, "android_native_app_glue.o")) do
  sh %(#{ANDROID_AR} rc #{File.join(ANDROID_LIB_OUT, "libandroid_native_app_glue.a")} #{$files})
end

cmd :build_minifb do
  call :build_minifb_obj
  call :build_minifb_lib
end

cmd :build_minifb_obj, each([
  # "../deps/minifb/include/MiniFB.h",
  # "../deps/minifb/include/MiniFB_cpp.h",
  # "../deps/minifb/include/MiniFB_enums.h",
  "../deps/minifb/src/MiniFB_common.c",
  "../deps/minifb/src/MiniFB_cpp.cpp",
  "../deps/minifb/src/MiniFB_internal.c",
  # "../deps/minifb/src/MiniFB_internal.h",
  "../deps/minifb/src/MiniFB_timer.c",
  "../deps/minifb/src/MiniFB_linux.c",
  # "../deps/minifb/src/WindowData.h",
  "../deps/minifb/src/android/AndroidMiniFB.c",
  # "../deps/minifb/src/android/WindowData_Android.h",
]) do
  out = File.join(ANDROID_OBJ_OUT, "minifb")
  if !Dir.exist?(out)
    FileUtils.mkdir_p(out)
  end
  sh %(#{$file.ext == ".cpp" ? ANDROID_CXX : ANDROID_CC} ) +
    %(#{$file.ext == ".cpp" ? ANDROID_CXX_FLAGS : ANDROID_C_FLAGS} ) +
    %(-c #{$file} ) +
    [
      "#{ANDROID_NDK}/sources/android/native_app_glue",
      "../deps/minifb/include",
      "../deps/minifb/src",
    ].map { |p| "-I#{p}" }.join(" ") + " " +
    %(-o #{out + "/" + $file.name + ".o"})
end

cmd :build_minifb_lib, all(File.join(ANDROID_OBJ_OUT, "minifb", "*.o")) do
  sh %(#{ANDROID_AR} rc #{File.join(ANDROID_LIB_OUT, "libminifb.a")} #{$files})
end

cmd :build_shared_lib, all([File.join(ANDROID_OBJ_OUT, "main.o")]) do
  puts $files
  libs = Dir[File.join(ANDROID_LIB_OUT, "*.a")].map do |lib|
    "-l#{File.basename(lib,".a").gsub("lib", "")}"
  end.join(" ")
  # libs = "-larena "
  shared_libs = ["log"].map do |lib|
    "-l#{lib}"
  end.join(" ")
  # sh %(#{ANDROID_CC} -Wall -DNDEBUG -O2 -u ANativeActivity_onCreate -Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections -Qunused-arguments -Wl,--no-undefined -shared #{libs} -L#{ANDROID_SYSROOT} #{shared_libs} -I#{ANDROID_INCLUDE_OUT} -o #{File.join(ANDROID_LIB_OUT, "libstadsspel.so")} #{$files} -Wl,--export-dynamic -landroid -llog -static-libstdc++ -latomic -lm)
  puts $files
  sh %(#{ANDROID_CXX} -fPIC -std=gnu++11 -Wall -O2 -g -DNDEBUG ) +
    %(-Wl,--build-id=sha1 -Wl,--no-rosegment -Wl,--fatal-warnings -Wl,--gc-sections ) +
    %(-Qunused-arguments -Wl,--no-undefined ) +
    %(-u ANativeActivity_onCreate ) +
    %(-shared ) +
    %(-Wl,-soname,libstadsspel.so ) +
    %(-o #{File.join(ANDROID_LIB_OUT, "libstadsspel.so")} ) +
    %(#{$files} ) +
    # %(#{File.join(ANDROID_LIB_OUT, "libandroid_native_app_glue.a")} ) +
    %(-L#{ANDROID_LIB_OUT} ) +
    # %(-lrender_objects )
    # %(-L#{ANDROID_NDK + "/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/x86_64-linux-android/33/"}) +
    # %(-Wl,--whole-archive ) +
    %(#{libs} ) +
    # %(#{`fd -HiL -t file -e .o libzhashmap ../out/android-x86_64/debug/zig`.gsub("\n", "")} ) +
    # %(#{`fd -HiL -t file -e .o liblinked_list ../out/android-x86_64/debug/zig`.gsub("\n", "")} ) +
    # %(#{`fd -HiL -t file -e .o librender_objects ../out/android-x86_64/debug/zig`.gsub("\n", "")} ) +
    %(../out/android-x86_64/debug/lib/librender_objects.a ) +
    %(-landroid -llog ) +
    %(-static-libstdc++ ) +
    %(-latomic -lm )
end

$beaver.end
