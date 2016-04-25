#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "clhash.h"




static inline void flipbit ( void * block, int length, uint32_t bit ) {
    uint8_t * b = (uint8_t*)block;
    int byte = bit >> 3;
    assert(byte < length);
    bit = bit & 0x7;
    b[byte] ^= (1 << bit);
}

static void clhashavalanchetest() {
    const int N = 1024;
    char * array  = (char*)malloc(N);
    char * array1  = (char*)malloc(N);

    char *  rs = (char*)malloc(RANDOM_BYTES_NEEDED_FOR_CLHASH);
    for(int k = 0; k<N; ++k) {
        array[k] = 0;
        array1[k] = 0;
    }
    for(int k = 0; k<RANDOM_BYTES_NEEDED_FOR_CLHASH; ++k) {
        rs[k] = k+1-k*k;
    }
    int K = 16;
    printf("[clhashavalanchetest] Testing CLHASH avalanche effect.\n");

    for(int bytelength = 1; bytelength < K; ++bytelength ) {

        for(int whichcase = 0; whichcase < 256; ++whichcase) {

            for(int k = 0; k<bytelength; ++k) {
                array[k] = whichcase;
                array1[k] = whichcase + 35;
            }
            uint64_t orighash = clhash(rs, array, bytelength);
            uint64_t orighash1 = clhash(rs, array1, bytelength);

            for(int z = 0; z < 8*bytelength; ++z) {
                flipbit(array,bytelength,z);
                uint64_t newhash = clhash(rs, array, bytelength);
                flipbit(array,bytelength,z);
                assert(orighash != newhash);

                flipbit(array1,bytelength,z);
                uint64_t newhash1 = clhash(rs, array1, bytelength);
                flipbit(array1,bytelength,z);
                assert(orighash1 != newhash1);

                if((unsigned int) bytelength <= sizeof(uint64_t))
                    assert((orighash ^ newhash) == (orighash1 ^ newhash1));

            }
        }
    }
    free(array);
    free(array1);
    free(rs);
    printf("Test passed! \n");
}

// ---------------------------------------------------------------------
// contributed by Eik List
static void clhashcollisiontest() {
    printf("[clhashcollisiontest] Testing whether we can induce collisions by hacking the right bytes (Eik List's test).\n");
    const size_t NUM_TRIALS = 10;
    const size_t CLNH_NUM_BYTES_PER_BLOCK = 1024;
    const uint8_t KEY_OFFSET = 0x63; // Anything to prevent that K = M

    uint8_t* k = (uint8_t*)malloc(RANDOM_BYTES_NEEDED_FOR_CLHASH);
    size_t j2;

    // Fill key with some deterministic information
    for (j2 = 0; j2 < RANDOM_BYTES_NEEDED_FOR_CLHASH; ++j2) {
        k[j2] = (j2 + KEY_OFFSET) & 0xFF;
    }

    for (size_t i = 1; i < NUM_TRIALS; ++i) {
        for (size_t j = 1; j <= sizeof(uint64_t); ++j) {
            // #Bytes / block + x, with 1 <= x < 8
            const uint64_t mlen = i * CLNH_NUM_BYTES_PER_BLOCK + j;
            uint8_t* m = (uint8_t*)malloc(mlen);

            for (j2 = 0; j2 < mlen; ++j2) {
                m[j2] = j2 & 0xFF;
            }

            const uint64_t actual1 = clhash(k, (const char*)m, mlen);

            // Change final byte
            m[mlen-1] = (m[mlen-1] + 1) & 0xFF;

            const uint64_t actual2 = clhash(k, (const char*)m, mlen);
            const int are_equal = !memcmp(&actual1, &actual2, sizeof(uint64_t));

            if(are_equal) printf("Testing %" PRIu64 " bytes, H1: %" PRIX64 ", H2: %" PRIX64 ", equal: %d \n", mlen, actual1, actual2, are_equal);
            free(m);
            assert(!are_equal); // strictly speaking this would not be a bug, but if it happens, it is very likely to be a bug!
        }
    }

    free(k);
    printf("Test passed! \n");
}


static void clhashtest() {
    const int N = 1024;
    char * array  = (char*)malloc(N);
    char *  rs = (char*)malloc(RANDOM_BYTES_NEEDED_FOR_CLHASH);
    for(int k = 0; k<N; ++k) {
        array[k] = 0;
    }
    for(int k = 0; k < RANDOM_BYTES_NEEDED_FOR_CLHASH ; ++k) {
        rs[k] = (char) (1-k);
    }

    printf("[clhashtest] checking that flipping a bit changes hash value with clhash \n");
    for(int bit = 0; bit < 64; ++bit ) {
        for(int length = (bit+8)/8; length <= (int)sizeof(uint64_t); ++length) {
            uint64_t x = 0;
            uint64_t orig = clhash(rs, (const char *)&x, length);
            x ^= ((uint64_t)1) << bit;
            uint64_t flip = clhash(rs, (const char *)&x, length);
            assert(flip != orig);
            x ^= ((uint64_t)1) << bit;
            uint64_t back = clhash(rs, (const char *)&x, length);
            assert(back == orig);
        }
    }

    free(array);
    free(rs);
    printf("Test passed! \n");
}

void demo() {
    // generate random key
    void * random =  get_random_key_for_clhash(UINT64_C(0x23a23cf5033c3c81),UINT64_C(0xb3816f6a2c68e530));
    uint64_t hashvalue1 = clhash(random,"my dog",6);
    uint64_t hashvalue2 = clhash(random,"my cat",6);
    uint64_t hashvalue3 = clhash(random,"my dog",6);
    assert(hashvalue1 == hashvalue3);
    assert(hashvalue1 != hashvalue2);// very likely to be true

    free(random);
}


int main() {
    clhashtest();
    clhashcollisiontest();
    clhashavalanchetest();
    demo();
}

