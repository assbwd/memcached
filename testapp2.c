#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stdint.h"

enum test_return { TEST_SKIP, TEST_PASS, TEST_FAIL };

static enum test_return cache_limit_revised_downward_test(void) {
  int limit = 10, allocated_num = limit + 1, i;
  char **alloc_objs = calloc(allocated_num, sizeof(char *));

  cache_t *cache = cache_create("test", sizeof(uint32_t), sizeof(char *));
  assert(cache != NULL);

  printf("buffer size: %ld\n", cache->bufsize);

  /* cache->limit is 0 and we can allocate limit+1 items */
  for (i = 0; i < allocated_num; i++) {
    alloc_objs[i] = cache_alloc(cache);
    assert(alloc_objs[i] != NULL);
  }
  char * h = (char*) alloc_objs[0];
  *(h - 1) = 10;
  assert(cache->total == allocated_num);

  /* revised downward cache->limit */
  cache_set_limit(cache, limit);

  /* If we free one item, the cache->total should decreased by one*/
  cache_free(cache, alloc_objs[0]);

  assert(cache->total == allocated_num - 1);
  cache_destroy(cache);

  free(alloc_objs);

  return TEST_PASS;
}

int main() {
  size_t s = sizeof(uint64_t);
  printf("size of uint64_t is: %ld\n", s);
  cache_limit_revised_downward_test();
  printf("%s\n", "test app2 success");
}
