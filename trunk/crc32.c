#include "crc32.h"


void crc32_init(struct CRC32 * o, uint32_t genpoly){
	o -> genpoly = genpoly;
	o -> state = 0;
}

void crc32_update(struct CRC32 * o, uint8_t byte){
	uint8_t i;
	for(i=0;i<8;i+=1){
		uint8_t bit = byte >> i & 1;
		uint32_t feedback = o -> state & (0x80000000) ? o -> genpoly : 0;
		uint32_t input = o -> state << 1 | bit;
		o -> state = input ^ feedback;
	}
}

void crc32_finalize(struct CRC32 * o){
	crc32_update(o,0);
	crc32_update(o,0);
	crc32_update(o,0);
	crc32_update(o,0);
}

uint8_t crc32_get_reversed(struct CRC32 * o, uint8_t i){
	uint8_t feed = (uint8_t) (o -> state >> 8*(3-i));
	uint8_t byte = 0;
	uint8_t j;
	for(j=0; j < 8; j+=1){
		uint8_t bit = feed & 1;
		feed = feed >> 1;
		byte = byte << 1 | bit;
	}
	return byte;
}

uint8_t crc32_check_zero(struct CRC32 * o){
	return o -> state == 0;
}

