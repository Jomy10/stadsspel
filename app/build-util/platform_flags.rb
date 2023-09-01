def get_platform_flags(platform)
  case platform
    when 'macos'
      sdk_root = `xcrun --sdk macosx --show-sdk-path`.gsub("\n", "")
      return "-mmacosx-version-min=#{MACOS_VERSION} " +
      # -isysroot
        "-isysroot #{sdk_root} "
        # "-syslibroot #{sdk_root}"
    when 'iossim'
      sdk_root = `xcrun --sdk iphonesimulator --show-sdk-path`.gsub("\n", "")
      return "-arch x86_64 " +
        # "-mcpu=x86_64 " +
        # "-sys=ios " +
        #"-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk " +
        # "--host=i386-apple-darwin_sim " +
        "-mios-simulator-version-min=#{IOS_VERSION} " +
        "-isysroot #{sdk_root} "
        # "-syslibroot #{sdk_root} "
        #"-Fdeps/SDL/XCode/SDL/"
    when 'ios'
      sdk_root = `xcrun --sdk iphoneos --show-sdk-path`.gsub("\n", "")
      return "-arch armv7" +
        "-mcpu=cortex-a8 -marm " +
        "-isysroot #{sdk_root} " +
        # "-syslibroot #{sdk_root} " +
        "-mios-version-min=#{IOS_VERSION} "
    else
      return ""
  end
end

def get_platform_zig_flags(platform)
  case platform
    when 'iossim'
      return "-Dtarget=x86_64-ios-simulator --sysroot #{`xcrun --sdk iphonesimulator --show-sdk-path`.gsub("\n", "")} "
        #"-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk "
    when 'ios'
      return "-Dtarget=aarch64-ios "
    else
      return ""
  end
end
