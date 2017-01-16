//
// Created by Borchers, Henry Samuel on 1/15/17.
//

#include <stdlib.h>
#include <stdio.h>


void *d_debug_mem_malloc(size_t size, char *file, size_t line) {
    fprintf(stderr, "Malloc called at %s on line %zu\n", file, line);
    return malloc(size);
}
void *d_debug_mem_calloc(size_t size, size_t num, char *file, size_t line) {
    fprintf(stderr, "calloc called at %s on line %zu\n", file, line);
    return calloc(size, num);
}
