/*
 * CLHash is a very fast hashing function that uses the
 * carry-less multiplication and SSE instructions.
 *
 * Daniel Lemire, Owen Kaser, Faster 64-bit universal hashing
 * using carry-less multiplications, Journal of Cryptographic Engineering (to appear)
 *
 * Best used on recent x64 processors (Haswell or better).
 *
 * Compile option: if you define BITMIX during compilation, extra work is done to
 * pass smhasher's avalanche test succesfully. Disabled by default.
 **/

#ifndef INCLUDE_CLHASH_H_
#define INCLUDE_CLHASH_H_


#include <stdlib.h>
#include <stdint.h> // life is short, please use a C99-compliant compiler
#include <stddef.h>

enum {RANDOM_64BITWORDS_NEEDED_FOR_CLHASH=133,RANDOM_BYTES_NEEDED_FOR_CLHASH=133*8};



/**
 *  random : the random data source (should contain at least
 *  RANDOM_BYTES_NEEDED_FOR_CLHASH random bytes), it should
 *  also be aligned on 16-byte boundaries so that (((uintptr_t) random & 15) == 0)
 *  for performance reasons. This is usually generated once and reused with many
 *  inputs.
 *
 *
 * stringbyte : the input data source, could be anything you want to has
 *
 *
 * length : number of bytes in the string
 */
uint64_t clhash(const void* random, const char * stringbyte,
                const size_t lengthbyte);



/**
 * Convenience method. Will generate a random key from two 64-bit seeds.
 * Caller is responsible to call "free" on the result.
 */
void * get_random_key_for_clhash(uint64_t seed1, uint64_t seed2);


#endif /* INCLUDE_CLHASH_H_ */
