# Starts the app and waits for the debugger to attach

set -e
set -m

adb="$ANDROID_SDK_ROOT/platform-tools/adb"
pkg="be.jonaseveraert.stadsspel"
port=1234

# copy lldb-server to device
$adb push "$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/darwin-x86_64/lib64/clang/14.0.6/lib/linux/aarch64/lldb-server" /data/local/tmp
$adb shell chmod a+x /data/local/tmp/lldb-server

$adb shell killall -9 lldb-server || true
sleep 1
$adb shell run-as $pkg cp /data/local/tmp/lldb-server /data/data/$pkg
$adb shell am start -D -n "$pkg/android.app.NativeActivity"

# Start the application and wait for lldb to connect
#$adb shell run-as $pkg ./lldb-server platform --server --listen "*:$port"
# $adb shell run-as $pkg ./lldb-server g 192.168.43.1:2000
$adb shell run-as $pkg ./lldb-server --list "*:$port" --server
