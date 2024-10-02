/**
 * @file   main.c
 * @author Rick Wertenbroek
 * @date   14.12.21
 *
 * @brief  Testing fibonacci sequences
 */

#ifndef __QEMU_BARE__
#   define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
#   include <common.h>
#   include <exports.h>
#else
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

// https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/// @brief a nice red KO with a space
#define __KO "[" ANSI_COLOR_RED "KO" ANSI_COLOR_RESET "] "
/// @brief a nice green OK with a space
#define __OK "[" ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "] "

#ifndef FUN_TO_CHECK
#   define FUN_TO_CHECK fib
#endif

#define LOOP_CHECK(FUN) \
    do { \
        for (uint32_t i = 0; i < 10; ++i) CHECK_FIB(i, FUN); \
        for (uint32_t i = 0; i < 7; ++i) CHECK_FIB(i*5+10, FUN); \
    } while(0)

#define CHECK_FIB(N, FUN) \
    do { \
        uint32_t result = fib_c_reference(N); \
        uint32_t result_asm = FUN (N); \
        if (result != result_asm) { \
            printf(__KO "result of fib(%u) should be %u not %u\n", N, result, result_asm); \
        } else { \
            printf(__OK "result of fib(%u) is %u\n", N, result); \
        } \
    } while(0)

extern uint32_t silly_fib_asm(uint32_t n);
extern uint32_t fib_asm(uint32_t n, uint32_t a, uint32_t b);
uint32_t fib(uint32_t n) { return fib_asm(n, 0, 1); }
extern uint32_t fib_asm_tail(uint32_t n, uint32_t a, uint32_t b);
uint32_t fib_tail(uint32_t n) { return fib_asm_tail(n, 0, 1); }

uint32_t fib_c_reference(uint32_t n) {
    // Loop version
    uint32_t a = 0;
    uint32_t b = 1;

    if (n == 0) {
        return 0;
    }

    while(--n) {
        uint32_t sum = a+b;
        a = b;
        b = sum;
    }

    return b;
}

uint32_t fib_recursive(uint32_t n) {
    // Bad idea ! exponential recursion
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib_recursive(n-1) + fib_recursive(n-2);
}

int main(int argc, char *argv[]) {

    LOOP_CHECK(FUN_TO_CHECK);
    printf("\nDone !\n");

    return 0;

    for (uint32_t i = 0; i < 10; ++i) {
        CHECK_FIB(i, fib_c_reference);
    }
    for (uint32_t i = 0; i < 7; ++i) {
        uint32_t n = 5 * i + 10;
        //CHECK_FIB(n, fib_c_reference);
        CHECK_FIB(n, fib_recursive);
    }

    printf("\nDone !\n");

    return 0;
}
