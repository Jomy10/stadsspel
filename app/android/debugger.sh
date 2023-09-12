set -e
set -x

adb="$ANDROID_SDK_ROOT/platform-tools/adb"
pkg="be.jonaseveraert.stadsspel"

# Start debugging server on android device
$adb shell ./data/local/tmp

#$adb shell iptables -I INPUT -t nat -p tcp -d 192.168.43.1 --dport 2000 -j SNAT --to-source 192.168.43.1:50000
# $adb shell ./data/local/tmp/lldb-server g 192.168.43.1:2000

# lldb --attach-pid 192.168.43.1:2000

# Start app and wait for debugger to attach
# $adb shell am start -a android.intent.action.MAIN -n "$pkg/android.app.NativeActivity"

# pid=$($adb shell run-as $pkg ps | grep $pkg | awk '{print $2}')
# echo "Process pid = $pid"
# $adb forward tcp:12345 tcp:12345
# lldb --attach-pid $pid
