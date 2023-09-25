# Building android

1. generate a storepass

```sh
./build.sh gen-key
# Choose debug or release key
# Choose a password for the key
# Input the correct information
```

2. Build the app

```sh
ANDROID_ARCH=x86_64 STOREPASS=the_storepass_password ./build.sh build
```

3. Install the apk onto a simulator


```sh
# Launching a simulator
./build.sh emulator start
# To see logs
./build.sh emulator logcat

# Instal on the device
ANDROID_ARCH=x86_64 ./build.sh install
```
