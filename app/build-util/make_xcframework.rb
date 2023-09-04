require 'tmpdir'
require 'fileutils'

def def_xcframework(name, headers = File.join(INCLUDE_OUT, name), copy_headers: false)
  cmd "build_#{name}_xcframework".to_sym,
    all([
      "iossim", "ios", "macos"
    ].map { |platform| File.join(OUT_BASE, platform, OPT.downcase, "lib") }
     .map do |platform_lib_path|
       File.join(platform_lib_path, "lib#{name}.a")
     end
  ) do
    headers_dir = nil
    if copy_headers
      headers_dir = File.join(Dir.tmpdir(), "be.jonaseveraert.beaver.build.xcframework.include")
      if Dir.exist?(headers_dir) then `rm -r #{headers_dir}` end
      Dir.mkdir(headers_dir)

      if headers.respond_to? :each
        headers.each do |header|
          FileUtils.copy(header, File.join(headers_dir, File.basename(header)))
        end
      else
        FileUtils.copy_file(headers, File.join(headers_dir, File.basename(headers)))
      end
  else
      headers_dir = headers
    end

    # archives = [
    #   "iossim",
    #   "ios",
    #   "macos"
    # ]
    #   .map { |platform| File.join(OUT_BASE, platform, OPT.downcase, "lib") }
    #   .map do |platform_libs|
    #     File.join(platform_libs, "lib#{name}.a")
    #   end
    archives = $files.paths

    libraries = archives
      .map do |ar|
        File.exist?(ar) ? ar : nil
      end
      .filter do |ar|
        !ar.nil?
      end
      .map do |ar|
        "-library #{ar} -headers #{headers_dir}"
      end
      .join(" ")

    fout = File.join(FRAMEWORK_OUT, name + ".xcframework")
    if File.exist?(fout) then sh %(rm -r #{fout}) end
    sh %(xcodebuild -create-xcframework #{libraries} -output #{fout})
  end
end
