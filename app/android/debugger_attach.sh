# Attaches lldb to the application

set -e
set -x

adb="$ANDROID_SDK_ROOT/platform-tools/adb"
pkg="be.jonaseveraert.stadsspel"
port=8006

#$adb shell run-as $pkg ps
pid=$($adb shell run-as $pkg ps | grep $pkg | awk '{print $2}')
echo "Process pid = $pid"

devicename=$($adb devices | sed -n 2p | awk '{print $1}')
echo "Device name = $devicename"

echo "
platform select remote-android
platform connect connect://$devicename:$port
attach $pid
" > debugger_lldb_commands
lldb -s debugger_lldb_commands -O log
