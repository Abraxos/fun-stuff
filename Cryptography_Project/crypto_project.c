//
// Created by eugene on 11/7/15.
//

#include <stdio.h>
#include "ap_int.h"

int main();

int main() {
    printf("Introduction to Modern Cryptography: Graduate Project - by Eugene Kovalev\n");
    // Please feel free to insert any code you wish to run using the ap_int
    // crypto library here. The testing code is a good place to look for
    // examples of how to use the library properly.

    // Example:
    ap_int* a = init_ap_int_with_decimal_string("398475032987450239");
    print_ap_int(a);
    free_ap_int(a);
}