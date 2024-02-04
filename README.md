# clhash

C library implementing the ridiculously fast CLHash hashing function (with C++ wrappers)


 CLHash is a very fast hashing function that uses the
 carry-less multiplication and SSE instructions.
 Best used on recent x64 processors (Haswell or better).

CLHash has the following characteristics :

* On a recent Intel processor (e.g., Skylake), it can hash input strings at a speed of 0.1 cycles per byte for sufficiently long strings. You read this right: it is simply ridiculously fast.
* It has strong theoretical guarantees: XOR universality of short strings and excellent almost universality for longer strings.

For details, please see the research article:

Daniel Lemire, Owen Kaser, Faster 64-bit universal hashing using carry-less multiplications, Journal of Cryptographic Engineering 6 (3), 2016. http://arxiv.org/abs/1503.03465

## Requirements


Please do not try to compile and run this software on legacy hardware (x64 processors
before Haswell), it will either fail to work or be slow. It should be able to port
the code to other architectures such as ARM or POWER processors but performance is
unknown at this point. And, yes, this means that CLHash is *not* portable hardware-wise.

If your compiler is not C99 compliant... please get better one.

If your applications requires hashing tiny strings, then you will not get a speed close to 0.1 cycles per byte. 
The string should be significantly several times larger than a vector register (128 bits). So clhash is not
meant to be a general-purpose hash function.


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
```

## Simple benchmark

 ```bash
 make
 ./benchmark
 ```

 ## C++

If you prefer the convenience of a C++ interface with support for stl::vector and std::string,
you can create a clhasher object instead.

```C
#include <assert.h>
#include <cstdio>
#include <vector>

#include "clhash.h"


int main(void) {
    clhasher h(UINT64_C(0x23a23cf5033c3c81),UINT64_C(0xb3816f6a2c68e530));
    std::vector<int> vec{1,3,4,5,2,24343};
    uint64_t vechash = h(vec);

    uint64_t arrayhash = h(vec.data(), vec.size());

    assert(vechash == arrayhash);

    uint64_t cstringhash = h("o hai wurld");
    uint64_t stringhash = h(std::string("o hai wurld"));
    assert(cstringhash == stringhash);
}
```

 ```bash
 make
 ./cppunit
 ```


Credit for C++ wrapper: Daniel Baker
