#define XXH_INLINE_ALL
 #include "memcached/xxhash.h"
 #include <stdio.h>

 int main(){
 const char* buffer = "hello,world,world,world";
 size_t size = strlen(buffer);
 XXH64_hash_t hash = XXH3_64bits(buffer, size);
 printf("hash: %lu\n", hash);