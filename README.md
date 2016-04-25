# clhash
C library implementing the ridiculously fast CLHash hashing function


 CLHash is a very fast hashing function that uses the
 carry-less multiplication and SSE instructions.
 Best used on recent x64 processors (Haswell or better).
  
For details, please see the research article:
  
Daniel Lemire, Owen Kaser, Faster 64-bit universal hashing using carry-less multiplications, Journal of Cryptographic Engineering (to appear) http://arxiv.org/abs/1503.03465

## Requirements

 
Please do not try to compile and run this software on legacy hardware (x64 processors
before Haswell), it will either fail to work or be slow. It should be able to port
the code to other architectures such as ARM or POWER processors but performance is
unknown at this point. And, yes, this means that CLHash is *not* portable hardware-wise.

If your compiler is not C99 compliant... please get better one.


 
 
## Usage 
 
 ```bash
 make
 ./unit
 ```
Compile option: if you define BITMIX during compilation, extra work is done to 
pass smhasher's avalanche test succesfully. Disabled by default.
 
## Code sample
 
```C
#include <assert.h>

#include "clhash.h"

int main() {
    void * random =  get_random_key_for_clhash(UINT64_C(0x23a23cf5033c3c81),UINT64_C(0xb3816f6a2c68e530));
    uint64_t hashvalue1 = clhash(random,"my dog",6);
    uint64_t hashvalue2 = clhash(random,"my cat",6);
    uint64_t hashvalue3 = clhash(random,"my dog",6);
    assert(hashvalue1 == hashvalue3);
    assert(hashvalue1 != hashvalue2);// very likely to be true
    free(random);
    return 0;
}

``
