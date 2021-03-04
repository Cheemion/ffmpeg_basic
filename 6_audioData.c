#include<libavformat/avformat.h>
#include<libavutil/log.h>
#include<stdio.h>
#define ADTS_HEADER_LEN 7; //Header consists of 7 or 9 bytes (without or with CRC).
typedef unsigned short uint_16;
typedef unsigned char byte;
typedef unsigned int uint;


void adts_header1(char* szAdtsHeader, int dataLen) {
	int audio_object_type = 2;
	int sampling_frequency_index = 3;
	int channel_config = 2;
	int adtslen = dataLen + 7;
	szAdtsHeader[0] = 0xff;
	szAdtsHeader[1] = 0xf0;
	szAdtsHeader[1] |= (0 << 3);
	szAdtsHeader[1] |= (0 << 1);
	szAdtsHeader[1] |= 1;

	szAdtsHeader[2] = (audio_object_type - 1) << 6;
	szAdtsHeader[2] |= (sampling_frequency_index & 0x0f) << 2;
	szAdtsHeader[2] |= (0 << 1);
	szAdtsHeader[2] |=  (channel_config & 0x04) >> 2; 


	szAdtsHeader[3] = (channel_config & 0x03) << 6;
	szAdtsHeader[3] |= (0 << 5);
	szAdtsHeader[3] |= (0 << 4);
	szAdtsHeader[3] |= (0 << 3);
	szAdtsHeader[3] |= (0 << 2);
	szAdtsHeader[3] |= ((adtslen & 0x1800) >> 11);
	szAdtsHeader[4] = (uint8_t)((adtslen & 0x7f8) >> 3);
	szAdtsHeader[5] = (uint8_t)((adtslen & 0x7) << 5);
	szAdtsHeader[5] |= 0x1f;
	szAdtsHeader[6] = 0xfc;	
}

void adts_header(char* szAdtsHeader, byte datalen)	{
	szAdtsHeader[0] = 0xff; //设置syncword12bit 都为1
	szAdtsHeader[1]	= 0xf0; //设置syncword12bit 都为1
	byte mpegVersion = 0;
	szAdtsHeader[1]	|= (mpegVersion << 3); //设置 MPEG Version：MPEG Version: 0 for MPEG-4, 1 for MPEG-2 
	byte layer = 0;
	szAdtsHeader[1]	|= (layer << 1); //set Layer: always 0
	byte noCRC = 1;
	szAdtsHeader[1]	|= noCRC;  //protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC
	byte profile = 1; //(profile, the MPEG-4 Audio Object Type minus 1)    1: AAC Main
	byte sampling_frequency_index = 6;  //4: 44100 Hz  //3: 48000 Hz

	byte channelConfiguration = 2; //双声道
	szAdtsHeader[2] = (profile << 6) | (sampling_frequency_index << 2) | (channelConfiguration >> 2);
	uint_16 frameLength = (noCRC == 1 ? 7 : 9) + datalen; //13 bits
	szAdtsHeader[3] = (channelConfiguration << 6) | (byte)(frameLength >> 11);
	szAdtsHeader[4] = (byte)(frameLength >> 3);
	szAdtsHeader[5] = (((byte)frameLength) << 5) | (byte)0xff;
	byte numberOfACCFrames = 1;
	szAdtsHeader[6]	= (byte)0xfc; 

}

int main() {
	int len;
	int ret;
	int audio_index;
	av_log_set_level(AV_LOG_INFO);
	const char* input = "C:\\Users\\kim\\Desktop\\ffmpeg_basic\\out.mp4";
	const char* out = "C:\\Users\\kim\\Desktop\\ffmpeg_basic\\out.acc";

	AVFormatContext* fmt_ctx = NULL;
	AVPacket pkt;
	ret = avformat_open_input(&fmt_ctx, input, NULL, NULL);

	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "cannot open file %s", av_err2str(ret));
		return -1;
	}

	av_dump_format(fmt_ctx, 0, "C:\\Users\\kim\\Desktop\\ffmpeg_basic\\out.mp4", 0);
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
	av_init_packet(&pkt);//
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		if (pkt.stream_index == audio_index) {
			char adts_header_buf[7];
			adts_header1(adts_header_buf, pkt.size);
			fwrite(adts_header_buf, 1, 7, outFile);
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
