#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.12)
project("stadsspel")

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# MiniFB
add_library(minifb STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/include/MiniFB.h
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/include/MiniFB_cpp.h
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/include/MiniFB_enums.h

    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_common.c
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_cpp.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_internal.c
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_timer.c
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/MiniFB_linux.c
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/WindowData.h

    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/android/AndroidMiniFB.c
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src/android/WindowData_Android.h
)

target_include_directories(minifb PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/include
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/src
)

#file(GLOB app_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../common/app/*.c")
#add_library(app STATIC ${app_SRC})
#target_include_directories(minifb PRIVATE
#    ${CMAKE_CURRENT_LIST_DIR}/../out/include)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(stadsspel SHARED
  main.cpp
)

target_include_directories(stadsspel PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${CMAKE_CURRENT_LIST_DIR}/../deps/minifb/include
    ${CMAKE_CURRENT_LIST_DIR}/../deps/olive.c
    ${CMAKE_CURRENT_LIST_DIR}/../out/include)

# add lib dependencies
target_link_libraries(stadsspel -L${CMAKE_CURRENT_LIST_DIR}/../out/android-x86_64/debug/lib -larena)
target_link_libraries(stadsspel
    android
    minifb
    native_app_glue
    log)
