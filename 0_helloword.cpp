#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/codec.h"
}
int main() {
	AVFormatContext* pFormatContext = avformat_alloc_context();
	std::cout << "hello11" << std::endl;
	return 0;
}