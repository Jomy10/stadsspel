# Excellent article here: https://www.joelotter.com/posts/2022/10/android-ndk-without-gradle/

set -e
set -x

ANDROID_BUILD_TOOLS_VERSION="33.0.1"
ANDROID_SDK_VERSION="33"
PROJECT_NAME="stadsspel"
OPT="debug"
# ANDROID_ARCH="x86_64"

build_tools="${ANDROID_SDK_ROOT}/build-tools/${ANDROID_BUILD_TOOLS_VERSION}"
aapt="${build_tools}/aapt"
zipalign="${build_tools}/zipalign"
apksigner="${build_tools}/apksigner"

# Building #
ruby make.rb build

[ ! -d "build-${ANDROID_ARCH}" ] && mkdir "build-${ANDROID_ARCH}"
cd "build-${ANDROID_ARCH}"

# Building #
# cmake -DCMAKE_SYSTEM_NAME="Android" \
#   -DCMAKE_ANDROID_NDK="${ANDROID_NDK_ROOT}" \
#   -DANDROID_NDK="${ANDROID_NDK_ROOT}" \
#   -DCMAKE_SYSTEM_PROCESSOR=${ANDROID_ARCH} \
#   -DCMAKE_C_FLAGS="-fPIC" \
#   -DCMAKE_CXX_FLAGS="-fPIC" \
#   ..

# make VERBOSE=1

# Packaging #
mkdir -p "apk/lib/${ANDROID_ARCH}"
# cp "lib${PROJECT_NAME}.so" "apk/lib/${ANDROID_ARCH}"
cp "../../out/android-${ANDROID_ARCH}/debug/lib/lib${PROJECT_NAME}.so" "apk/lib/${ANDROID_ARCH}"

$aapt package \
  -f \
  -M ../AndroidManifest.xml \
  -I "${ANDROID_SDK_ROOT}/platforms/android-${ANDROID_SDK_VERSION}/android.jar" \
  -S ../res \
  -F apk-unaligned.apk \
  apk

# Align and sign #
$zipalign -f -v 4 apk-unaligned.apk apk-unsigned.apk

$apksigner sign \
  --ks ../debug.keystore \
  --ks-key-alias=androidkey \
  --key-pass pass:${STOREPASS} \
  --ks-pass pass:${STOREPASS} \
  --out apk-signed.apk \
  apk-unsigned.apk

  # TODO: copy to bin
