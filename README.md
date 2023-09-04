# Development

## Cloning the repo

```sh
git clone –recurse-submodules https://github.com/jomy10/stadsspel
```

## Building

### Building the app

These tools are required to build the project:
- a C compiler (for iOS: needs to be able to build Object-C as well (i.e. the default Apple clang compiler installed on any Mac))
- a [Zig](https://ziglang.org/learn/getting-started/#installing-zig)-compiler (min version 0.12 when building for iOS)
- the [beaver build system](https://github.com/jomy10/beaver)

To build the app, the project needs to be compiled for the target platform. The possible values for the platform variable are:
- ios
- iossim
- macos
- android
```sh
cd app
PLATFORM=platform_from_list_above ./make.rb build all
```

**iOS instructions**
```sh
cd app

PLATFORM=ios ./make.rb build all -f
PLATFORM=iossim ./make.rb build all -f
./make.rb build_xcframeworks

open iOS/Stadsspel/Stadsspel.xcodeproj -a xcode
```

Now press `⌘ + B` to build the project in XCode.

**macOS instructions**
```sh
cd app
./make.rb build_macos_main
```

The binary is now at `out/macos/debug/app`.

**Android instructions**

*WIP*

## Tools

To configure clangd for your editor (this requires [bear](https://github.com/rizsotto/Bear)):

```sh
cd app
./make.rb configure_clangd
```
