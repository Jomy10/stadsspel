cmd :gen_meson_cross_ios do
  puts "Creating cross-compilation file for iOS".light_black
  File.open(File.join(OUT, "meson-cross-ios.toml"), "w") do |f|
    iphone_sdk_path = `xcrun --sdk iphoneos --show-sdk-path`.gsub("\n", "")
    content = <<-TOML
    [binaries]
    c = ['clang', '-arch', 'arm64', '-isysroot', '#{iphone_sdk_path}']
    cpp = ['clang++', '-arch', 'arm64', '-isysroot', '#{iphone_sdk_path}']
    objc = ['clang', '-arch', 'arm64', '-isysroot', '#{iphone_sdk_path}']
    objcpp = ['clang++', '-arch', 'arm64', '-isysroot', '#{iphone_sdk_path}']
    ar = 'ar'
    strip = 'strip'

    [built-in options]
    c_args = ['-miphoneos-version-min=#{IOS_VERSION}']
    cpp_args = ['-miphoneos-version-min=#{IOS_VERSION}']
    c_link_args = ['-miphoneos-version-min=#{IOS_VERSION}']
    cpp_link_args = ['-miphoneos-version-min=#{IOS_VERSION}']
    objc_args = ['-miphoneos-version-min=#{IOS_VERSION}']
    objcpp_args = ['-miphoneos-version-min=#{IOS_VERSION}']

    [properties]
    root = '#{File.expand_path("../..", iphone_sdk_path)}'
    has_function_printf = true
    has_function_hfkerhisadf = false

    [host_machine]
    system = 'darwin'
    subsystem = 'ios'
    kernel = 'xnu'
    cpu_family = 'aarch64'
    cpu = 'aarch64'
    endian = 'little'
    TOML
    f.write(content)
  end
end

cmd :gen_meson_cross_iossim do
  puts "Creating cross-compilation file for iOSSimulator".light_black
  File.open(File.join(OUT, "meson-cross-iossim.toml"), "w") do |f|
    iphone_sdk_path = `xcrun --sdk iphonesimulator --show-sdk-path`.gsub("\n", "")
    content = <<-TOML
    [binaries]
    c = ['clang', '-arch', 'x86_64', '-isysroot', '#{iphone_sdk_path}']
    cpp = ['clang++', '-arch', 'x86_64', '-isysroot', '#{iphone_sdk_path}']
    objc = ['clang', '-arch', 'x86_64', '-isysroot', '#{iphone_sdk_path}']
    objcpp = ['clang++', '-arch', 'x86_64', '-isysroot', '#{iphone_sdk_path}']
    ar = 'ar'
    strip = 'strip'

    [built-in options]
    c_args = ['-mios-simulator-version-min=#{IOS_VERSION}']
    cpp_args = ['-mios-simulator-version-min=#{IOS_VERSION}']
    c_link_args = ['-mios-simulator-version-min=#{IOS_VERSION}']
    cpp_link_args = ['-mios-simulator-version-min=#{IOS_VERSION}']
    objc_args = ['-mios-simulator-version-min=#{IOS_VERSION}']
    objcpp_args = ['-mios-simulator-version-min=#{IOS_VERSION}']

    [properties]
    root = '#{File.expand_path("../..", iphone_sdk_path)}'
    has_function_printf = true
    has_function_hfkerhisadf = false

    [host_machine]
    system = 'darwin'
    subsystem = 'iossimulator'
    kernel = 'xnu'
    cpu_family = 'x86_64'
    cpu = 'x86_64'
    endian = 'little'
    TOML
    f.write(content)
  end
end
