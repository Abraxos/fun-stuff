# Introduction to Modern Cryptography - Graduate Project

_by Eugene Kovalev_

NOTE: This readme was written in markdown format. It can be read in a normal text editor, however, to view it in the most reader-friendly way please use a markdown reader application. If no such application is installed on your machine, please use the online markdown reader provided here: http://dillinger.io/ (simply paste this readme into the left panel of the site, and it will be rendered as markdown on the right).

NOTE: This program, as well as all dependencies, was written, installed, and tested on Debian Jessie. Consequently this readme was written assuming the same operating system is used. I cannot guarantee the same instructions will work the same way on other systems.

This submission comes with an optional VM image which can be downloaded here as a bzip of a qcow2 img file: [https://goo.gl/Sm7N5P](https://goo.gl/Sm7N5P) It was created by the program QEMU/Gnome Boxes which should be easy to install on a Linux system: `sudo apt-get install gnome-boxes`. The image can also easily be converted to any other format: [https://en.wikibooks.org/wiki/QEMU/Images](https://en.wikibooks.org/wiki/QEMU/Images). The VM is already set up to run the project. When booted it will automatically log in as the user `cryptographer` (if it does not, the password is identical to the enrollment key for the course that was emailed out before the start of the semester). The project is located in the `Crypto_Project` directory on the desktop and the final build products are available in the `Crypto_Project/build` directory.

## Project Description

The project has several files and folders and are listed below along with their descriptions:

1. `build` - This is the directory inside of which the executables will be produced. This is described in the **Building** section.

2. `cmake` - This folder contains several module files for the CMake build system. You should not have to look into or modify anything inside of this folder.

3. `ap_int.c` - This is the core file inside of which all the required functionality of this library is implemented.

4. `ap_int.h` - This is the header file that describes the functionality of everything inside `ap_int.c`.

5. `ap_int_tests.c` - This is the most important file of the project. It contains unit tests that confirm the functionality of the library as well as examples of how to use it. To confirm for yourself that the project functions as required, you should compile and execute the testing executable that is based on this file. This is described in detail in the **Testing** section. Anything inside this file will be compiled into the `Test_Crypto_Project` executable.

6. `CMakeLists.txt` - This is the cmake build system configuration file. It should only be edited if you wish to compile with debugging symbols (described in the **Testing Memory Consumption and Leaks** section).

7. `crypto_project.c` - This is the "main" executable of the project. It doesn't really have anything in it. The intention is to allow the grader to put whatever code they want in there to verify the functionality of the program without having to run the unit tests. Please note that for best results you should really run the unit tests and comment out the ones you don't need. Anything in this file will be compiled into the `Crypto_Project` executable.

8. `utils.c` - Basic utility functions.

9. `utils.h` - Header for the above file.

## Building

### Requirements:

1. cmake: `$ sudo apt-get install cmake`. Please note that this project requires cmake version 3.0 or above which should come with the current Debian and Ubuntu package managers.
3. cmocka - required for testing, but not required for running. Installation procedure described below in the testing section.

### Running The Main Executable

The main executable for this project is called Crypto_Project. It should execute any code placed in the `crypto_project.c` file. Unlike the testing executable, it does not actually have any useful code as of right now. The intention is to allow the grader to put any code they want in there for verification. The `ap_int_tests.c` file should provide ample examples on how to use the library for whatever is required.

To compile and run the main executable (especially if the cmocka unit testing framework is not installed), please follow these instructions: cd into the build directory, and direct CMake to compile the Crypto_Project executable only like so:

```
$ cd build
$ cmake ..
$ cmake --build . --target Crypto_Project --
```
To execute the Crypto_Project binary, simply run: `./Crypto_Project` which by default should print out a header and a single ap-int:
```
$ ./Crypto_Project
Introduction to Modern Cryptography: Graduate Project - by Eugene Kovalev
398475032987450239
```
Please note that after you have compiled the executable the first time, all subsequent builds of the Crypto_Project executable can be done with just the last command `$ cmake --build . --target Crypto_Project --` as building the CMake Cache should no longer be necessary.

## Running Unit Tests & Grading

In general, the file ap_int_tests.c contains a comprehensive list of every single test case that was run on my software. There are a lot of testing functions in there some of which are only indirectly related to the assignment (i.e. the test some sort of functionality that is required to run the test of the assigned problem).

You are encouraged to look through the whole file to see for yourself that the code was tested extensively and that all of it works. The **Testing** section below has a set of precise instructions that demonstrate how to run the set of tests on your machine. If there are any issues compiling and running the code, please feel free to contact me at eugene.kovalev@uconn.edu and I will attempt to resolve them in a way that does not alter the functionality of what I submitted.

## Testing

In order to run the tests, cmocka must be installed, which can be aquired here: https://cmocka.org/

#### CMocka Installation:

1. Download cmocka here: https://cmocka.org/files/1.0/cmocka-1.0.1.tar.xz
2. Extract cmocka:
```
$ tar -xpf cmocka-1.0.1.tar.xz
$ cd cmocka-1.0.1
```
3. Create a build directory: `$ mkdir build`
4. Go into the build directory and build cmocka:
```
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/cc -DCMAKE_CXX_COMPILER=/usr/bin/c++ ..
$ make
$ sudo make install
```
5. The library should now be built inside the directory as well we being placed inside one of the directories in your path. You can confirm that cmocka has been installed by running `$ which cmocka`.

#### Running the Tests

Arguably the most critical component of this project is running the testing executable. In order to do this, the cmocka library needs to be installed as well as the other packages listed above. Assuming those are installed and functioning properly, you can `cd` into the root directory of the project and then follow these instructions. Go into the build directory, compile, and run the Test_Crypto_Project exectuable like so:
```
$ cd build
$ cmake ..
$ make
```
Once this is done, assuming everything ran correctly, the contents of the build directory should look like this:
```
$ ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Crypto_Project  Makefile  Test_Crypto_Project
```

From now on, if you wish to recompile the executable, simply execute: `$ cmake .. && make`

To run all of the tests, simply execute the testing executable, like so:
```
./Test_Crypto_Project
```

You are of course free to comment out certain tests to verify specific components of the project. A correct execution of the `Test_Crypto_Project` executable should produce the following output:
```
[==========] Running 28 test(s).
[ RUN      ] test_addition_overflow_detection
[       OK ] test_addition_overflow_detection
[ RUN      ] test_multiplication_overflow_detection
[       OK ] test_multiplication_overflow_detection
[ RUN      ] test_uint32_to_binary_str
[       OK ] test_uint32_to_binary_str
[ RUN      ] test_debug_printing
[       OK ] test_debug_printing
[ RUN      ] test_ap_int_addition
[       OK ] test_ap_int_addition
[ RUN      ] test_ap_int_subtraction
[       OK ] test_ap_int_subtraction
[ RUN      ] test_ap_int_shift_left
[       OK ] test_ap_int_shift_left
[ RUN      ] test_ap_int_shift_right
[       OK ] test_ap_int_shift_right
[ RUN      ] test_ap_int_shift_left_by
[       OK ] test_ap_int_shift_left_by
[ RUN      ] test_ap_int_multiplication
[       OK ] test_ap_int_multiplication
[ RUN      ] test_ap_int_from_string
[       OK ] test_ap_int_from_string
[ RUN      ] test_ap_int_comparison
[       OK ] test_ap_int_comparison
[ RUN      ] test_ap_int_bitwise_construction
[       OK ] test_ap_int_bitwise_construction
[ RUN      ] test_ap_int_most_significant_bit_index
[       OK ] test_ap_int_most_significant_bit_index
[ RUN      ] test_ap_int_shift_right_by
[       OK ] test_ap_int_shift_right_by
[ RUN      ] test_ap_int_division
[       OK ] test_ap_int_division
[ RUN      ] test_ap_int_to_decimal_string
[       OK ] test_ap_int_to_decimal_string
[ RUN      ] test_ap_int_modulus
[       OK ] test_ap_int_modulus
[ RUN      ] test_ap_int_is_odd
[       OK ] test_ap_int_is_odd
[ RUN      ] test_ap_int_modular_exponentiation
[       OK ] test_ap_int_modular_exponentiation
[ RUN      ] test_division_by_two
[       OK ] test_division_by_two
[ RUN      ] test_random_int
[       OK ] test_random_int
[ RUN      ] test_random_ap_int
[       OK ] test_random_ap_int
[ RUN      ] test_ap_int_incrementing_decrementing
[       OK ] test_ap_int_incrementing_decrementing
[ RUN      ] test_MR_primality_of_ap_int
[       OK ] test_MR_primality_of_ap_int
[ RUN      ] test_generating_random_factored_ap_int
[       OK ] test_generating_random_factored_ap_int
[ RUN      ] test_generating_random_prime_factorization
[       OK ] test_generating_random_prime_factorization
[ RUN      ] test_prg
PSEUDORANDOM: 00000000000011000001110110000100
[  PASSED  ] 28 test(s).
[       OK ] test_prg
[==========] 28 test(s) run.

Process finished with exit code 0
```

#### Time Consuming Tests

Several of the tests that are in the Test_Crypto_Project executable are extremely time consuming. Running all of them may take longer than 15 minutes as they attempt to check random functions and etc. Such tests are labeled with the comment:
```
// Very time consuming, disable unless necessary
```
At submission times, these tests will not be commented out so that when the executable is run, the grader sees that all of the required functions have been written and work properly, however, when testing individual functions those time consuming tests should be disabled by commenting out the entire line which executes the test. This will make testing of individual functions go considerably faster. Other tests not labeled with the comment shown above do not have to be disabled as they can be tested near-instantaneously.

#### Testing Memory Consumption and Leaks

These tests should show that all the functionality of the library is memory safe and leak free. To test this, valgrind must be installed:

1. Install valgrind:
```
$ sudo apt-get install valgrind
```
2. Then from the Cryto-Project directory simply execute:
```
$ cmake .. && make clean && make && valgrind --leak-check=full ./Test_Crypto_Project
```

3. To get a more accurate output in terms of memory leaks (there shouldn't be any, but if there are) you would need to compile with debug information turned on, which can be done by uncommenting line 9 in the file CMakeLists.txt:
```
#set(CMAKE_BUILD_TYPE Debug)
```
to be:
```
set(CMAKE_BUILD_TYPE Debug)
```
and then again executing:
```
$ cmake .. && make clean && make && valgrind --leak-check=full ./Test_Crypto_Project
```

### Tests as Proof of Correctness

Below is an itemized list of the assigned tasks and their corresponding tests (Please note that the bare minimum requirements are shown in the writeup, if you look at the actual file you will notice that the tests are a *lot* more extensive than what is shown here):

1. **Provide a routine to convert a regular machine-precision number into your new format. Provide a routine to print out a number stored in your format:** This requirement is met and demonstrated in the following tests:
```
test_ap_int_from_string
```
Which clearly demonstrates that we can convert a regular number in decimal format to an arbitrary precision integer: `ap_int* a = init_ap_int_with_decimal_string("987230987450129387412341098");` and then demonstrates that it can be printed out as a binary string: `const char* bin_str = ap_int_to_binary_str(a);`.
```
test_ap_int_to_decimal_string
```
Similarly, this test shows that we can read a decimal string and output a decimal string: `a = init_ap_int_with_decimal_string("987230987450129387412341098"); str = ap_int_to_decimal_str(a,27);`.
```
test_ap_int_multiplication
```
Finally, this test, besides demonstrating multiplication, also demonstrates that we can print the ap_int as a binary string with the line: `const char* bin_str = ap_int_to_binary_str(f);`

2. **Provide a routine to add and multiply numbers in your format. Your addition routine should take linear time in the length of the numbers; your multiplication algorithm should take time no worse than quadratic in the length of the numbers:** This requirement is met and demonstrated in the following tests:
```
test_ap_int_addition
```
Which shows us an example of addition using the framework:
```
a = init_ap_int_with_decimal_string("987230987450129387412341098");
b = init_ap_int_with_decimal_string("987230987450129387412341098");
c = ap_int_plus_ap_int(a,b);
debug_str = ap_int_to_binary_str(c);
assert_string_equal(debug_str, "000001100110000100111100100101100010001001000011100110011100011000111001111100101000001011010100");
free((void*)debug_str);
free_ap_int(a); free_ap_int(b); free_ap_int(c);
```
A thorough inspection of the addition method should confirm that it has a linear runtime:
```
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
```
```
test_ap_int_multiplication
```
Which shows us multiplication under similar circumstances:
```
ap_int* a = init_ap_int_with_uint32(4294967295);
ap_int* b = init_ap_int_with_uint32(91283749);
ap_int* c = init_ap_int_with_uint32(2355432189);
ap_int* d = ap_int_multiply_ap_int(a,a);
ap_int* e = ap_int_multiply_ap_int(d,b);
ap_int* f = ap_int_multiply_ap_int(e,c);
const char* bin_str = ap_int_to_binary_str(f);
assert_string_equal(bin_str, "00000010111110111110000011100100110100101111101100110001110001110101000100010101111110011100001011011000111100101111001110010001");
```
A thorough inspection of the multiplication method should confirm that it runs in quadratic time:
```
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
```

3. **Provide a routine to compute, given two arbitrary precision numbers (ap-numbers, for short) the result of division of one by the other. The result should be returned to you as a quotient and a (non-negative) remainder. This should take no more than quadratic time in the length of the numbers:** This requirement is met and demonstrated in the following tests:
```
test_ap_int_division
```
This test shows us the results of using division in this ap-int library:
```
a = init_ap_int_with_decimal_string("987230987450129387412341098");
b = init_ap_int_with_decimal_string("56298734589302");
r = ap_int_divide_by_ap_int(a,b);
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
```
A thorough inspection should confirm that the runtime of the division algorithm is no more than quadratic:
```
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
```
4. **Provide a routine to compute a^b \bmod c for three ap-numbers a, b, and c. This should take polynomial time in the lengths of the numbers (so a cubic time algorithm is fine):**
```
test_ap_int_modular_exponentiation
```
```
a = init_ap_int_with_decimal_string("982984509432852348751342134235412341234232");
b = init_ap_int_with_decimal_string("98324750923847502");
c = init_ap_int_with_decimal_string("3211324234234324924102489098321");
d = ap_int_modular_exponentiation(a, b, c);
str = ap_int_to_decimal_str(d,31);
assert_string_equal(str,"2146333580357975467844001322588");
```
```
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
```

5. **Provide an implementation of the Miller-Rabin randomized primality test. This takes an ap-number as input and determines if it is prime; ensure that your algorithm errs with probability no more than 1/2^{10}. (The algorithm is described in detail in your textbook.):**
```
test_MR_primality_of_ap_int
```
```
a = init_ap_int_with_decimal_string("29996224275833");
assert_true(probably_prime_MR(a, 5));
```
```
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
```

6. **Provide an implementation of A. Kalai's algorithm for generating a random factored integer. The algorithm is described in the (one-page) paper posted below. This algorithm, given an ap-number N, returns a random number R in the range 1\ldots N along with a factorization of the number in to primes. (You will need to use your primality algorithm from above.):**
```
test_generating_random_factored_ap_int
```
```
ap_int* min = init_ap_int_with_uint32(0);
ap_int* max = init_ap_int_with_decimal_string("1342134235412341234232");
ap_int_prime_factorization* f = random_factored_number(max, 10);
assert_true(ap_int_leq_ap_int(f->number, max) && ap_int_geq_ap_int(f->number, min));
free_ap_int(min); free_ap_int(max);
for (uint32_t i = 0; i < f->num_factors; i++)
{
    assert_true(probably_prime_MR(f->prime_factors[i], 100));
    free_ap_int(f->prime_factors[i]);
}
free(f->prime_factors);
free_ap_int(f->number);
free(f);
```
```
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
```

7. **Provide an algorithm that generates a random prime p (uniformly in a given range 1...N) along with the factorization of p-1. To do this, generate m at random (with its factorization) and then test to see if m+1 is prime. If so, you've found the pair that you need:**
```
test_generating_random_prime_factorization
```
```
ap_int* max = init_ap_int_with_decimal_string("1342134235412341234232");
ap_int_prime_factorization* f;
for (uint32_t i = 0; i < 5; i++)
{
    f = random_prime_factorization(max, 10);
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
```
```
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
```

8. **Using the algorithm above, give an algorithm that can produce a random prime p (in a given range 1...N) and then generate a generator for the multiplicative group \mathbb{Z}/p^\*. To do this, you will need to know the factorization of p-1. Then, repeat the following until it succeeds: guess a random number g less than p. Check that g^\alpha \neq 1 \pmod{p} for all numbers \alpha that can be written (p-1)/q where q is a prime dividing p-1. If g passes this test, it is a generator:** - This assignment does not have a method for testing it directly, but is instead tested as part of the algorithm for the following exercise.

9. **Finally, implement the following (proposed) pseudorandom generator. Select a prime p at random as above (in a large interval 1...N), so that you also have the factorization of the number p-1. Select a generator g for the multiplicative group at random. Select a random seed s of the multiplicative group. Consider the sequence s_0 = s, s_1 = g^{s_0} \pmod{p}, ..., s_{i+1} = g^{s_i} \pmod{p},.... Define b_i = \operatorname{MSB}(p_i), where \operatorname{MSB} denotes the "most significant bit" which is defined to 1 in this setting if the argument is more than p/2 and zero otherwise. Your algorithm should produce any desired number of such output bits (the b_i):**
```
test_prg
```
```
ap_int* a = init_ap_int_with_decimal_string("1342134235412341234232");
ap_int* pseudorandom = prg(a,20);
const char* str = ap_int_to_binary_str(pseudorandom);
printf("PSEUDORANDOM: %s\n", str);
assert_true(strlen(str) == 32);
free((void*)str); free_ap_int(pseudorandom);
```
```
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
```
