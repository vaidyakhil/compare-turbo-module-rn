# reference: https://github.com/mrousavy/react-native-vision-camera,
# reference: https://github.com/mrousavy/react-native-mmkv

# sets the name of library or executable
# to load it in java code, this is the name to be used
cmake_minimum_required(VERSION 3.4.1)

# used to set a variable to a value
set(PACKAGE_NAME "custom-jsi-module")
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)


# the `SHARED` here tells Cmake to build this lib
# as a dynamic link library, i.e .so file on linux/android
add_library(
    ${PACKAGE_NAME}
    SHARED
    # Shared C++
    cpp-adapter.cpp
)

# Third party libraries (Prefabs)
# this only looks for the mentioned packages and do not link them itself
# for linking, call target_link_libraries
find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)
find_library(LOG_LIB log)

# Link everything together
target_link_libraries(
    ${PACKAGE_NAME}
    ${LOG_LIB}                          # <-- Logcat logger
#    android                             # <-- Android JNI core
    ReactAndroid::jsi                   # <-- RN: JSI | required to use <jsi/jsi.h> headers
    ReactAndroid::reactnativejni        # <-- RN: React Native JNI bindings
    fbjni::fbjni                        # <-- fbjni
)