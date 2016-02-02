//
// Created by eugene on 11/7/15.
//

#include "ap_int.h"

BOOL addition_will_overflow(uint32_t a, uint32_t b)
{
    if (a == 0 || b == 0) return FALSE;
    else return (a + b <= MAX(a,b));
}

BOOL multiplication_will_overflow(uint32_t a, uint32_t b)
{
    if (a == 0 || b == 0 || a == 1 || b == 1) return FALSE;
    else {
        uint32_t c = a * b;
        return (c / a != b);
    }
}

ap_int* init_ap_int_with_uint32(uint32_t i)
{
    ap_int* a = (ap_int*)malloc(sizeof(ap_int));
    ap_int_component* ac = (ap_int_component*)malloc(sizeof(ap_int_component));
    ac->integer = i;
    ac->less_significant = NULL;
    ac->more_significant = NULL;
    a->least_significant = ac;
    a->most_significant = ac;
    a->size = 1;
    return a;
}

ap_int* init_ap_int_with_decimal_string(const char* str)
{
    ap_int* a = init_ap_int_with_uint32(0);
    ap_int* b;
    uint32_t c;

    // Iterate through all the characters of the input string and update the result
    for (int i = 0; str[i] != '\0'; ++i)
    {
        c = (uint32_t)(str[i] - '0');
        b = ap_int_multiply_uint32(a,10);
        ap_int_plus_uint32(b, c);
        free_ap_int(a);
        a = b;
    }
    return a;
}

ap_int* init_ap_int_with_binary_string(const char* str)
{
    uint64_t str_len = strlen(str);
    BOOL new_bit = (str[0] == '1') ? TRUE : FALSE;
    ap_int* res = init_ap_int_with_uint32((uint32_t)new_bit);
    for (uint64_t i = 1; i < str_len; i++)
    {
        new_bit = (str[i] == '1') ? TRUE : FALSE;
        ap_int_shift_left(res);
        res->least_significant->integer |= new_bit;
    }
    return res;
}

BOOL ap_int_component_plus_uint32(ap_int_component* ac, uint32_t b)
{
    uint32_t a = ac->integer;
    ac->integer = b + a;
    return addition_will_overflow(b, a);
}

void ap_int_add_component(ap_int* a, uint32_t integer)
{
    ap_int_component* new_component = (ap_int_component*)malloc(sizeof(ap_int_component));
    new_component->integer = integer;
    new_component->more_significant = NULL;
    new_component->less_significant = a->most_significant;
    a->most_significant->more_significant = new_component;
    a->most_significant = new_component;
    a->size ++;
}

BOOL set_current_bit_move_more_sig(ap_int_bitwise_iterator *iter, BOOL bit) // runtime O(1)
{
    uint32_t value = (uint32_t)bit;
    if (iter->current_component == NULL) // create a new, more significant component
    {
        ap_int_add_component(iter->a, value);
        iter->current_component = iter->a->most_significant;
        iter->i = 1;
        return TRUE;
    }
    else
    {
        iter->current_component->integer |= value << iter->i;
        iter->i++;
        if (iter->i == 32)
        {
            iter->i = 0;
            iter->current_component = iter->current_component->more_significant;
        }
        return TRUE;
    }
}

BOOL set_current_bit_move_less_sig(ap_int_bitwise_iterator *iter, BOOL bit) // runtime O(1)
{
    uint32_t value = (uint32_t)bit;
    if (iter->current_component == NULL) return FALSE;
    else
    {
        iter->current_component->integer |= value << iter->i;
        if (iter->i == 0)
        {
            iter->i = 31;
            iter->current_component = iter->current_component->less_significant;
        }
        else iter->i--;

        return TRUE;
    }
}

BOOL get_more_significant_bit(ap_int_bitwise_iterator *iter, BOOL *result) // runtime O(1)
{
    if (iter->current_component == NULL) return FALSE;
    uint32_t bit;
    uint32_t mask = (uint32_t)1 << iter->i;
    bit = (iter->current_component->integer & mask) >> iter->i;
    iter->i++;
    if (iter->i == 32)
    {
        iter->i = 0;
        iter->current_component = iter->current_component->more_significant;
    }
    (*result) = (BOOL)bit;
    return TRUE;
}

BOOL get_less_significant_bit(ap_int_bitwise_iterator *iter, BOOL *result) // runtime O(1)
{
    if (iter->current_component == NULL) return FALSE;
    uint32_t bit;
    uint32_t mask = (uint32_t)1 << iter->i;
    bit = (iter->current_component->integer & mask) >> iter->i;
    if (iter->i == 0)
    {
        iter->i = 31;
        iter->current_component = iter->current_component->less_significant;
    }
    else
    {
        iter->i--;
    }
    (*result) = (BOOL)bit;
    return TRUE;
}

ap_int_bitwise_iterator* init_ap_bitwise_iterator(ap_int* a, BOOL starting_point)
{
    ap_int_bitwise_iterator* iter = (ap_int_bitwise_iterator*)malloc(sizeof(ap_int_bitwise_iterator));
    iter->a = a;
    if (starting_point == START)
    {
        iter->current_component = a->least_significant;
        iter->i = 0;
    }
    else if (starting_point == FINISH)
    {
        iter->current_component = a->most_significant;
        iter->i = 31;
    }
    return iter;
}

void free_ap_bitwise_iterator(ap_int_bitwise_iterator* iter)
{
    free(iter);
}

void ap_int_plus_uint32(ap_int* a, uint32_t b) // runtime O(n)
{
    ap_int_component* ac = a->least_significant;

    while(ac && ap_int_component_plus_uint32(ac, b))
    {
        ac = ac->more_significant;
        if (!ac)
        {
            ap_int_add_component(a,1);
        }
        b = 1;
    }
}

ap_int* ap_int_plus_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    ap_int* c = init_ap_int_with_uint32(0);
    // loop over the components of a and b simultaneously from least to most significant
    ap_int_component* ca = a->least_significant, * cb = b->least_significant, * cc = c->least_significant;
    BOOL overflow;
    while (ca || cb)
    {
        if (ca)
        {
            overflow = ap_int_component_plus_uint32(cc, ca->integer);
            if (overflow)
            {
                ap_int_add_component(c,1);
            }
            ca = ca->more_significant;
        }
        if (cb)
        {
            overflow = ap_int_component_plus_uint32(cc, cb->integer);
            if (overflow)
            {
                if (!cc->more_significant)
                {
                    ap_int_add_component(c,0);
                }
                overflow = ap_int_component_plus_uint32(cc->more_significant,1);
                // this operation should never result in an overflow since at this point the maximum value of cc->more_significant is 2
                assert(!overflow);
            }
            cb = cb->more_significant;
        }
        if ((ca || cb) && !cc->more_significant)
        {
            ap_int_add_component(c,0);
        }
        cc = cc->more_significant;
    }
    return c;
}

BOOL ap_int_greater_than_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    strip_significant_zeroes_from_ap_int(a);
    strip_significant_zeroes_from_ap_int(b);
    if (a->size > b->size) return TRUE;
    else if (b->size > a->size) return FALSE;

    ap_int_component* ac = a->most_significant;
    ap_int_component* bc = b->most_significant;

    while(ac && bc)
    {
        if (ac->integer > bc->integer) return TRUE;
        else if (ac->integer < bc->integer) return FALSE;
        else
        {
            ac = ac->less_significant;
            bc = bc->less_significant;
        }
    }

    return FALSE;
}

BOOL ap_int_less_than_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    strip_significant_zeroes_from_ap_int(a);
    strip_significant_zeroes_from_ap_int(b);
    if (a->size > b->size) return FALSE;
    else if (b->size > a->size) return TRUE;

    ap_int_component* ac = a->most_significant;
    ap_int_component* bc = b->most_significant;

    while(ac && bc)
    {
        if (ac->integer > bc->integer) return FALSE;
        else if (ac->integer < bc->integer) return TRUE;
        else
        {
            ac = ac->less_significant;
            bc = bc->less_significant;
        }
    }

    return FALSE;
}

BOOL ap_int_equal_to_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    strip_significant_zeroes_from_ap_int(a);
    strip_significant_zeroes_from_ap_int(b);
    if (a->size != b->size) return FALSE;

    ap_int_component* ac = a->most_significant;
    ap_int_component* bc = b->most_significant;

    while(ac && bc)
    {
        if (ac->integer != bc->integer) return FALSE;
        else
        {
            ac = ac->less_significant;
            bc = bc->less_significant;
        }
    }

    return TRUE;
}

BOOL ap_int_geq_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    strip_significant_zeroes_from_ap_int(a);
    strip_significant_zeroes_from_ap_int(b);
    if (a->size > b->size) return TRUE;
    else if (b->size > a->size) return FALSE;

    ap_int_component* ac = a->most_significant;
    ap_int_component* bc = b->most_significant;

    while(ac && bc)
    {
        if (ac->integer > bc->integer) return TRUE;
        else if (ac->integer < bc->integer) return FALSE;
        else
        {
            ac = ac->less_significant;
            bc = bc->less_significant;
        }
    }

    return TRUE;
}

BOOL ap_int_leq_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    strip_significant_zeroes_from_ap_int(a);
    strip_significant_zeroes_from_ap_int(b);
    if (a->size > b->size) return FALSE;
    else if (b->size > a->size) return TRUE;

    ap_int_component* ac = a->most_significant;
    ap_int_component* bc = b->most_significant;

    while(ac && bc)
    {
        if (ac->integer > bc->integer) return FALSE;
        else if (ac->integer < bc->integer) return TRUE;
        else
        {
            ac = ac->less_significant;
            bc = bc->less_significant;
        }
    }

    return TRUE;
}

ap_int* ap_int_minus_ap_int(ap_int* a, ap_int* b) // runtime O(n)
{
    if (ap_int_leq_ap_int(a,b)) return init_ap_int_with_uint32(0); // we don't do negative numbers
    ap_int* c = init_ap_int_with_uint32(0);
    ap_int_bitwise_iterator* ai = init_ap_bitwise_iterator(a,START);
    ap_int_bitwise_iterator* bi = init_ap_bitwise_iterator(b,START);
    ap_int_bitwise_iterator* ci = init_ap_bitwise_iterator(c,START);
    BOOL a_bit;
    BOOL b_bit;
    BOOL c_bit;
    BOOL carry = FALSE;
    while(get_more_significant_bit(ai, &a_bit))
    {
        if (!get_more_significant_bit(bi, &b_bit)) b_bit = FALSE;
        b_bit = !b_bit;

        c_bit = ((a_bit ^ b_bit) ^ carry);
        carry = (a_bit & b_bit) | (b_bit & carry) | (carry & a_bit);
        set_current_bit_move_more_sig(ci, c_bit);
    }
    ap_int_plus_uint32(c,1);
    strip_significant_zeroes_from_ap_int(c);

    free_ap_bitwise_iterator(ai); free_ap_bitwise_iterator(bi); free_ap_bitwise_iterator(ci);
    return c;
}

ap_int* ap_int_minus_uint32(ap_int* a, uint32_t b) // runtime O(n)
{
    ap_int* B = init_ap_int_with_uint32(b);
    ap_int* result = ap_int_minus_ap_int(a, B);
    free_ap_int(B);
    return result;
}

void ap_int_decrement(ap_int* a)
{
    if (ap_int_greater_than_uint32(a, 0))
    {
        ap_int_component* ac = a->least_significant;
        while (ac)
        {
            if (ac->integer > 0)
            {
                ac->integer--;
                break;
            }
            else
            {
                ac->integer--;
                ac = ac->more_significant;
            }
        }
    }
}

void ap_int_increment(ap_int* a)
{
    ap_int_component* ac = a->least_significant;
    while (ac)
    {
        ac->integer++;
        if (ac->integer == 0)
        {
            ac = ac->more_significant;
        }
        else return;
    }
    ap_int_add_component(a,1);
}

void strip_significant_zeroes_from_ap_int(ap_int *a)
{
    ap_int_component* c = a->most_significant;
    while(c->less_significant && !c->integer)
    {
        a->most_significant = c->less_significant;
        a->most_significant->more_significant = NULL;
        a->size--;
        free(c);
        c = a->most_significant;
    }
}

ap_int* copy_ap_int(ap_int* a)
{
    ap_int* c = init_ap_int_with_uint32(a->least_significant->integer);
    ap_int_component* ac = a->least_significant->more_significant;
    while(ac)
    {
        ap_int_add_component(c, ac->integer);
        ac = ac->more_significant;
    }
    return c;
}

void free_ap_int(ap_int* a)
{
    ap_int_component* c = a->least_significant, * n;
    while (c)
    {
        n = c->more_significant;
        free(c);
        c = n;
    }
    free(a);
}

ap_int* ap_int_multiply_ap_int(ap_int* a, ap_int* b) // runtime O(n^2)
{
    // loop over each bit of a, if its zero do nothing, if its a 1, add a shifted copy of b to the aggregator
    ap_int_bitwise_iterator* i = init_ap_bitwise_iterator(a,START);
    ap_int* aggregator = init_ap_int_with_uint32(0), * new_aggregator, * multiplier;
    multiplier = copy_ap_int(b);
    BOOL bit;

    while (get_more_significant_bit(i, &bit))
    {
        if (bit)
        {
            new_aggregator = ap_int_plus_ap_int(aggregator, multiplier);
            free_ap_int(aggregator);
            aggregator = new_aggregator;
        }
        ap_int_shift_left(multiplier);
    }
    free_ap_bitwise_iterator(i);
    free_ap_int(multiplier);
    return aggregator;
}

ap_int* ap_int_multiply_uint32(ap_int* a, uint32_t b) // runtime O(n^2)
{
    ap_int* apb = init_ap_int_with_uint32(b);
    ap_int* result = ap_int_multiply_ap_int(apb,a);
    free_ap_int(apb);
    return result;
}

ap_int_division_result* ap_int_divide_by_two(ap_int* a) // runtime O(n)
{
    ap_int_division_result* result = (ap_int_division_result*)malloc(sizeof(ap_int_division_result));
    if (ap_int_is_odd(a)) result->remainder = init_ap_int_with_uint32(1);
    else result->remainder = init_ap_int_with_uint32(0);

    result->quotient = copy_ap_int(a);
    ap_int_shift_right(result->quotient);
    return result;
}

ap_int_division_result* ap_int_divide_by_ap_int(ap_int* N, ap_int* D) // runtime O(n^2)
{
    // if D is 2, which we can check in constant time, we can save a lot of time
    // by using a dedicated divide by two method.
    if (ap_int_equal_to_uint32(D,2)) return ap_int_divide_by_two(N);
    // if D is 1, then just return (N,0)
    if (ap_int_equal_to_uint32(D,1))
    {
        ap_int_division_result* result = (ap_int_division_result*)malloc(sizeof(ap_int_division_result));
        result->quotient = copy_ap_int(N);
        result->remainder = init_ap_int_with_uint32(0);
        return result;
    }
    // if D is 0, freak the hell out
    if (ap_int_equal_to_uint32(D,0)) return NULL;
    // if N < D, then just return (0,N)
    if (ap_int_less_than_ap_int(N, D)) // O(n)
    {
        ap_int_division_result* result = (ap_int_division_result*)malloc(sizeof(ap_int_division_result));
        result->quotient = init_ap_int_with_uint32(0);
        result->remainder = copy_ap_int(N);
        return result;
    }

    // Otherwise actually execute the division
    ap_int* Q = init_ap_int_with_uint32(0);
    for (uint64_t i = N->size - 1; i; i--) ap_int_add_component(Q,0);
    ap_int* R = init_ap_int_with_uint32(0);
    ap_int* t;
    ap_int_bitwise_iterator* iter_n = init_ap_bitwise_iterator(N, FINISH);
    ap_int_bitwise_iterator* iter_q = init_ap_bitwise_iterator(Q, FINISH);
    uint32_t n = 0;

    while(get_less_significant_bit(iter_n, (BOOL*)&n))
    {
        ap_int_shift_left(R);
        ap_int_plus_uint32(R,n);

        if (ap_int_geq_ap_int(R,D))
        {
            t = ap_int_minus_ap_int(R,D);
            free_ap_int(R);
            R = t;
            set_current_bit_move_less_sig(iter_q, TRUE);
        }
        else
        {
            set_current_bit_move_less_sig(iter_q, FALSE);
        }
    }

    free_ap_bitwise_iterator(iter_n); free_ap_bitwise_iterator(iter_q);

    ap_int_division_result* r = (ap_int_division_result*)malloc(sizeof(ap_int_division_result));

    strip_significant_zeroes_from_ap_int(Q);
    strip_significant_zeroes_from_ap_int(R);

    r->quotient = Q;
    r->remainder = R;

    return r;
}

ap_int_division_result* ap_int_divide_by_uint32(ap_int* dividend, uint32_t divisor)
{
    ap_int* d = init_ap_int_with_uint32(divisor);
    ap_int_division_result* r = ap_int_divide_by_ap_int(dividend, d);
    free_ap_int(d);
    return r;
}

uint64_t most_significant_bit_idx(ap_int* a)
{
    uint64_t idx = (a->size - 1) * 32;
    for (int i = 31; i; i--)
    {
        if (a->most_significant->integer >> i & 1){
            idx += i;
            break;
        }
    }
    return idx;
}

BOOL ap_int_greater_than_uint32(ap_int* a, uint32_t b)
{
    strip_significant_zeroes_from_ap_int(a);
    if (a->size > 1) return TRUE;
    return a->least_significant->integer > b;
}

BOOL ap_int_less_than_uint32(ap_int* a, uint32_t b)
{
    strip_significant_zeroes_from_ap_int(a);
    if (a->size > 1) return FALSE;
    return a->least_significant->integer < b;
}

BOOL ap_int_equal_to_uint32(ap_int* a, uint32_t b)
{
    strip_significant_zeroes_from_ap_int(a);
    if (a->size > 1) return FALSE;
    return a->least_significant->integer == b;
}

BOOL ap_int_geq_uint32(ap_int* a, uint32_t b)
{
    strip_significant_zeroes_from_ap_int(a);
    if (a->size > 1) return TRUE;
    return a->least_significant->integer >= b;
}

BOOL ap_int_leq_uint32(ap_int* a, uint32_t b)
{
    strip_significant_zeroes_from_ap_int(a);
    if (a->size > 1) return FALSE;
    return a->least_significant->integer <= b;
}

BOOL ap_int_is_odd(ap_int* a)
{
    return (BOOL)(a->least_significant->integer % 2);
}

ap_int* ap_int_mod_ap_int(ap_int* a, ap_int* b)
{
    if (ap_int_equal_to_uint32(b, 1))
    {
        return init_ap_int_with_uint32(0);
    }
    ap_int_division_result* r = ap_int_divide_by_ap_int(a,b);
    free_ap_int(r->quotient);
    ap_int* result = r->remainder;
    free(r);
    return result;
}

uint32_t ap_int_mod_uint32(ap_int* a, uint32_t b)
{
    if (b == 1)
    {
        return 0;
    }
    ap_int_division_result* r = ap_int_divide_by_uint32(a,b);
    free_ap_int(r->quotient);
    uint32_t result = r->remainder->least_significant->integer;
    free_ap_int(r->remainder); free(r);
    return result;
}

ap_int* ap_int_modular_exponentiation(ap_int* base, ap_int* exponent, ap_int* modulus)
{
    if (ap_int_equal_to_uint32(modulus, 1))
    {
        return init_ap_int_with_uint32(0);
    }
    ap_int* result = init_ap_int_with_uint32(1);
    ap_int* t, * exp = copy_ap_int(exponent);
    ap_int* b = ap_int_mod_ap_int(base, modulus);

    while(ap_int_greater_than_uint32(exp,0))
    {
        if (ap_int_is_odd(exp))
        {
            t = ap_int_multiply_ap_int(result, b);
            free_ap_int(result);
            result = ap_int_mod_ap_int(t, modulus);
            free_ap_int(t);
        }
        ap_int_shift_right(exp);
        t = ap_int_multiply_ap_int(b, b);
        free_ap_int(b);
        b = ap_int_mod_ap_int(t, modulus);
        free_ap_int(t);
    }

    free_ap_int(exp); free_ap_int(b);

    return result;
}

void ap_int_shift_left(ap_int* a)
{
    ap_int_component* c = a->least_significant;
    uint32_t msb = 0;
    uint32_t prev_msb = 0;
    while(c)
    {
        // get the most significant bit of the current component
        msb = c->integer >> 31;
        // shift the component
        c->integer = c->integer << 1;
        // add the previous most significant bit on the end
        c->integer = c->integer | prev_msb;
        prev_msb = msb;
        c = c->more_significant;
    }
    if (msb == 1) ap_int_add_component(a, prev_msb);
}

void ap_int_shift_left_by(ap_int* a, uint64_t s)
{
    uint64_t component_shift = s / 32;
    uint8_t bitwise_shift = (uint8_t)(s % 32);
    ap_int_component* c;
    for (uint8_t i = 0; i < bitwise_shift; i++)
    {
        ap_int_shift_left(a);
    }
    for (; component_shift; component_shift--)
    {
        // append an all zeroes component to the right size of the ap_int
        c = (ap_int_component*)malloc(sizeof(ap_int_component));
        c->integer = 0;
        c->less_significant = NULL;
        c->more_significant = a->least_significant;
        a->least_significant->less_significant = c;
        a->least_significant = c;
        a->size ++;
    }
}

void ap_int_shift_right(ap_int* a)
{
    ap_int_component *c = a->least_significant;
    while (c)
    {
        uint32_t next_lsb = (c->more_significant) ? c->more_significant->integer & TRUE : FALSE;
        c->integer = (c->integer >> 1) | (next_lsb << 31);
        c = c->more_significant;
    }
}

void ap_int_shift_right_by(ap_int* a, uint64_t s)
{
    uint64_t component_shift = s / 32;
    if (component_shift >= a->size)
    {
        // set a equal to zero
        a->most_significant->integer = 0;
        for (ap_int_component* ac = a->least_significant; ac->more_significant; ac = ac->more_significant)
        {
            ac->integer = 0;
        }
        strip_significant_zeroes_from_ap_int(a);
        return;
    }
    uint8_t bitwise_shift = (uint8_t)(s % 32);
    ap_int_component* c;
    for (uint8_t i = 0; i < bitwise_shift; i++)
    {
        ap_int_shift_right(a);
    }
    for (; component_shift; component_shift--)
    {
        c = a->least_significant->more_significant;
        free(a->least_significant);
        a->least_significant = c;
        a->least_significant->less_significant = NULL;
        a->size--;
    }
    strip_significant_zeroes_from_ap_int(a);
}

/*
 * This function generates a random AP-int given two values: a uint64_t
 * representing the max number of components in the AP in and a uint8_t
 * representing the maximum number of bits in the last component. Effectively,
 * this function creates a random AP int with a maximum number of bits equal to
 * 32 * max_num_components + max_num_bits_in_msc
 *
 * Another way to think of it is that it generates a random number between 0
 * and 2 to the (32 * max_num_components + max_num_bits_in_msc). The reason that
 * the function takes two parameters instead of a number of bits is so as not
 * to limit the possible number of results to less than 2 to the (max uint64_t)
 * but rather to the actual maximum size of an ap_int which is 2 to the (32 *
 * max uint64_t).
 */
ap_int* basic_random_ap_int_of_maxsize(uint64_t max_num_components, uint8_t max_num_bits_in_msc)
{
    ap_int* a = init_ap_int_with_uint32(0);
    ap_int_component* ac = a->least_significant;

    // Pick a random number of components between 0 and the maximum
    uint64_t num_components = random64(0,max_num_components);
    for (; num_components; num_components--)
    {
        ac->integer = random(0,4294967295);
        ap_int_add_component(a,0);
        ac = ac->more_significant;
    }

    // if the random number of components is equal to the max number of
    // components, then we we need to consider the max number of bits in the
    // most significant component, otherwise, we can just pick a random uint8_t
    // for the number of bits in the most significant component.
    if (num_components != max_num_components)
    {
        max_num_bits_in_msc = random8(0, 31);
    }
    ac->integer = random(0,4294967295) >> (31 - max_num_bits_in_msc);

    return a;
}

ap_int* random_ap_int_in_range(ap_int* min, ap_int* max)
{
    ap_int* range = ap_int_minus_ap_int(max,min);
    ap_int* large_rand = basic_random_ap_int_of_maxsize(max->size + 1, 31);
    ap_int_division_result* d = ap_int_divide_by_ap_int(large_rand, range);
    ap_int* rand = ap_int_plus_ap_int(d->remainder, min);
    free_ap_int(d->remainder); free_ap_int(d->quotient); free(d);
    free_ap_int(range); free_ap_int(large_rand);

    return rand;
}

BOOL probably_prime_MR(ap_int* potential_prime, uint64_t iterations)
{
    if (ap_int_equal_to_uint32(potential_prime,2) || ap_int_equal_to_uint32(potential_prime,3))
        return TRUE;
    if (ap_int_equal_to_uint32(potential_prime,0) || ap_int_equal_to_uint32(potential_prime,1) || !ap_int_is_odd(potential_prime))
        return FALSE;

    ap_int_division_result* t_dr;
    ap_int* s = ap_int_minus_uint32(potential_prime,1);
    ap_int* p_1 = ap_int_minus_uint32(potential_prime,1);
    ap_int* tt_api;
    ap_int* t_api;
    ap_int* rand_api;
    while (!ap_int_is_odd(s))
    {
        t_dr = ap_int_divide_by_two(s);
        free_ap_int(s);
        s = t_dr->quotient;
        free_ap_int(t_dr->remainder); free(t_dr);
    }

    ap_int* min = init_ap_int_with_uint32(2);
    ap_int* mod;
    for(uint64_t i = 0; i < iterations; i++)
    {
        rand_api = random_ap_int_in_range(min,p_1);
        t_api = copy_ap_int(s);
        mod = ap_int_modular_exponentiation(rand_api, t_api, potential_prime);
        free_ap_int(rand_api);

        while (!ap_int_equal_to_ap_int(t_api, p_1) &&
                !ap_int_equal_to_uint32(mod, 1) &&
                !ap_int_equal_to_ap_int(mod, p_1))
        {
            tt_api = ap_int_modular_exponentiation(mod,min,potential_prime);
            free_ap_int(mod);
            mod = tt_api;
            ap_int_shift_left(t_api);
        }


        if (!ap_int_equal_to_ap_int(mod, p_1) && !ap_int_is_odd(t_api))
        {
            free_ap_int(mod); free_ap_int(t_api);
            free_ap_int(p_1); free_ap_int(min); free_ap_int(s);
            return COMPOSITE;
        }
        free_ap_int(mod); free_ap_int(t_api);
    }
    free_ap_int(p_1); free_ap_int(min); free_ap_int(s);
    return PROBABLY_PRIME;
}

ap_int_prime_factorization* random_factored_number(ap_int* max, uint64_t prime_certainty_index)
{
    ap_int** prime_factor_candidates = (ap_int**)malloc(sizeof(ap_int*) * NUM_FACTORIZATION_CANDIDATES);
    int_arraylist* primes = init_arraylist();
    ap_int* one = init_ap_int_with_uint32(1);

    ap_int_prime_factorization* result = (ap_int_prime_factorization*)malloc(sizeof(ap_int_prime_factorization));
    while (primes->size == 0)
    {
        // Generate the candidate factors, and pick out the primes.
        for (uint32_t i = 0; i < NUM_FACTORIZATION_CANDIDATES; i++)
        {
            prime_factor_candidates[i] = random_ap_int_in_range(one, max);
            if (probably_prime_MR(prime_factor_candidates[i], prime_certainty_index))
            {
                arraylist_append(primes, i);
            }
        }
        if (!primes->size)
        {
            for (uint32_t i = 0; i < NUM_FACTORIZATION_CANDIDATES; i++)
                free_ap_int(prime_factor_candidates[i]);
            free_arraylist(primes);
            primes = init_arraylist();
        }
    }

    result->num_factors = primes->size;
    result->prime_factors = (ap_int**)malloc(sizeof(ap_int*) * result->num_factors);
    uint32_t j = 0;

    for (uint32_t i = 0; i < NUM_FACTORIZATION_CANDIDATES; i++)
    {
        if (i == arraylist_get(primes,j))
        {
            assert(primes->size > 0);
            result->prime_factors[j++] = prime_factor_candidates[i];
        }
        else
        {
            free_ap_int(prime_factor_candidates[i]);
        }
    }
    assert(j == primes->size);
    assert(j == result->num_factors);
    free(prime_factor_candidates);
    free_arraylist(primes);

    result->number = init_ap_int_with_uint32(1);
    ap_int* t_api;

    for (uint32_t i = 0; i < result->num_factors; i++)
    {
        t_api = result->number;
        result->number = ap_int_multiply_ap_int(t_api, result->prime_factors[i]);
        free_ap_int(t_api);
    }

    if (ap_int_leq_ap_int(result->number, max) &&
            ap_int_geq_ap_int(result->number, one) &&
            result->num_factors > 1)
    {
        free_ap_int(one);
        return result;
    }
    else // free the results and call the method again
    {
        free_ap_int(one);
        free_ap_int(result->number);
        for (uint32_t i = 0; i < result->num_factors; i++)
        {
            free_ap_int(result->prime_factors[i]);
        }
        free(result->prime_factors);
        free(result);

        return random_factored_number(max, prime_certainty_index);
    }
}

ap_int_prime_factorization* random_prime_factorization(ap_int* max, uint64_t prime_certainty_index)
{
    while (42)
    {
        ap_int_prime_factorization* m = random_factored_number(max, prime_certainty_index);
        ap_int_plus_uint32(m->number, 1);
        if (probably_prime_MR(m->number, prime_certainty_index))
        {
            return m;
        }
        else
        {
            free_ap_int(m->number);
            for (uint32_t i = 0; i < m->num_factors; i++)
                free_ap_int(m->prime_factors[i]);
            free(m->prime_factors);
            free(m);
        }
    }
}

ap_int* generator_for_prime_ap_int(ap_int_prime_factorization* f)
{
    assert(probably_prime_MR(f->number,100));
    ap_int* two = init_ap_int_with_uint32(2);
    ap_int* g = random_ap_int_in_range(two,f->number);
    free_ap_int(two);

    return g;
}

BOOL lsb_of_msc(ap_int* a)
{
    return (BOOL)(a->most_significant->integer & 1);
}

ap_int* prg(ap_int* N, uint64_t num_bits_to_generate)
{
    ap_int* zero = init_ap_int_with_uint32(0);
    ap_int* s = basic_random_ap_int_of_maxsize(N->size * 1000,0); // generates a large random seed that's less than 1000 times the size of the prime range
    ap_int* temp;
    ap_int_prime_factorization* f = random_prime_factorization(N,10);
    ap_int* p = f->number;
    ap_int* g = generator_for_prime_ap_int(f);

    ap_int* result = init_ap_int_with_uint32(0);
    ap_int_bitwise_iterator* iter = init_ap_bitwise_iterator(result, START);
    set_current_bit_move_more_sig(iter, lsb_of_msc(s));

    for (uint64_t i = 0; i < num_bits_to_generate - 1; i++)
    {
        temp = ap_int_modular_exponentiation(g,s,p);
        free_ap_int(s);
        s = temp;
        set_current_bit_move_more_sig(iter, lsb_of_msc(s));
    }

    free_ap_bitwise_iterator(iter);
    free_ap_int(f->number);
    for (uint32_t i = 0; i < f->num_factors; i++)
        free_ap_int(f->prime_factors[i]);
    free(f->prime_factors);
    free(f);
    free_ap_int(s); free_ap_int(g); free_ap_int(zero);

    return result;
}

const char* ap_int_to_debug_str(ap_int* a)
{
    ap_int_component* c = a->most_significant;
    char * str = (char*)calloc(sizeof(char),a->size * 38);
    const char * data;
    while (c)
    {
        data = uint32_to_binary_str(c->integer);
        strcat(str,data);
        strcat(str," <-> ");
        free((void*)data);
        c = c->less_significant;
    }
    return str;
}

const char* ap_int_to_binary_str(ap_int* a)
{
    ap_int_component* c = a->most_significant;
    char * str = (char*)calloc(sizeof(char),a->size * 32 + 1);
    const char * data;
    while (c)
    {
        data = uint32_to_binary_str(c->integer);
        strcat(str,data);
        free((void*)data);
        c = c->less_significant;
    }
    return str;
}

const char* ap_int_to_decimal_str(ap_int* a, uint64_t size)
{
    uint32_t i = 0;
    char* str = (char*)calloc(sizeof(char),size + 13); //...truncated\0
    uint32_t rem;
    ap_int* num = a;
    ap_int_division_result* d;

    if (a->size == 1 && a->most_significant->integer == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    while (!ap_int_equal_to_uint32(num,0))
    {
        rem = ap_int_mod_uint32(num, 10);
        str[i++] = (rem > 9)? (char)((rem-10) + 'a'): (char)(rem + '0');
        d = ap_int_divide_by_uint32(num,10);
        if (num != a) free_ap_int(num);
        num = d->quotient;
        free_ap_int(d->remainder); free(d);
        if (i > size)
        {
            str[--i] = '\0';
            strcat(str,"...detacnurt"); //truncated...
            break;
        }
    }
    free_ap_int(num);
    reverse(str);
    return str;
}

void print_ap_int_bin(ap_int* a)
{
    const char* bin_str = ap_int_to_binary_str(a);
    printf("%s\n", bin_str);
    free((void*)bin_str);
}

void print_ap_int(ap_int* a)
{
    const char* str = ap_int_to_decimal_str(a, 100);
    printf("%s\n", str);
    free((void*) str);
}