clang -std=c++17 -lstdc++ -lpthread -I../external/glm main.cpp \
        TestScale.cpp TestVector.cpp TestSet.cpp TestSlot.cpp TestDeque.cpp TestGLM.cpp \
        TestTimestampExtrapolator.cpp TestJMP.cpp TestMap.cpp TestPtr.cpp TestJson.cpp \
        -o test-cpp
