cmd :test_runner do
  call :test__compile
  call :test__run
end

test_libs=["cunit", "arena"].map { |l| "-l"+l }.join(" ")

cmd :test__compile do
  sh %(#{CC} -I#{INCLUDE_OUT} -L#{LIB_OUT} #{test_libs} #{Dir["tests/**/*.c"].join " "} -o #{File.join(BIN_OUT, "testrunner")})
end

cmd :test__run do
  sh %(#{File.join("./", BIN_OUT, "testrunner")})
end
