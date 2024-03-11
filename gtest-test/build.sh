# build gtest first
# cd googletest        # Main directory of the cloned repository.
# mkdir build          # Create a directory to hold the build output.
# cd build
# cmake ..             # Generate native build scripts for GoogleTest.
# make -j4


# ./test-gtest --gtest_list_tests
# ./test-gtest --gtest_filter=SquareRootTest.PositiveNos*

clang -std=c++17 -lstdc++ -lm -lpthread -lcurl -I/home/shiruiwei/thirdparty/googletest/googletest/include basic.cpp \
        /home/shiruiwei/thirdparty/googletest/build/lib/libgtest.a \
        -o test-gtest
