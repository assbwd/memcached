#include <stdbool.h>
#include "memcached.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Hook the hash function, this is unrelated to the problem */
uint32_t _hash(const void *key, size_t length) {
    return 0;
}

/* Hash table that uses the global hash function */
static PREFIX_STATS *prefix_stats[PREFIX_HASH_SIZE];

static char prefix_delimiter;
static int num_prefixes = 0;
static int total_prefix_size = 0;

void stats_prefix_init(char delimiter) {
    prefix_delimiter = delimiter;
    memset(prefix_stats, 0, sizeof(prefix_stats));
}

void stats_prefix_clear(void) {
    int i;

    for (i = 0; i < PREFIX_HASH_SIZE; i++) {
        PREFIX_STATS *cur, *next;
        for (cur = prefix_stats[i]; cur != NULL; cur = next) {
            next = cur->next;
            free(cur->prefix);
            free(cur);
        }
        prefix_stats[i] = NULL;
    }
    num_prefixes = 0;
    total_prefix_size = 0;
}

PREFIX_STATS *stats_prefix_find(const char *key, const size_t nkey) {
    PREFIX_STATS *pfs;
    uint32_t hashval;
    size_t length;
    bool bailout = true;

    assert(key != NULL);

    for (length = 0; length < nkey && key[length] != '\0'; length++) {
        if (key[length] == prefix_delimiter) {
            bailout = false;
            break;
        }
    }

    if (bailout) {
        return NULL;
    }

    /* Edited here, ignore the hash */
    hashval = _hash(key, length) % PREFIX_HASH_SIZE;

    for (pfs = prefix_stats[hashval]; NULL != pfs; pfs = pfs->next) {
        if (strncmp(pfs->prefix, key, length) == 0)
            return pfs;
    }

    pfs = calloc(sizeof(PREFIX_STATS), 1);
    if (NULL == pfs) {
        perror("Can't allocate space for stats structure: calloc");
        return NULL;
    }

    pfs->prefix = malloc(length + 1);
    if (NULL == pfs->prefix) {
        perror("Can't allocate space for copy of prefix: malloc");
        free(pfs);
        return NULL;
    }

    // first problem. Comment the next line will remove the first error from the output of infer
    strncpy(pfs->prefix, key, length);
    // second problem. Comment the next line will remove the second error from the output of infer
    pfs->prefix[length] = '\0';      /* because strncpy() sucks */
    pfs->prefix_len = length;

    pfs->next = prefix_stats[hashval];
    prefix_stats[hashval] = pfs;

    num_prefixes++;
    total_prefix_size += length;

    return pfs;
}

/* main function, added */
int main() {
    stats_prefix_init(':');
    stats_prefix_clear();
    PREFIX_STATS* p1 = stats_prefix_find("aaaa:1111", 9);
    printf("%zu", p1->prefix_len);
}