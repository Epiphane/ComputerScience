/*
 * Malloc custom library
 * CPE 453 Assignment 1
 *
 * Thomas Steinke (tsteinke@calpoly.edu)
 */

#include <stddef.h>

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
