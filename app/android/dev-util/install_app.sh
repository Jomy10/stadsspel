if [[ ANDROID_ARCH == "" ]]; then
  echo "Install for architecture"
  ANDROID_ARCH=$(gum choose "x86_64" "arm64")
fi

cd "build-${ANDROID_ARCH}"
"${ANDROID_SDK_ROOT}/platform-tools/adb" install -r apk-signed.apk
