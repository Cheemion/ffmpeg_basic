#include<libavformat/avformat.h>
#include<libavutil/log.h>
#include<stdio.h>
int main() {
	int len;
	int ret;
	int audio_index;
	av_log_set_level(AV_LOG_INFO);
	const char* input = "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4";
	const char* out = "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.acc";

	AVFormatContext* fmt_ctx = NULL;
	AVPacket pkt;
	ret = avformat_open_input(&fmt_ctx, input, NULL, NULL);

	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot open file %s", av_err2str(ret));
		return -1;
	}

	av_dump_format(fmt_ctx, 0, "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4", 0);
	//outFile
	FILE* outFile = fopen(out, "wb");
	if (!outFile) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open file");
		return -1;
	}
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot fine audio stream");
		avformat_close_input(&fmt_ctx);
		fclose(outFile);
		return -1;
	}
	audio_index = ret;
	av_init_packet(&pkt);
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		if (pkt.stream_index == audio_index) {
			len = fwrite(pkt.data, 1, pkt.size, outFile);
			if (len != pkt.size) {
				av_log(NULL, AV_LOG_ERROR, "Error when Writting audio binary");
			}
		}
	}

	fclose(outFile);
	avformat_close_input(&fmt_ctx);
	return 0;
}
