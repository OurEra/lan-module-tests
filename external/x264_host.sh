# configure
./configure --enable-static --enable-shared --disable-asm --enable-debug --prefix=host/

# encode
./host/bin/x264 -o test.h264 ~/resource/dump_640x360_i420.yuv 640x360
