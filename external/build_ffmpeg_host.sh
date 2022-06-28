X264_DIR=/home/shiruiwei/thirdparty/x264/host
./configure --enable-libx264 --enable-gpl \
        --disable-x86asm --disable-stripping --enable-debug \
        --extra-cflags="-I$X264_DIR/include" \
        --extra-ldflags="-L$X264_DIR/lib"

# x264 load path
export LD_LIBRARY_PATH=./


# ./configure
# –help	print this message
# –list-decoders
# –list-encoders
# –list-hwaccels
# –list-demuxers
# –list-muxers
# –list-parsers
# –list-protocols
# –list-bsfs
# –list-indevs
# –list-outdevs
# –list-filters
