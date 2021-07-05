# build gtest first
# cd googletest        # Main directory of the cloned repository.
# mkdir build          # Create a directory to hold the build output.
# cd build
# cmake ..             # Generate native build scripts for GoogleTest.
# make -j4

clang -std=c++17 -lstdc++ -lm -lpthread -I../external/gtest/googletest/include basic.cpp \
        ../external/gtest/build/lib/libgtest.a \
        -o test-gtest
