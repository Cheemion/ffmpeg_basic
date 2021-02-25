#include<libavformat/avformat.h>
#include<stdio.h>
int main() {


	av_register_all();


	int ret;
	ret = avpriv_io_move("111.txt", "222.txt");
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "ERROR - fail to rename");
		return -1;
	}
	av_log(NULL, AV_LOG_INFO, "rename successfully");


	ret = avpriv_io_delete("C:\\Users\\Administrator\\Desktop\\ffmpeg_basic\\mytestfile.txt");
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "ERROR - fail to delete flie");
		return -1;
	}
	av_log(NULL, AV_LOG_INFO, "delete successfully");
	return 0;
}
