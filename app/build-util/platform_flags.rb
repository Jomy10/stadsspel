def android_arch(arch)
  case ARCH
  when "ARM64"
    return "aarch64"
  when "ARM32"
    return "armv7a"
  when "X86_32"
    return "i686"
  when "X86_64"
    return "x86_64"
  end
end

def get_platform_flags(platform)
  case platform
    when 'macos'
      sdk_root = `xcrun --sdk macosx --show-sdk-path`.gsub("\n", "")
      return "-mmacosx-version-min=#{MACOS_VERSION} " +
        "-isysroot #{sdk_root} "
    when 'iossim'
      sdk_root = `xcrun --sdk iphonesimulator --show-sdk-path`.gsub("\n", "")
      return "-arch x86_64 " +
        "-mios-simulator-version-min=#{IOS_VERSION} " +
        "-isysroot #{sdk_root} "
    when 'ios'
      sdk_root = `xcrun --sdk iphoneos --show-sdk-path`.gsub("\n", "")
      return "-arch arm64 " +
        "-isysroot #{sdk_root} " +
        "-mios-version-min=#{IOS_VERSION} "
    when 'android'
      androidflags = "-ffunction-sections -fdata-sections -DANDROID -DAPPNAME=\"stadsspel\" " +
        "-I#{File.join(ANDROID_NDK_ROOT, "sysroot", "usr", "include")}" +
        "-I#{File.join(ANDROID_NDK_ROOT, "sysroot", "usr", "include", "android")} " +
        "-I#{File.join(ANDROID_NDK_ROOT, "toolchains", "llvm", "prebuilt", HOST_MACHINE, "sysroot", "usr", "include")} " +
        "-I#{File.join(ANDROID_NDK_ROOT, "toolchains", "llvm", "prebuilt", HOST_MACHINE, "sysroot", "usr", "include", "android")} " +
        "-fPIC -DANDROIDVERSION=#{ANDROID_SDK_VERSION} "
      case ARCH
      when "ARM64"
        androidflags += "-m64 "
      when "ARM32"
        androidflags += "-mfloat-abi=softfp -m32 "
      when "X86_32"
        androidflags += "-march=i686 -mssse3 -mfpmath=sse -m32 "
      when "X86_64"
        androidflags += "-march=x86-64 -msse4.2 -mpopcnt -m64 "
      end
    else
      return ""
  end
end

def get_platform_zig_flags(platform)
  case platform
    when 'iossim'
      return "-Dtarget=native-ios-simulator --sysroot #{`xcrun --sdk iphonesimulator --show-sdk-path`.gsub("\n", "")}"
    when 'ios'
      return "-Dtarget=aarch64-ios --sysroot #{`xcrun --sdk iphoneos --show-sdk-path`.gsub("\n", "")}"
    when 'android'
      arch = nil
      case ARCH
      when "ARM64"
        arch = "aarch64"
      when "ARM32"
        arch = "arm"
      when "X86_32"
        arch = "x86"
      when "X86_64"
        arch = "x86_64"
      end

      return "-Dtarget=#{arch}-linux-android -Dandroid-version=android#{ANDROID_VERSION} "
    else
      return ""
  end
end
