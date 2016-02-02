//
// Created by eugene on 11/7/15.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "ap_int.h"

static void test_addition_overflow_detection(void** state)
{
    uint32_t a = 4294967290;
    uint32_t b = 8;
    uint32_t c = 5;
    uint32_t d = 4294967295;
    uint32_t e = 4294967295;
    uint32_t f = 0;

    assert_true(addition_will_overflow(a,b));
    assert_true(addition_will_overflow(b,a));
    assert_false(addition_will_overflow(a,c));
    assert_false(addition_will_overflow(c,a));
    assert_true(addition_will_overflow(d,e));
    assert_true(addition_will_overflow(e,d));
    assert_false(addition_will_overflow(d,f));
    assert_false(addition_will_overflow(f,d));
}

static void test_multiplication_overflow_detection(void** state)
{
    uint32_t a = 4294967290;
    uint32_t b = 8;
    uint32_t c = 1;
    uint32_t d = 2147483649;
    uint32_t e = 2147483648;

    assert_true(multiplication_will_overflow(a,b));
    assert_true(multiplication_will_overflow(b,a));
    assert_false(multiplication_will_overflow(a,c));
    assert_false(multiplication_will_overflow(c,a));
    assert_true(multiplication_will_overflow(d,3));
    assert_true(multiplication_will_overflow(e,3));
}

static void test_uint32_to_binary_str(void** state)
{
    uint32_t a = 4294967295;
    uint32_t b = 8;
    uint32_t c = 1;
    uint32_t d = 2147483649;
    uint32_t e = 2147483648;
    const char* test;
    test = uint32_to_binary_str(a);
//    printf("%s\n", test);
    assert_string_equal("11111111111111111111111111111111",test);
    free((void*)test);

    test = uint32_to_binary_str(b);
//    printf("%s\n", uint32_to_binary_str(b));
    assert_string_equal("00000000000000000000000000001000",test);
    free((void*)test);

    test = uint32_to_binary_str(c);
//    printf("%s\n", uint32_to_binary_str(c));
    assert_string_equal("00000000000000000000000000000001",test);
    free((void*)test);

    test = uint32_to_binary_str(d);
//    printf("%s\n", uint32_to_binary_str(d));
    assert_string_equal("10000000000000000000000000000001",test);
    free((void*)test);

    test = uint32_to_binary_str(e);
//    printf("%s\n", test);
    assert_string_equal("10000000000000000000000000000000",test);
    free((void*)test);
}

static void test_debug_printing(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
    const char* debug_str = ap_int_to_debug_str(a);
//    printf("%s\n",debug_str);
    assert_string_equal("11111111111111111111111111111111 <-> ", debug_str);
    free((void*)debug_str);

    ap_int_plus_uint32(a, 4294967295);
    debug_str = ap_int_to_debug_str(a);
//    printf("%s\n", debug_str);
    assert_string_equal("00000000000000000000000000000001 <-> 11111111111111111111111111111110 <-> ", debug_str);
    free((void*)debug_str);
    free_ap_int(a);
}

static void test_ap_int_addition(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(a, 1);
//    printf("A: ");
//    print_ap_int(a);
    ap_int* b = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(b, 4294967295);
    ap_int_plus_uint32(b, 4294967295);
//    printf("B: ");
//    print_ap_int(b);
    ap_int* c = ap_int_plus_ap_int(a,b);
//    printf("C: ");
//    print_ap_int(c);
    const char* debug_str = ap_int_to_debug_str(c);
    assert_string_equal("00000000000000000000000000000011 <-> 11111111111111111111111111111101 <-> ", debug_str);
    free((void*)debug_str);
    free_ap_int(a); free_ap_int(b); free_ap_int(c);

    // Testing addition of differently-sized ap-integers
    a = init_ap_int_with_uint32(4294967295);
//    printf("A: ");
//    print_ap_int(a);
    b = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(b, 4294967295);
    ap_int_plus_uint32(b, 4294967295);
//    printf("B: ");
//    print_ap_int(b);
    c = ap_int_plus_ap_int(a,b);
//    printf("C: ");
//    print_ap_int(c);
    debug_str = ap_int_to_debug_str(c);
    assert_string_equal("00000000000000000000000000000011 <-> 11111111111111111111111111111100 <-> ", debug_str);
    free((void*)debug_str);
    free_ap_int(a); free_ap_int(b); free_ap_int(c);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("987230987450129387412341098");
    c = ap_int_plus_ap_int(a,b);
    debug_str = ap_int_to_binary_str(c);
    assert_string_equal(debug_str, "000001100110000100111100100101100010001001000011100110011100011000111001111100101000001011010100");
    free((void*)debug_str);
    free_ap_int(a); free_ap_int(b); free_ap_int(c);
}

static void test_ap_int_shift_left(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_left(a);
//    printf("A: ");
//    print_ap_int(a);
    const char* debug_str = ap_int_to_debug_str(a);
    assert_string_equal("00000000000000000000000000000001 <-> 11111111111111111111111111111110 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);

    // Testing with multiple components
    a = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(a, 4294967295);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_left(a);
//    printf("A: ");
//    print_ap_int(a);
    debug_str = ap_int_to_debug_str(a);
    assert_string_equal("00000000000000000000000000000011 <-> 11111111111111111111111111111100 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);
}

static void test_ap_int_shift_right(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_right(a);
//    printf("A: ");
//    print_ap_int(a);
    const char* debug_str = ap_int_to_debug_str(a);
    assert_string_equal("01111111111111111111111111111111 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);

    // Testing with multiple components
    a = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(a, 4294967295);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_right(a);
//    printf("A: ");
//    print_ap_int(a);
    debug_str = ap_int_to_debug_str(a);
    assert_string_equal("00000000000000000000000000000000 <-> 11111111111111111111111111111111 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);
}

static void test_ap_int_shift_left_by(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_left_by(a, 64);
//    printf("A: ");
//    print_ap_int(a);
    const char* debug_str = ap_int_to_debug_str(a);
    assert_string_equal("11111111111111111111111111111111 <-> 00000000000000000000000000000000 <-> 00000000000000000000000000000000 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);

    // Testing with multiple components
    a = init_ap_int_with_uint32(4294967295);
    ap_int_plus_uint32(a, 4294967295);
    ap_int_shift_left(a);
//    printf("A: ");
//    print_ap_int(a);

    ap_int_shift_left_by(a, 101);
//    printf("A: ");
//    print_ap_int(a);
    debug_str = ap_int_to_debug_str(a);
    assert_string_equal("00000000000000000000000001111111 <-> 11111111111111111111111110000000 <-> 00000000000000000000000000000000 <-> 00000000000000000000000000000000 <-> 00000000000000000000000000000000 <-> ", debug_str);
    free((void*)debug_str); free_ap_int(a);
}

static void test_ap_int_shift_right_by(void** state)
{
    ap_int* a = init_ap_int_with_binary_string("00000010111110111110000011100100110100101111101100110001110001110101000100010101111110011100001011011000111100101111001110010001");
    ap_int_shift_right_by(a, 73);
    const char* str = ap_int_to_binary_str(a);
    assert_string_equal(str, "0000000000000001011111011111000001110010011010010111110110011000");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_uint32(3209845);
//    print_ap_int_bin(a);
    ap_int_shift_right_by(a,12);
    str = ap_int_to_binary_str(a);
    assert_string_equal(str,"00000000000000000000001100001111");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_uint32(3209845);
//    print_ap_int_bin(a);
    ap_int_shift_right_by(a,45);
    str = ap_int_to_binary_str(a);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_binary_string("00000010111110111110000011100100110100101111101100110001110001110101000100010101111110011100001011011000111100101111001110010001");
    ap_int_shift_right_by(a, 1000);
    str = ap_int_to_binary_str(a);
    assert_string_equal(str, "00000000000000000000000000000000");
    free_ap_int(a); free((void*)str);
}

static void test_ap_int_multiplication(void** state)
{
    // 10111110111110000011100100110100101111101100110001110001110101000100010101111110011100001011011000111100101111001110010001
    ap_int* a = init_ap_int_with_uint32(4294967295);
    ap_int* b = init_ap_int_with_uint32(91283749);
    ap_int* c = init_ap_int_with_uint32(2355432189);
    ap_int* d = ap_int_multiply_ap_int(a,a);
    ap_int* e = ap_int_multiply_ap_int(d,b);
    ap_int* f = ap_int_multiply_ap_int(e,c);
//    print_ap_int_bin(f);
    const char* bin_str = ap_int_to_binary_str(f);
    assert_string_equal(bin_str, "00000010111110111110000011100100110100101111101100110001110001110101000100010101111110011100001011011000111100101111001110010001");
    free((void*)bin_str);

    ap_int* g = ap_int_multiply_uint32(a,10);
//    print_ap_int_bin(g);
    bin_str = ap_int_to_binary_str(g);
    assert_string_equal(bin_str, "0000000000000000000000000000100111111111111111111111111111110110");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free_ap_int(e); free_ap_int(f); free_ap_int(g);
    free((void*)bin_str);
}

static void test_ap_int_from_string(void** state)
{
    // Test Decimal String
    ap_int* a = init_ap_int_with_decimal_string("987230987450129387412341098");
    const char* bin_str = ap_int_to_binary_str(a);
//    printf(bin_str);
    assert_string_equal(bin_str, "000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    free((void*)bin_str);
    free_ap_int(a);

    // Test Binary String
    a = init_ap_int_with_binary_string("000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    bin_str = ap_int_to_binary_str(a);
//    printf(bin_str);
    assert_string_equal(bin_str, "000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    free((void*)bin_str);
    free_ap_int(a);
}

static void test_ap_int_subtraction(void** state)
{
    ap_int* a = init_ap_int_with_decimal_string("987230987450129387412341098");
    ap_int* b = init_ap_int_with_decimal_string("987230987450129387412341097");
    ap_int* c = init_ap_int_with_uint32(1);
    ap_int* d = ap_int_minus_ap_int(a,b);
    assert_true(ap_int_equal_to_ap_int(c,d));
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d);

    a = init_ap_int_with_decimal_string("997230987450129387412340098");
    b = init_ap_int_with_decimal_string("987230987450129387412341097");
    c = init_ap_int_with_decimal_string("9999999999999999999999001");
    d = ap_int_minus_ap_int(a,b);
    assert_true(ap_int_equal_to_ap_int(c,d));
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("987230987450129387412341098");
    c = init_ap_int_with_decimal_string("0");
    d = ap_int_minus_ap_int(a,b);
    assert_true(ap_int_equal_to_ap_int(c,d));
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("56298734589302");
    c = init_ap_int_with_decimal_string("987230987450073088677751796");
    d = ap_int_minus_ap_int(a,b);
    assert_true(ap_int_equal_to_ap_int(c,d));
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    d = ap_int_minus_uint32(a, 4294967295);
    c = init_ap_int_with_decimal_string("987230987450129383117373803");
    assert_true(ap_int_equal_to_ap_int(d,c));
    free_ap_int(a); free_ap_int(c); free_ap_int(d);
}

static void test_ap_int_comparison(void** state)
{
    // One Component Tests:
    ap_int* a = init_ap_int_with_uint32(4294967295);
    ap_int* b = init_ap_int_with_uint32(4294967290);
    //a>b
    assert_true(ap_int_greater_than_ap_int(a,b));
    assert_true(ap_int_geq_ap_int(a,b));
    assert_false(ap_int_less_than_ap_int(a,b));
    assert_false(ap_int_leq_ap_int(a,b));
    assert_false(ap_int_equal_to_ap_int(a,b));
    //b<a
    assert_false(ap_int_greater_than_ap_int(b,a));
    assert_false(ap_int_geq_ap_int(b,a));
    assert_true(ap_int_less_than_ap_int(b,a));
    assert_true(ap_int_leq_ap_int(b,a));
    assert_false(ap_int_equal_to_ap_int(b,a));
    //a=a
    assert_false(ap_int_greater_than_ap_int(a,a));
    assert_true(ap_int_geq_ap_int(a,a));
    assert_false(ap_int_less_than_ap_int(a,a));
    assert_true(ap_int_leq_ap_int(a,a));
    assert_true(ap_int_equal_to_ap_int(a,a));
    //b=b
    assert_false(ap_int_greater_than_ap_int(b,b));
    assert_true(ap_int_geq_ap_int(b,b));
    assert_false(ap_int_less_than_ap_int(b,b));
    assert_true(ap_int_leq_ap_int(b,b));
    assert_true(ap_int_equal_to_ap_int(b,b));
    free_ap_int(a); free_ap_int(b);

    // UInt32 Tests
    a = init_ap_int_with_uint32(4294967294);
    uint32_t d = 4294967290;
    uint32_t e = 4294967294;
    uint32_t f = 4294967295;
    //a>d
    assert_true(ap_int_greater_than_uint32(a,d));
    assert_true(ap_int_geq_uint32(a,d));
    assert_false(ap_int_less_than_uint32(a,d));
    assert_false(ap_int_leq_uint32(a,d));
    assert_false(ap_int_equal_to_uint32(a,d));
    //a<f
    assert_false(ap_int_greater_than_uint32(a,f));
    assert_false(ap_int_geq_uint32(a,f));
    assert_true(ap_int_less_than_uint32(a,f));
    assert_true(ap_int_leq_uint32(a,f));
    assert_false(ap_int_equal_to_uint32(a,f));
    //a=e
    assert_false(ap_int_greater_than_uint32(a,e));
    assert_true(ap_int_geq_uint32(a,e));
    assert_false(ap_int_less_than_uint32(a,e));
    assert_true(ap_int_leq_uint32(a,e));
    assert_true(ap_int_equal_to_uint32(a,e));
    free_ap_int(a);

    // Multiple Component Tests:
    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("987230987450129387412341097");
    ap_int* c = init_ap_int_with_decimal_string("9872309874");
    //a>b
    assert_true(ap_int_greater_than_ap_int(a,b));
    assert_true(ap_int_geq_ap_int(a,b));
    assert_false(ap_int_less_than_ap_int(a,b));
    assert_false(ap_int_leq_ap_int(a,b));
    assert_false(ap_int_equal_to_ap_int(a,b));
    //a>c
    assert_true(ap_int_greater_than_ap_int(a,c));
    assert_true(ap_int_geq_ap_int(a,c));
    assert_false(ap_int_less_than_ap_int(a,c));
    assert_false(ap_int_leq_ap_int(a,c));
    assert_false(ap_int_equal_to_ap_int(a,c));
    //b>c
    assert_true(ap_int_greater_than_ap_int(b,c));
    assert_true(ap_int_geq_ap_int(b,c));
    assert_false(ap_int_less_than_ap_int(b,c));
    assert_false(ap_int_leq_ap_int(b,c));
    assert_false(ap_int_equal_to_ap_int(b,c));
    //b<a
    assert_false(ap_int_greater_than_ap_int(b,a));
    assert_false(ap_int_geq_ap_int(b,a));
    assert_true(ap_int_less_than_ap_int(b,a));
    assert_true(ap_int_leq_ap_int(b,a));
    assert_false(ap_int_equal_to_ap_int(b,a));
    //c<a
    assert_false(ap_int_greater_than_ap_int(c,a));
    assert_false(ap_int_geq_ap_int(c,a));
    assert_true(ap_int_less_than_ap_int(c,a));
    assert_true(ap_int_leq_ap_int(c,a));
    assert_false(ap_int_equal_to_ap_int(c,a));
    //c<b
    assert_false(ap_int_greater_than_ap_int(c,b));
    assert_false(ap_int_geq_ap_int(c,b));
    assert_true(ap_int_less_than_ap_int(c,b));
    assert_true(ap_int_leq_ap_int(c,b));
    assert_false(ap_int_equal_to_ap_int(c,b));
    //a=a
    assert_false(ap_int_greater_than_ap_int(a,a));
    assert_true(ap_int_geq_ap_int(a,a));
    assert_false(ap_int_less_than_ap_int(a,a));
    assert_true(ap_int_leq_ap_int(a,a));
    assert_true(ap_int_equal_to_ap_int(a,a));
    //b=b
    assert_false(ap_int_greater_than_ap_int(b,b));
    assert_true(ap_int_geq_ap_int(b,b));
    assert_false(ap_int_less_than_ap_int(b,b));
    assert_true(ap_int_leq_ap_int(b,b));
    assert_true(ap_int_equal_to_ap_int(b,b));
    //c=c
    assert_false(ap_int_greater_than_ap_int(c,c));
    assert_true(ap_int_geq_ap_int(c,c));
    assert_false(ap_int_less_than_ap_int(c,c));
    assert_true(ap_int_leq_ap_int(c,c));
    assert_true(ap_int_equal_to_ap_int(c,c));
    free_ap_int(a); free_ap_int(b); free_ap_int(c);

    // Multiple Component (with many zeroes) Tests:
    a = init_ap_int_with_binary_string("0000000000000000000000000000000000000000000000000000000000000000000000000000000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    b = init_ap_int_with_binary_string("0000000000000000000000000000000000000000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    c = init_ap_int_with_binary_string("0000000000000000000000000000000000000000000110011000010011110010010110001000100100001110011001110001100011100111110010000000101101010");
    //a=b
    assert_false(ap_int_greater_than_ap_int(a,b));
    assert_true(ap_int_geq_ap_int(a,b));
    assert_false(ap_int_less_than_ap_int(a,b));
    assert_true(ap_int_leq_ap_int(a,b));
    assert_true(ap_int_equal_to_ap_int(a,b));
    //a>c
    assert_true(ap_int_greater_than_ap_int(a,c));
    assert_true(ap_int_geq_ap_int(a,c));
    assert_false(ap_int_less_than_ap_int(a,c));
    assert_false(ap_int_leq_ap_int(a,c));
    assert_false(ap_int_equal_to_ap_int(a,c));
    //b>c
    assert_true(ap_int_greater_than_ap_int(b,c));
    assert_true(ap_int_geq_ap_int(b,c));
    assert_false(ap_int_less_than_ap_int(b,c));
    assert_false(ap_int_leq_ap_int(b,c));
    assert_false(ap_int_equal_to_ap_int(b,c));
    //b=a
    assert_false(ap_int_greater_than_ap_int(b,a));
    assert_true(ap_int_geq_ap_int(b,a));
    assert_false(ap_int_less_than_ap_int(b,a));
    assert_true(ap_int_leq_ap_int(b,a));
    assert_true(ap_int_equal_to_ap_int(b,a));
    //c<a
    assert_false(ap_int_greater_than_ap_int(c,a));
    assert_false(ap_int_geq_ap_int(c,a));
    assert_true(ap_int_less_than_ap_int(c,a));
    assert_true(ap_int_leq_ap_int(c,a));
    assert_false(ap_int_equal_to_ap_int(c,a));
    //c<b
    assert_false(ap_int_greater_than_ap_int(c,b));
    assert_false(ap_int_geq_ap_int(c,b));
    assert_true(ap_int_less_than_ap_int(c,b));
    assert_true(ap_int_leq_ap_int(c,b));
    assert_false(ap_int_equal_to_ap_int(c,b));
    //a=a
    assert_false(ap_int_greater_than_ap_int(a,a));
    assert_true(ap_int_geq_ap_int(a,a));
    assert_false(ap_int_less_than_ap_int(a,a));
    assert_true(ap_int_leq_ap_int(a,a));
    assert_true(ap_int_equal_to_ap_int(a,a));
    //b=b
    assert_false(ap_int_greater_than_ap_int(b,b));
    assert_true(ap_int_geq_ap_int(b,b));
    assert_false(ap_int_less_than_ap_int(b,b));
    assert_true(ap_int_leq_ap_int(b,b));
    assert_true(ap_int_equal_to_ap_int(b,b));
    //c=c
    assert_false(ap_int_greater_than_ap_int(c,c));
    assert_true(ap_int_geq_ap_int(c,c));
    assert_false(ap_int_less_than_ap_int(c,c));
    assert_true(ap_int_leq_ap_int(c,c));
    assert_true(ap_int_equal_to_ap_int(c,c));
    free_ap_int(a); free_ap_int(b); free_ap_int(c);

    // Different number of components test
    a = init_ap_int_with_binary_string("00000000000000000110011000010011");
    b = init_ap_int_with_binary_string("0000000000000000001100110011010000010010000100111001010101110110");
    //a<b
    assert_false(ap_int_greater_than_ap_int(a,b));
    assert_false(ap_int_geq_ap_int(a,b));
    assert_true(ap_int_less_than_ap_int(a,b));
    assert_true(ap_int_leq_ap_int(a,b));
    assert_false(ap_int_equal_to_ap_int(a,b));
    //a=a
    assert_false(ap_int_greater_than_ap_int(a,a));
    assert_true(ap_int_geq_ap_int(a,a));
    assert_false(ap_int_less_than_ap_int(a,a));
    assert_true(ap_int_leq_ap_int(a,a));
    assert_true(ap_int_equal_to_ap_int(a,a));
    //b=b
    assert_false(ap_int_greater_than_ap_int(b,b));
    assert_true(ap_int_geq_ap_int(b,b));
    assert_false(ap_int_less_than_ap_int(b,b));
    assert_true(ap_int_leq_ap_int(b,b));
    assert_true(ap_int_equal_to_ap_int(b,b));
    free_ap_int(a); free_ap_int(b);
}

static void test_ap_int_bitwise_construction(void** state)
{
    ap_int* a = init_ap_int_with_uint32(0);
    ap_int_bitwise_iterator* iter = init_ap_bitwise_iterator(a,START);
    for (int i = 10; i; i--)
    {
        set_current_bit_move_more_sig(iter, 1);
        set_current_bit_move_more_sig(iter, 0);
        set_current_bit_move_more_sig(iter, 0);
        set_current_bit_move_more_sig(iter, 1);
        set_current_bit_move_more_sig(iter, 1);
        set_current_bit_move_more_sig(iter, 1);
        set_current_bit_move_more_sig(iter, 0);
        set_current_bit_move_more_sig(iter, 1);
    }
//    print_ap_int(a);
//    print_ap_int(b);
    const char* str = ap_int_to_binary_str(a);
    assert_string_equal(str, "000000000000000010111001101110011011100110111001101110011011100110111001101110011011100110111001");
    free_ap_int(a); free_ap_bitwise_iterator(iter); free((void*)str);

    a = init_ap_int_with_uint32(0);
    iter = init_ap_bitwise_iterator(a, FINISH);
    for (int i = 4; i; i--)
    {
        set_current_bit_move_less_sig(iter, 1);
        set_current_bit_move_less_sig(iter, 0);
        set_current_bit_move_less_sig(iter, 0);
        set_current_bit_move_less_sig(iter, 1);
        set_current_bit_move_less_sig(iter, 1);
        set_current_bit_move_less_sig(iter, 1);
        set_current_bit_move_less_sig(iter, 0);
        set_current_bit_move_less_sig(iter, 1);
    }
    str = ap_int_to_binary_str(a);
    assert_string_equal(str, "10011101100111011001110110011101");
    free_ap_int(a); free_ap_bitwise_iterator(iter); free((void*)str);

    a = init_ap_int_with_binary_string("000000000000000010111001101110011011100110111001101110011011100110111001101110011011100110111001");
    ap_int* b = init_ap_int_with_uint32(0);
    ap_int_bitwise_iterator* iter_a = init_ap_bitwise_iterator(a, START);
    ap_int_bitwise_iterator* iter_b = init_ap_bitwise_iterator(b, START);
    BOOL bit = FALSE;
    while(get_more_significant_bit(iter_a, &bit) && set_current_bit_move_more_sig(iter_b, bit));
    assert_true(ap_int_equal_to_ap_int(a,b));
    free_ap_int(a); free_ap_int(b); free_ap_bitwise_iterator(iter_a); free_ap_bitwise_iterator(iter_b);

    a = init_ap_int_with_binary_string("000000000000000010111001101110011011100110111001101110011011100110111001101110011011100110111001");
    b = init_ap_int_with_uint32(0);
    for (uint64_t i = a->size - 1; i; i--) ap_int_add_component(b,0);
//    print_ap_int(a);
//    print_ap_int(b);
    iter_a = init_ap_bitwise_iterator(a, FINISH);
    iter_b = init_ap_bitwise_iterator(b, FINISH);
    bit = FALSE;
    while(get_less_significant_bit(iter_a, &bit) && set_current_bit_move_less_sig(iter_b, bit));
//    print_ap_int(a);
//    print_ap_int(b);
    assert_true(ap_int_equal_to_ap_int(a,b));
    free_ap_int(a); free_ap_int(b); free_ap_bitwise_iterator(iter_a); free_ap_bitwise_iterator(iter_b);
}

static void test_ap_int_most_significant_bit_index(void** state)
{
    ap_int* a = init_ap_int_with_binary_string("000000000000000010111001101110011011100110111001101110011011100110111001101110011011100110111001");
//    print_ap_int(a);
//    printf("%i\n",most_significant_bit_idx(a));
    assert_int_equal(79,most_significant_bit_idx(a));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("1235321454");
//    print_ap_int(a);
    assert_int_equal(30,most_significant_bit_idx(a));
    free_ap_int(a);
}

static void test_ap_int_division(void** state)
{
    ap_int* a = init_ap_int_with_uint32(12);
    ap_int* b = init_ap_int_with_uint32(4);
    ap_int_division_result* r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    const char* str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"00000000000000000000000000000011");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b); free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(1);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"11111111111111111111111111111111");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b); free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(4);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"00111111111111111111111111111111");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000011");
    free_ap_int(a); free_ap_int(b); free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(5);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"00110011001100110011001100110011");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(134456);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"00000000000000000111110011000111");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000001001100101110111");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(27);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"00001001011110110100001001011110");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000010101");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

////////////////////////////////////////////////////////////////////////////////


    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_uint32(2);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"000000011001100001001111001001011000100010010000111001100111000110001110011111001010000010110101");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);
    
    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_uint32(1);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"000000110011000010011110010010110001000100100001110011001110001100011100111110010100000101101010");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);
    
    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_uint32(5);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"000000001010001101010010110110111101000000111001111101011100011100000101110010110111001101111011");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000011");
    free_ap_int(a); free_ap_int(b);
    free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_decimal_string("987230987450129387412341095");
    b = init_ap_int_with_uint32(5);
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"000000001010001101010010110110111101000000111001111101011100011100000101110010110111001101111011");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b); free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("56298734589302");
    r = ap_int_divide_by_ap_int(a,b);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"0000000000000000000011111111001011010010000010101001010111111100");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"0000000000000000000111100011000101001001000110000111001101000010");
    free((void*)str);
    ap_int* c = init_ap_int_with_decimal_string("987230987450129387412341098");
    ap_int* d = init_ap_int_with_decimal_string("56298734589302");
    assert_true(ap_int_equal_to_ap_int(a,c));
    assert_true(ap_int_equal_to_ap_int(b,d));
    free_ap_int(a); free_ap_int(b); free_ap_int(d); free_ap_int(c);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);

    a = init_ap_int_with_decimal_string("987230987450129387412341095");
    r = ap_int_divide_by_uint32(a,5);
//    print_ap_int(r->quotient);
//    print_ap_int(r->remainder);
    str = ap_int_to_binary_str(r->quotient);
    assert_string_equal(str,"000000001010001101010010110110111101000000111001111101011100011100000101110010110111001101111011");
    free((void*)str);
    str = ap_int_to_binary_str(r->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free((void*)str);
    free_ap_int(r->remainder); free_ap_int(r->quotient); free(r);
}

static void test_ap_int_to_decimal_string(void** state)
{
    ap_int* a = init_ap_int_with_uint32(8);
    const char* str = ap_int_to_decimal_str(a,1);
    assert_string_equal(str,"8");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_uint32(4294967295);
    str = ap_int_to_decimal_str(a,10);
    assert_string_equal(str,"4294967295");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    str = ap_int_to_decimal_str(a,27);
    assert_string_equal(str,"987230987450129387412341098");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_uint32(0);
    str = ap_int_to_decimal_str(a,1);
    assert_string_equal(str,"0");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_uint32(1);
    str = ap_int_to_decimal_str(a,1);
    assert_string_equal(str,"1");
    free_ap_int(a); free((void*)str);

    a = init_ap_int_with_decimal_string("9872309874501295687623452345234566543870932485209384241433234109892384750932875093248750293487520934750943875320945");
    str = ap_int_to_decimal_str(a,100);
    assert_string_equal(str,"truncated...5687623452345234566543870932485209384241433234109892384750932875093248750293487520934750943875320945");
    free_ap_int(a); free((void*)str);
}

static void test_ap_int_modulus(void** state)
{
    ap_int* a = init_ap_int_with_uint32(8);
    ap_int* b = init_ap_int_with_uint32(2);
    ap_int* mod = ap_int_mod_ap_int(a,b);
    const char* str = ap_int_to_binary_str(mod);
    assert_string_equal(str,"00000000000000000000000000000000");
    free_ap_int(a); free_ap_int(b); free_ap_int(mod); free((void*)str);

    a = init_ap_int_with_uint32(8);
    b = init_ap_int_with_uint32(3);
    mod = ap_int_mod_ap_int(a,b);
    str = ap_int_to_binary_str(mod);
    assert_string_equal(str,"00000000000000000000000000000010");
    free_ap_int(a); free_ap_int(b); free_ap_int(mod); free((void*)str);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(27);
    mod = ap_int_mod_ap_int(a,b);
    str = ap_int_to_binary_str(mod);
    assert_string_equal(str,"00000000000000000000000000010101");
    free_ap_int(a); free_ap_int(b); free_ap_int(mod); free((void*)str);

    //17535580411388×56298734589302+33197028569922
    //987230987450129387412341098
    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    b = init_ap_int_with_decimal_string("56298734589302");
    ap_int* d = init_ap_int_with_decimal_string("33197028569922");
    mod = ap_int_mod_ap_int(a,b);
    assert_true(ap_int_equal_to_ap_int(d,mod));
    free_ap_int(a); free_ap_int(b); free_ap_int(mod); free_ap_int(d);

    a = init_ap_int_with_uint32(4294967295);
    assert_int_equal(21, ap_int_mod_uint32(a, 27));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    assert_int_equal(827042968, ap_int_mod_uint32(a, 4294967295));
    free_ap_int(a);
}

static void test_ap_int_is_odd(void** state)
{
    ap_int* a = init_ap_int_with_uint32(8);
    assert_false(ap_int_is_odd(a));
    free_ap_int(a);

    a = init_ap_int_with_uint32(12345);
    assert_true(ap_int_is_odd(a));
    free_ap_int(a);

    a = init_ap_int_with_uint32(4294967295);
    assert_true(ap_int_is_odd(a));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    assert_false(ap_int_is_odd(a));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("982984509432852348751342134235412341234231");
    assert_true(ap_int_is_odd(a));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("348925515950656637849431999905261488");
    assert_false(ap_int_is_odd(a));
    free_ap_int(a);
}

static void test_ap_int_modular_exponentiation(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4);
    ap_int* b = init_ap_int_with_uint32(13);
    ap_int* c = init_ap_int_with_uint32(497);
    ap_int* d = ap_int_modular_exponentiation(a, b, c);
    const char* str = ap_int_to_decimal_str(d,3);
    assert_string_equal(str,"445");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free((void*)str);

    a = init_ap_int_with_uint32(4);
    b = init_ap_int_with_uint32(0);
    c = init_ap_int_with_uint32(497);
    d = ap_int_modular_exponentiation(a, b, c);
    str = ap_int_to_decimal_str(d,1);
    assert_string_equal(str,"1");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free((void*)str);

    a = init_ap_int_with_uint32(4294967295);
    b = init_ap_int_with_uint32(3456);
    c = init_ap_int_with_uint32(2346453765);
    d = ap_int_modular_exponentiation(a, b, c);
    str = ap_int_to_decimal_str(d,8);
    assert_string_equal(str,"27162135");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free((void*)str);

    a = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    b = init_ap_int_with_decimal_string("98324750923847502");
    c = init_ap_int_with_decimal_string("3211324234234324924102489098321");
    d = ap_int_modular_exponentiation(a, b, c);
    str = ap_int_to_decimal_str(d,31);
    assert_string_equal(str,"2146333580357975467844001322588");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free((void*)str);

    a = init_ap_int_with_decimal_string("341550071728321");
    b = init_ap_int_with_decimal_string("85387517932080");
    c = init_ap_int_with_decimal_string("338940394193814");
    d = ap_int_modular_exponentiation(a, b, c);
    str = ap_int_to_decimal_str(d,13);
    assert_string_equal(str,"3578598472555");
    free_ap_int(a); free_ap_int(b); free_ap_int(c); free_ap_int(d); free((void*)str);
}

static void test_random_int(void** state)
{
    uint32_t r;
    uint32_t* counters = (uint32_t*)calloc(100,sizeof(uint32_t));
    for (uint32_t i = 1000000; i; i--)
    {
        r = random(0, 100);
        counters[r]++;
//        printf("RANDOM: %i\n", r);
        assert_true((r >= 0 && r <= 99));
    }
    free(counters);

    // Uncomment the following to visually confirm randomness distribution
//    for (uint32_t i = 99; i; i--)
//    {
//        printf("[%i] -> %i\n",i,counters[i]);
//    }
//    printf("[%i] -> %i\n",0,counters[0]);

    uint64_t r64;
    uint64_t* counters64 = (uint64_t*)calloc(100,sizeof(uint64_t));
    for (uint32_t i = 1000000; i; i--)
    {
        r64 = random64(0, 100);
        counters64[r64]++;
//        printf("RANDOM: %i\n", r);
        assert_true((r64 >= 0 && r64 <= 99));
    }
    free(counters64);

    // Uncomment the following to visually confirm randomness distribution
//    for (uint32_t i = 99; i; i--)
//    {
//        printf("[%i] -> %i\n",i,counters[i]);
//    }
//    printf("[%i] -> %i\n",0,counters[0]);
}

static void test_random_ap_int(void** state)
{
    // How the heck do I test a random int??
    ap_int* a = basic_random_ap_int_of_maxsize(3,0);
    //printf("SIZE: %lu\n", a->size);
    //print_ap_int(a);
    free_ap_int(a);

    a = basic_random_ap_int_of_maxsize(128,23);
    //printf("SIZE: %lu\n", a->size);
    //print_ap_int(a);
    free_ap_int(a);

    ap_int* max = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    ap_int* min = init_ap_int_with_decimal_string("98324750923847502");
    for (uint32_t i = 100000; i; i--)
    {
        a = random_ap_int_in_range(min,max);
        assert_true(ap_int_geq_ap_int(a,min));
        assert_true(ap_int_leq_ap_int(a,max));
        free_ap_int(a);
    }
    free_ap_int(max); free_ap_int(min);

    max = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    min = init_ap_int_with_decimal_string("1");
    for (uint32_t i = 100000; i; i--)
    {
        a = random_ap_int_in_range(min,max);
        assert_true(ap_int_geq_ap_int(a,min));
        assert_true(ap_int_leq_ap_int(a,max));
        free_ap_int(a);
    }
    free_ap_int(max); free_ap_int(min);

    max = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    min = init_ap_int_with_decimal_string("0");
    for (uint32_t i = 100000; i; i--)
    {
        a = random_ap_int_in_range(min,max);
        assert_true(ap_int_geq_ap_int(a,min));
        assert_true(ap_int_leq_ap_int(a,max));
        free_ap_int(a);
    }
    free_ap_int(max); free_ap_int(min);

    max = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
    min = init_ap_int_with_decimal_string("982984509432852348751342134235412341234231");
    for (uint32_t i = 100000; i; i--)
    {
        a = random_ap_int_in_range(min,max);
        assert_true(ap_int_geq_ap_int(a,min));
        assert_true(ap_int_leq_ap_int(a,max));
        free_ap_int(a);
    }
    free_ap_int(max); free_ap_int(min);
}

static void test_division_by_two(void** state)
{
    ap_int* a = init_ap_int_with_uint32(4294967295);
    //2147483647×2+1
    ap_int_division_result* d = ap_int_divide_by_two(a);
    assert_true(ap_int_equal_to_uint32(d->quotient, 2147483647));
    assert_true(ap_int_equal_to_uint32(d->remainder,1));
    free_ap_int(a); free_ap_int(d->remainder); free_ap_int(d->quotient); free(d);

    a = init_ap_int_with_decimal_string("987230987450129387412341098");
    d = ap_int_divide_by_two(a);
    const char* str = ap_int_to_binary_str(d->quotient);
    assert_string_equal(str,"000000011001100001001111001001011000100010010000111001100111000110001110011111001010000010110101");
    free((void*) str);
    str = ap_int_to_binary_str(d->remainder);
    assert_string_equal(str,"00000000000000000000000000000000");
    free((void*)str);
    free_ap_int(a); free_ap_int(d->remainder); free_ap_int(d->quotient); free(d);
}

static void test_ap_int_incrementing_decrementing(void** state)
{
    // NOTE: Very time-consuming and computationally intensive test. Disable unless necessary
    ap_int* a = init_ap_int_with_uint32(1);
    ap_int* c = init_ap_int_with_uint32(0);
    ap_int_increment(c); ap_int_decrement(a);
    assert_true(ap_int_equal_to_uint32(c,1));
    assert_true(ap_int_equal_to_uint32(a,0));
    free_ap_int(a); free_ap_int(c);

    a = init_ap_int_with_decimal_string("4294968295");
    c = init_ap_int_with_uint32(0);
    for (; ap_int_greater_than_uint32(a,0); ap_int_decrement(a))
    {
        ap_int_increment(c);
    }
    const char* strc = ap_int_to_decimal_str(c,10);
    assert_string_equal(strc,"4294968295");
    free_ap_int(a); free_ap_int(c);
    free((void*)strc);
}

static void test_MR_primality_of_ap_int(void** state)
{
    ap_int* a = init_ap_int_with_uint32(982451653);
    assert_true(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_uint32(677);
    assert_true(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("32416187567");
    assert_true(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("29996224275833");
    assert_true(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_uint32(678);
    assert_false(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_uint32(982451657);
    assert_false(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("32416187569");
    assert_false(probably_prime_MR(a, 5));
    free_ap_int(a);

    a = init_ap_int_with_decimal_string("67811856367");
    assert_false(probably_prime_MR(a, 5));
    free_ap_int(a);

    // There should be 78,498 primes in the first 1,000,000 integers
    uint32_t c = 0;
    for (uint32_t i = 0; i < 1000000; i++)
    {
        a = init_ap_int_with_uint32(i);
        if (probably_prime_MR(a,10))
        {
            c++;
        }
        // uncomment following line for visual confirmation
//        if (i % 1000 == 0) printf("TESTED: %i -> %i\n",i,c);
        free_ap_int(a);
    }
    assert_true(c >= 78498);
}

static void test_generating_random_factored_ap_int(void** state)
{
    ap_int* min = init_ap_int_with_uint32(0);
    ap_int* max = init_ap_int_with_decimal_string("1342134235412341234232");
    ap_int_prime_factorization* f = random_factored_number(max, 10);

//    printf("RESULT(%i): ", f->num_factors);
//    print_ap_int(f->number);
    assert_true(ap_int_leq_ap_int(f->number, max) && ap_int_geq_ap_int(f->number, min));
    free_ap_int(min); free_ap_int(max);
    for (uint32_t i = 0; i < f->num_factors; i++)
    {
//        printf("FACTOR: ");
//        print_ap_int(f->prime_factors[i]);
        assert_true(probably_prime_MR(f->prime_factors[i], 100));
        free_ap_int(f->prime_factors[i]);
    }
    free(f->prime_factors);
    free_ap_int(f->number);
    free(f);
}

static void test_generating_random_prime_factorization(void** state)
{
    ap_int* max = init_ap_int_with_decimal_string("1342134235412341234232");
    ap_int_prime_factorization* f;

    for (uint32_t i = 0; i < 5; i++)
    {
        f = random_prime_factorization(max, 10);

//        printf("RESULT(%i): ", f->num_factors);
//        print_ap_int(f->number);
        assert_true(ap_int_leq_ap_int(f->number, max));
        assert_true(probably_prime_MR(f->number, 10));
        for (uint32_t j = 0; j < f->num_factors; j++)
        {
            assert_true(probably_prime_MR(f->prime_factors[j], 10));
        }
        assert(f->num_factors > 0);
        free_ap_int(f->number);
        for (uint32_t j = 0; j < f->num_factors; j++)
        {
            free_ap_int(f->prime_factors[j]);
        }
        free(f->prime_factors);
        free(f);
    }
    free_ap_int(max);
}

static void test_prg(void** state)
{
    ap_int* a = init_ap_int_with_decimal_string("1342134235412341234232");
    ap_int* pseudorandom = prg(a,20);
    const char* str = ap_int_to_binary_str(pseudorandom);
    printf("PSEUDORANDOM: %s\n", str);
    assert_true(strlen(str) == 32);
    free((void*)str); free_ap_int(pseudorandom);

    pseudorandom = prg(a,10000);
    str = ap_int_to_binary_str(pseudorandom);
    printf("PSEUDORANDOM: %s\n", str);
    assert_true(strlen(str) == 10016);
    free((void*)str); free_ap_int(pseudorandom);

    free_ap_int(a);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_addition_overflow_detection),
            cmocka_unit_test(test_multiplication_overflow_detection),
            cmocka_unit_test(test_uint32_to_binary_str),
            cmocka_unit_test(test_debug_printing),
            cmocka_unit_test(test_ap_int_addition),
            cmocka_unit_test(test_ap_int_subtraction),
            cmocka_unit_test(test_ap_int_shift_left),
            cmocka_unit_test(test_ap_int_shift_right),
            cmocka_unit_test(test_ap_int_shift_left_by),
            cmocka_unit_test(test_ap_int_multiplication),
            cmocka_unit_test(test_ap_int_from_string),
            cmocka_unit_test(test_ap_int_comparison),
            cmocka_unit_test(test_ap_int_bitwise_construction),
            cmocka_unit_test(test_ap_int_most_significant_bit_index),
            cmocka_unit_test(test_ap_int_shift_right_by),
            cmocka_unit_test(test_ap_int_division),
            cmocka_unit_test(test_ap_int_to_decimal_string),
            cmocka_unit_test(test_ap_int_modulus),
            cmocka_unit_test(test_ap_int_is_odd),
            cmocka_unit_test(test_ap_int_modular_exponentiation),
            cmocka_unit_test(test_division_by_two),
//            cmocka_unit_test(test_random_int), // Very time consuming, disable unless necessary
//            cmocka_unit_test(test_random_ap_int), // Very time consuming, disable unless necessary
//            cmocka_unit_test(test_ap_int_incrementing_decrementing), // Very time consuming, disable unless necessary
//            cmocka_unit_test(test_MR_primality_of_ap_int), // Very time consuming, disable unless necessary
//            cmocka_unit_test(test_generating_random_factored_ap_int), // Very time consuming, disable unless necessary
//            cmocka_unit_test(test_generating_random_prime_factorization), // Very time consuming, disable unless necessary
            cmocka_unit_test(test_prg), // Very time consuming, disable unless necessary
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}