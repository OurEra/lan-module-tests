/home/shiruiwei/Android/Sdk/cmake/3.6.4111459/bin/cmake ./ \
    -DCMAKE_TOOLCHAIN_FILE=/home/shiruiwei/opt/android-ndk-r14b/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_NDK=/home/shiruiwei/opt/android-ndk-r14b \
    -DANDROID_NATIVE_API_LEVEL=23 \
    -DANDROID_ARM_NEON=ON \
    -DANDROID_TOOLCHAIN=clang

make -j4
