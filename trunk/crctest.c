#include <stdio.h>
#include <string.h>
#include "crc32.h"


#define GENPOLY (0x678cd5cf)

int main(int argc, char ** argv){
	if(argc != 2){
		return 1;
	}
	char * str = argv[1];
	uint8_t crc_val[4];
	{
		// generating CRC
		struct CRC32 crc;
		crc32_init(&crc, GENPOLY);
		uint8_t * p;
		for(p = str; *p; p += 1){
			crc32_update(&crc, *p);
		}
		crc32_finalize(&crc);
		uint8_t i;
		for(i=0;i<4;i+=1){
			crc_val[i] = crc32_get_reversed(&crc, i);
		}
	}
	int _return;
	{
		// checking crc
		struct CRC32 crc;
		crc32_init(&crc, GENPOLY);
		uint8_t * p;
		for(p = str; *p; p += 1){
			crc32_update(&crc, *p);
		}
		uint8_t i;
		for(i=0;i<4;i+=1){
			crc32_update(&crc, crc_val[i]);
		}
		if(crc32_check_zero(&crc)){
			fprintf(stderr, "success!\n");
			_return = 0;
		}else{
			fprintf(stderr, "fail =(\n");
			_return = 1;
		}
	}
	return _return;
}
