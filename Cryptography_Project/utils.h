//
// Created by eugene on 11/11/15.
//

#ifndef CRYPTO_PROJECT_UTILS_H
#define CRYPTO_PROJECT_UTILS_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const char* uint32_to_binary_str(uint32_t x);
void reverse(char *string);
uint32_t random(uint32_t min, uint32_t max);
uint64_t random64(uint64_t min, uint64_t max);
uint8_t random8(uint8_t min, uint8_t max);

typedef struct int_arraylist_t
{
    uint32_t size;
    uint32_t actual_size;
    uint32_t* array;
} int_arraylist;

// Arraylist functions
int_arraylist* init_arraylist();
void arraylist_append(int_arraylist* list, uint32_t integer);
uint32_t arraylist_get(int_arraylist* list, uint32_t index);
void free_arraylist(int_arraylist* list);

#endif //CRYPTO_PROJECT_UTILS_H
