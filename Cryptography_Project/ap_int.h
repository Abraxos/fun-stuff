//
// Created by eugene on 11/7/15.
//

#ifndef CRYPTO_PROJECT_AP_INT_H
#define CRYPTO_PROJECT_AP_INT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/param.h>
#include <assert.h>

#include "utils.h"

// Definitions
#define BOOL char
#define FALSE                        0
#define TRUE                         1
#define START                        0
#define FINISH                       1
#define COMPOSITE                    0
#define PROBABLY_PRIME               1
#define NUM_FACTORIZATION_CANDIDATES 100

// Arbitrary Precision Integer Structs
typedef struct ap_int_component_t
{
    struct ap_int_component_t* more_significant;
    struct ap_int_component_t* less_significant;
    uint32_t integer;
} ap_int_component;

typedef struct ap_int_t
{
    ap_int_component* most_significant;
    ap_int_component* least_significant;
    uint64_t size;
} ap_int;

typedef struct ap_int_bitwise_iterator_t
{
    ap_int* a;
    ap_int_component* current_component;
    uint8_t i;
} ap_int_bitwise_iterator;

typedef struct ap_int_division_result_t
{
    ap_int* quotient;
    ap_int* remainder;
} ap_int_division_result;

typedef struct ap_int_prime_factorization_t
{
    ap_int* number;
    uint32_t num_factors;
    ap_int** prime_factors;
} ap_int_prime_factorization;

// Arbitrary Precision Integer Functions
BOOL addition_will_overflow(uint32_t a, uint32_t b);
BOOL multiplication_will_overflow(uint32_t a, uint32_t b);
const char* ap_int_to_debug_str(ap_int* a);
const char* ap_int_to_binary_str(ap_int* a);
const char* ap_int_to_decimal_str(ap_int* a, uint64_t size);
uint64_t most_significant_bit_idx(ap_int* a);
void strip_significant_zeroes_from_ap_int(ap_int *a);
void print_ap_int(ap_int* a);
void print_ap_int_bin(ap_int* a);

//Bitwise Iterator Functions
BOOL get_more_significant_bit(ap_int_bitwise_iterator *iter, BOOL *result);
BOOL set_current_bit_move_more_sig(ap_int_bitwise_iterator *iter, BOOL bit);
BOOL get_less_significant_bit(ap_int_bitwise_iterator *iter, BOOL *result);
BOOL set_current_bit_move_less_sig(ap_int_bitwise_iterator *iter, BOOL bit);
ap_int_bitwise_iterator* init_ap_bitwise_iterator(ap_int* a, BOOL starting_point);
void free_ap_bitwise_iterator(ap_int_bitwise_iterator* iter);

//Arithmetic Operations
void ap_int_shift_left(ap_int* a);
void ap_int_shift_right(ap_int* a);
void ap_int_shift_left_by(ap_int* a, uint64_t s);
void ap_int_shift_right_by(ap_int* a, uint64_t s);
void ap_int_increment(ap_int* a);
void ap_int_plus_uint32(ap_int* a, uint32_t b);
BOOL ap_int_component_plus_uint32(ap_int_component* ac, uint32_t b);
BOOL ap_int_is_odd(ap_int* a);
ap_int* ap_int_mod_ap_int(ap_int* a, ap_int* b);
uint32_t ap_int_mod_uint32(ap_int* a, uint32_t b);
ap_int* ap_int_plus_ap_int(ap_int* a, ap_int* b);
void ap_int_decrement(ap_int* a);
ap_int* ap_int_minus_ap_int(ap_int* a, ap_int* b);
ap_int* ap_int_minus_uint32(ap_int* a, uint32_t b);
ap_int* ap_int_multiply_ap_int(ap_int* a, ap_int* b);
ap_int* ap_int_multiply_uint32(ap_int* a, uint32_t b);
ap_int_division_result* ap_int_divide_by_two(ap_int* a);
ap_int_division_result* ap_int_divide_by_ap_int(ap_int* dividend, ap_int* divisor);
ap_int_division_result* ap_int_divide_by_uint32(ap_int* dividend, uint32_t divisor);
ap_int* ap_int_modular_exponentiation(ap_int* base, ap_int* exponent, ap_int* modulus);

//Comparison Function
BOOL ap_int_greater_than_ap_int(ap_int* a, ap_int* b);
BOOL ap_int_less_than_ap_int(ap_int* a, ap_int* b);
BOOL ap_int_equal_to_ap_int(ap_int* a, ap_int* b);
BOOL ap_int_geq_ap_int(ap_int* a, ap_int* b);
BOOL ap_int_leq_ap_int(ap_int* a, ap_int* b);
BOOL ap_int_greater_than_uint32(ap_int* a, uint32_t b);
BOOL ap_int_less_than_uint32(ap_int* a, uint32_t b);
BOOL ap_int_equal_to_uint32(ap_int* a, uint32_t b);
BOOL ap_int_geq_uint32(ap_int* a, uint32_t b);
BOOL ap_int_leq_uint32(ap_int* a, uint32_t b);

//Crypto-Specific Functions
BOOL probably_prime_MR(ap_int* n, uint64_t k);
ap_int_prime_factorization* random_factored_number(ap_int* max, uint64_t prime_certainty_index);
ap_int_prime_factorization* random_prime_factorization(ap_int* max, uint64_t prime_certainty_index);
ap_int* prg(ap_int* N, uint64_t num_bits_to_generate);

//Constructors
void ap_int_add_component(ap_int* a, uint32_t integer);
ap_int* init_ap_int_with_uint32(uint32_t i);
ap_int* init_ap_int_with_decimal_string(const char* str);
ap_int* init_ap_int_with_binary_string(const char* str);
ap_int* basic_random_ap_int_of_maxsize(uint64_t max_num_components, uint8_t max_num_bits_in_msc);
ap_int* random_ap_int_in_range(ap_int* max, ap_int* min);
ap_int* generator_for_prime_ap_int(ap_int_prime_factorization* f);
ap_int* copy_ap_int(ap_int* a);

//Destructors
void free_ap_int(ap_int* a);

#endif //CRYPTO_PROJECT_AP_INT_H
