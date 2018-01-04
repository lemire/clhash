#include <cassert>
#include <vector>
#include <cstdio>
#include "clhash.h"


int main(void) {
    clhasher h(1, 4);
    std::vector<int> vec{1,3,4,5,2,24343};
    int arr[6];
    std::memcpy(arr, vec.data(), sizeof(arr));
    auto hash(h(vec));
    // HPR
    assert(h(arr, 6) == hash);
    hash = h("o hai wurld");
    // HPR
    assert(hash == h(std::string("o hai wurld")));
    hash = h(vec.data() + 1, vec.size() - 1);
    // HPR
    assert(h(vec.data() + 1, vec.size() - 1) == h(std::vector<int>(vec.begin() + 1, vec.end())));
    // HPR
    hash = h(7723291);
    // HPR
    assert(h(7723291ULL) != hash); // Changing the size of the object changes the "string".
    printf("code is good.\n");
}
