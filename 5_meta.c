#include<libavformat/avformat.h>
#include<libavutil/log.h>
#include<stdio.h>
int main() {
	int ret;
	av_log_set_level(AV_LOG_INFO);
	AVFormatContext* fmt_ctx = NULL;
	ret = avformat_open_input(&fmt_ctx, "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4", NULL, NULL);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot open file");
		return -1;
	}
	av_dump_format(fmt_ctx, 0, "C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\out.mp4", 0);

	avformat_close_input(&fmt_ctx);
	return 0;
}
