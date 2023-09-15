def get_cc(platform)
  if platform =~ /ios.*/
    return File.absolute_path(
      File.join(`xcrun --sdk iphoneos --show-sdk-platform-path`.gsub("\n", ""),
      "..", "..", "usr", "bin", "gcc"))
  elsif platform == 'android'
    return File.join(ANDROID_NDK_ROOT, "toolchains", "llvm", "prebuilt", HOST_MACHINE, "bin", "#{android_arch(ARCH)}-linux-android#{ANDROID_SDK_VERSION}-clang")
  else
    return ENV["CC"] || "clang"
  end
end

def get_ar(platform)
  if platform == "android"
    return File.join(ANDROID_NDK_ROOT, "toolchains", "llvm", "prebuilt", HOST_MACHINE, "bin", "llvm-ar")
  end

  return ENV['AR']||(
    (`#{CC} --version`.lines[0].include? 'clang') ?
      (`command -v llvm-ar` == "" ? 'ar' : 'llvm-ar') :
      'ar'
  )
end
