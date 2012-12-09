#ifndef CRC32_H
#define CRC32_H
#include <stdint.h>

struct CRC32 {
	uint32_t genpoly;
	uint32_t state;
};

/**
 * Initializes engine, setting state to zero and generational polynomial
 * to the specified
 */
void crc32_init(struct CRC32 * o, uint32_t genpoly);


/**
 * Hashes next byte into checksum
 */
void crc32_update(struct CRC32 * o, uint8_t byte);


/**
 * Hashes 4 zero bytes to finalize checksum calculation
 */
void crc32_finalize(struct CRC32 * o);


/**
 * Gets i'th byte of reversed checksum, that turns state into
 * all-zeroes, when hashed sequentially after it's data.
 */
uint8_t crc32_get_reversed(struct CRC32 * o, uint8_t i);



/**
 * Checks that state is equal to 0
 */
uint8_t crc32_check_zero(struct CRC32 * o);


#endif
