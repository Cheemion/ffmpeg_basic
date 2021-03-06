#include <iostream>
#include <cstdio>
#include <vector>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/codec.h"
}

static void save_gray_frame(unsigned char* buf, int wrap, int xsize, int ysize, char* filename)
{
	FILE* f;
	int i;
	f = fopen(filename, "w");
	// writing the minimal required header for a pgm file format
	// portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
	fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

	// writing line by line
	for (i = 0; i < ysize; i++)
		fwrite(buf + i * wrap, 1, xsize, f);
	fclose(f);
}


int main() {
	int ret;
	std::string url = "C:\\Users\\Administrator\\Desktop\\test.mp4";
	
	//分配内存
	AVFormatContext* pFormatContext = avformat_alloc_context();
	if (!pFormatContext) {
		std::cout << "Error - avformat_alloc_context()" << std::endl;
		return -1;
	}
	//读取头文件
	ret = avformat_open_input(&pFormatContext, url.c_str(), nullptr, nullptr);
	if (ret != 0) {
		std::cout << "cannot open file" << std::endl;
		return ret;
	}

	std::cout << "Format:" << pFormatContext->iformat->long_name << std::endl;
	std::cout << "duration:" <<	pFormatContext->duration << std::endl;
	std::cout << "bit_rate:" << pFormatContext->bit_rate<< std::endl;

	//读取流信息
	ret = avformat_find_stream_info(pFormatContext, nullptr);
	if (ret < 0) {
		std::cout << "cannot read stream" << std::endl;
		return ret;
	}

	AVCodecParameters* pCodecParameters = nullptr;
	AVCodec* pCodec = nullptr;
	int video_stream_index = -1;

	for (int i = 0; i < pFormatContext->nb_streams; i++)
	{
		 pCodecParameters = pFormatContext->streams[i]->codecpar;
		 pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
		if (!pCodec) {
			std::cout << "cannot find codec ID:" << pCodecParameters->codec_id << std::endl;;
			return -1;
		}


		std::printf("Codec %s ID %d bit_rate %lld\n", pCodec->long_name, pCodec->id, pCodecParameters->bit_rate);
		if (pCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
			if (video_stream_index == -1) {
				video_stream_index = i;
				std::printf("Video Codec: resolution %d x %d\n", pCodecParameters->width, pCodecParameters->height);
				break;
			}
		} else if (pCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
			std::printf("Audio Codec: %d channels, sample rate %d\n", pCodecParameters->channels, pCodecParameters->sample_rate);
		}
	}

	AVCodecContext* pCodecContext = avcodec_alloc_context3(pCodec);
	if (!pCodecContext) {
		std::cout << "cannot alloc context for Codec ID:" << pCodec->id << std::endl;
		return -1;
	}
	ret = avcodec_parameters_to_context(pCodecContext, pCodecParameters);
	if (ret < 0) {
		std::cout << "Error - avcodec_parameters_to_context()" << std::endl;;
		return ret;
	}
	ret = avcodec_open2(pCodecContext, pCodec, nullptr);
	if (ret != 0) {
		std::cout << "Error - avcodec_open2()" << std::endl;;
		return ret;
	}

	AVPacket* pPacket = av_packet_alloc();
	if (!pPacket) {
		std::cout << "Error - av_packet_alloc()" << std::endl;;
		return -1;
	}

	AVFrame* pFrame = av_frame_alloc();
	if (!pFrame) {
		std::cout << "Error - av_frame_alloc()" << std::endl;;
		return -1;
	}
	int how_many_packets_to_process = 8;

	while (av_read_frame(pFormatContext, pPacket) >= 0) {

		if (pPacket->stream_index == video_stream_index) {

			int response = avcodec_send_packet(pCodecContext, pPacket);

			if (response < 0)
				std::cout << "Error - avcodec_send_packet()" << std::endl;

			while (response >= 0) {
				response = avcodec_receive_frame(pCodecContext, pFrame);
				if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
					break;
				} else if (response < 0) {
					return response;
				}

				char frame_filename[1024];
				snprintf(frame_filename, sizeof(frame_filename), "%s-%d.pgm", "frame", pCodecContext->frame_number);
				std::cout << "pts : " << pFrame->pts << std::endl;
				std::cout << "pkt_dts : " << pFrame->pkt_dts << std::endl;
				std::cout << "av_get_picture_type_char(pFrame->pict_type) : " << av_get_picture_type_char(pFrame->pict_type) << std::endl;
				std::cout << "pFrame->pts : " << pFrame->pts << std::endl;
				std::cout << "pFrame->pkt_dts : " << pFrame->pkt_dts << std::endl;
				std::cout << "pFrame->key_frame : " << pFrame->key_frame << std::endl;
				std::cout << "pFrame->coded_picture_number : " << pFrame->coded_picture_number << std::endl;
				std::cout << "pFrame->display_picture_number : " << pFrame->display_picture_number << std::endl;

				save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
			}
			if (--how_many_packets_to_process <= 0) break;
		}
		
		av_packet_unref(pPacket);
	}

	//free all resources
	av_frame_free(&pFrame);
	av_packet_free(&pPacket);
	avcodec_free_context(&pCodecContext);
	avformat_close_input(&pFormatContext);
	return 0;
}