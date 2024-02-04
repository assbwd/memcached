#define XXH_INLINE_ALL
#include "memcached/xxhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/**
  * This is a simple test for xxhash.
  */

// genereate a random string with length
const char* random_string(size_t length) {
  char* str = malloc(length + 1);
  assert(str != NULL);
  for (size_t i = 0; i < length; i++) {
    str[i] = 'a' + rand() % 26;
  }
  str[length] = '\0';
  return str;
}

// generate a random unsigned integer between 0 and max
// like `__VERIFIER_nondet_uint()` in SV-COMP
size_t random_uint(size_t max) {
  // initialize random seed, or it will always be the same
  srand(time(NULL));
  return rand() % max;
}

int main() {
    size_t size = random_uint(100);
    const char* buffer = random_string(size);
    XXH64_hash_t hash = XXH3_64bits(buffer, size);
    printf("str:  %s\n", buffer);
    printf("hash: %lu\n", hash);
}