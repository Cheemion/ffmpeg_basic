#include<libavformat/avformat.h>
#include<libavcodec/avcodec.h>
#include<libavutil/log.h>
#include<stdio.h>

int h264_mp4toannexb(AVFormatContext* fmt_ctx, AVPacket* in, FILE* dst_fd) {

	AVPacket* out = NULL;
	AVPacket spspps_pkt;

	int len;
	uint8_t unit_type;
	int32_t nal_size;
	uint32_t cumul_size = 0;
	const uint8_t* buf;
	const uint8_t* buf_end;
	int buf_size;
	int ret = 0;
	int i;

	out = av_packet_alloc();
	buf = in->data;
	buf_size = in->size;
	buf_end = in->data + in->size; //exclusive

	do {
		ret = AVERROR(EINVAL);
		if (buf + 4 > buf_end)
			goto fail;

		//前面4个字节为大小
		for (nal_size = 0, i = 0; i < 4; i++) {
			nal_size = (nal_size << 8) | buf[i];
		}
		buf = buf + 4;
		unit_type = *buf & 0x1f; //(pps 8) (sps7) k frame 5  non-key 7
		if (nal_size > (buf_end - buf) || nal_size < 0) {
			goto fail;
		}

		if (unit_type == 5) { //key frame

		}
	}

fail:
	av_packet_free(&out);

	return ret;
}

int main() {
	int err_code;
	char errors[1024];
	const char* input = "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4";
	const char* out = "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.yuv";
	FILE* outFile = fopen(out, "wb");
	if (!outFile) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open file");
		return -1;
	}
	av_log_set_level(AV_LOG_DEBUG);
	// deprected av_register_all();

	AVFormatContext* fmt_ctx = NULL;
	AVPacket pkt;
	if ((err_code = avformat_open_input(&fmt_ctx, input, NULL, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot open context %s", av_err2str(err_code));
		return -1;
	}

	av_dump_format(fmt_ctx, 0, "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4", 0);
	//outFile

	int video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

	if (video_stream_index < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot find video stream");
		avformat_close_input(&fmt_ctx);
		fclose(outFile);
		return -1;
	}
	av_init_packet(&pkt);

	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		if (pkt.stream_index == video_stream_index) {
			h264_mp4toannexb(fmt_ctx, &pkt, outFile);
		}
		av_packet_unref(&pkt);
	}

	fclose(outFile);
	avformat_close_input(&fmt_ctx);
	return 0;
}
