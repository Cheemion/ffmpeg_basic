#include<libavutil/log.h>
#include<stdio.h>

int main() {
	av_log_set_level(AV_LOG_DEBUG);
	av_log(NULL, AV_LOG_INFO, "hello world %d", 10);
	return 0;
}