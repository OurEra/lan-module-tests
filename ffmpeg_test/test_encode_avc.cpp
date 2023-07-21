extern "C" {
  #include "libavcodec/avcodec.h"
}
#include "help.h"

bool readFile2Frame(FILE *file, AVFrame *dest)
{
    int size_plane = dest->width * dest->height;
    bool ret = true;
    av_frame_make_writable(dest);
    ret &= fread(dest->data[0], 1, size_plane, file) > 0;
    ret &= fread(dest->data[1], 1, size_plane / 4, file) > 0;
    ret &= fread(dest->data[2], 1, size_plane / 4, file) > 0;
    return ret;
}

// see ffmpeg doc/examples/encode_video.c
int main()
{
    FILE *raw_file = fopen("/media/shiruiwei/ourera/resource/yuvtestseq/RaceHorses_832x480_30.yuv", "r");
    FILE *dest_file = fopen("./output.h264", "w+");
    LOG_COLOR_RED("open file %p", raw_file);

    const AVCodec *encoder = avcodec_find_encoder_by_name("libx264");
    LOG_COLOR_RED("codec %p", encoder);

    AVCodecContext *encoder_context = avcodec_alloc_context3(encoder);
    encoder_context->bit_rate = 2000 * 1000;
    encoder_context->time_base = {1, 20};
    encoder_context->framerate = {20, 1};
    encoder_context->width = 832;
    encoder_context->height = 480;
    encoder_context->gop_size = 20;
    encoder_context->pix_fmt = AV_PIX_FMT_YUV420P;

    int ret = avcodec_open2(encoder_context, encoder, nullptr);
    LOG_COLOR_RED("open codec %d", ret);

    AVFrame *frame = av_frame_alloc();
    frame->width = encoder_context->width;
    frame->height = encoder_context->height;
    frame->format = encoder_context->pix_fmt;
    ret = av_frame_get_buffer(frame, 0);
    LOG_COLOR_RED("alloc frame ret %d", ret);

    AVPacket *packet = av_packet_alloc();

    bool readContent = true;
    int count = 0;
    while (readContent)
    {
        frame->pts = count++;
        readContent = readFile2Frame(raw_file, frame);
        int ret = avcodec_send_frame(encoder_context, frame);
        if (ret != 0)
        {
            LOG_COLOR_RED("send frame ret %d", ret);
        }
        do
        {
            ret = avcodec_receive_packet(encoder_context, packet);
            if (ret >= 0)
            {
                ret = fwrite(packet->data, 1, packet->size, dest_file);
            }
        } while (ret >= 0);
    }

    // clean up
    fclose(raw_file);
    fclose(dest_file);
    avcodec_free_context(&encoder_context);
    av_frame_free(&frame);
    av_packet_free(&packet);

    return 0;
}
