//
// Created by eugene on 11/11/15.
//

#include "utils.h"

/**
 * Given an unsigned 32 bit integer, this function returns an ASCII string
 * representation of the binary data inside the integer. This function is mostly
 * for debugging purposes.
 */
const char* uint32_to_binary_str(uint32_t x)
{
    char* b = (char*)calloc(sizeof(char),33);
    for (int i = 0; i < 32; i++)
    {
        strcat(b, (x & 2147483648) ? "1" : "0");
        x = x << 1;
    }

    return b;
}

void reverse(char *string)
{
    int length, c;
    char *begin, *end, temp;

    length = (int)strlen(string);
    begin  = string;
    end    = string;

    for (c = 0; c < length - 1; c++)
        end++;

    for (c = 0; c < length/2; c++)
    {
        temp   = *end;
        *end   = *begin;
        *begin = temp;

        begin++;
        end--;
    }
}

uint32_t random(uint32_t min, uint32_t max)
{
    uint32_t byte1 = (uint32_t)(rand() % 255) << 24;
    uint32_t byte2 = (uint32_t)(rand() % 255) << 16;
    uint32_t byte3 = (uint32_t)(rand() % 255) << 8;
    uint32_t byte4 = (uint32_t)(rand() % 255);
    uint32_t res = byte1 | byte2 | byte3 | byte4;
    uint8_t shift = (uint8_t)(rand() % 31);
    return (uint32_t)(res % (max - min) + min) >> shift;
}

uint64_t random64(uint64_t min, uint64_t max)
{
    uint64_t byte1 = (uint64_t)(rand() % 255) << 56;
    uint64_t byte2 = (uint64_t)(rand() % 255) << 48;
    uint64_t byte3 = (uint64_t)(rand() % 255) << 40;
    uint64_t byte4 = (uint64_t)(rand() % 255) << 32;
    uint64_t byte5 = (uint64_t)(rand() % 255) << 24;
    uint64_t byte6 = (uint64_t)(rand() % 255) << 16;
    uint64_t byte7 = (uint64_t)(rand() % 255) << 8;
    uint64_t byte8 = (uint64_t)(rand() % 255);
    uint64_t res = byte1 | byte2 | byte3 | byte4 | byte5 | byte6 | byte7 | byte8;
    return (uint64_t)(res % (max - min) + min);
}

uint8_t random8(uint8_t min, uint8_t max)
{
    return (uint8_t)(rand() % (max - min) + min);
}

// Arraylist functions
int_arraylist* init_arraylist()
{
    int_arraylist* list = (int_arraylist*)malloc(sizeof(int_arraylist));
    list->actual_size = 2;
    list->size = 0;
    list->array = (uint32_t*)malloc(sizeof(uint32_t) * list->actual_size);
    return list;
}
void free_arraylist(int_arraylist* list)
{
    free(list->array);
    free(list);
}
void arraylist_append(int_arraylist* list, uint32_t integer)
{
    if (list->size == list->actual_size)
    {
        list->actual_size *= 2;
        uint32_t* temp = (uint32_t*)malloc(sizeof(uint32_t) * list->actual_size);
        for (uint32_t i = 0; i < list->actual_size / 2; i++) temp[i] = list->array[i];
        free(list->array);
        list->array = temp;
    }
    list->array[list->size++] = integer;
}
uint32_t arraylist_get(int_arraylist* list, uint32_t index)
{
    if (index < list->size)
    {
        return list->array[index];
    }
    return 0;
}