//
// Created by luka on 5.4.19..
//

#include <stdlib.h>
#include "math.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"

int random_int(int min, int max) {
    return rand() % max + min;
}

unsigned long long random_ull(unsigned long long min, unsigned long long max) {
    return (((unsigned long long)rand() << 32) + (unsigned long long)rand()) % (max - min) + min;
}
