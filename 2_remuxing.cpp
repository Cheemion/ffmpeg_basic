#include <iostream>	
extern "C" {
#include <libavutil/timestamp.h>
#include<libavformat/avformat.h>
}
int main() {
	AVFormatContext* input_format_context = nullptr;
	AVFormatContext* output_format_context = nullptr;
	AVPacket packet;
	const char* in_filename = "C:\\Users\\Administrator\\Desktop\\test.mp4";
	const char* out_filename = "C:\\Users\\Administrator\\Desktop\\test.tsp";
	int ret, i;
	int stream_index = 0;
	int* streams_list = nullptr;
	int number_of_streams = 0;
	int fragmented_mp4_options = 0;

	if ((ret = avformat_open_input(&input_format_context, in_filename, nullptr, nullptr)) < 0) {
		std::cout << "Error - opening file\n";
		goto end;
	}
	if ((ret = avformat_find_stream_info(input_format_context, nullptr)) < 0) {
		std::cout << "Error - Failed to retrieve input stream information\n";
		goto end;
	}

	avformat_alloc_output_context2(&output_format_context, NULL, NULL, out_filename);
	if (!output_format_context) {
		fprintf(stderr, "Could not create output context\n");
		ret = AVERROR_UNKNOWN;
		goto end;
	}

	number_of_streams = input_format_context->nb_streams;

end:
	avformat_close_input(&input_format_context);
	/* close output */
	if (output_format_context && !(output_format_context->oformat->flags & AVFMT_NOFILE))
		avio_closep(&output_format_context->pb);
	avformat_free_context(output_format_context);
	av_freep(&streams_list);
	if (ret < 0 && ret != AVERROR_EOF) {
		fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
		return 1;
	}
	return 0;
}
