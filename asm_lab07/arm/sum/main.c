/**
 * @file   main.c
 * @author Rick Wertenbroek
 * @date   14.12.21
 *
 * @brief  Testing variadic sum function
 */

#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
#   include <common.h>
#   include <exports.h>
void memcpy_custom(char *dest, const char *src, unsigned int num) {
    while(num--) {
        *dest++ = *src++;
    }
}
uint32_t strlen(const char* s) {
    uint32_t result = 0;
    while(*(s+result)) {
        result++;
    }
    return result;
}
#   define memcpy memcpy_custom
#else
#   warning "Compiling for host"
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif
#include <stdarg.h>

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

// This line is from : https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
#define NUMARGS(...)  (sizeof((int[]){0, ##__VA_ARGS__})/sizeof(int)-1)

/// @brief checks the students variadic sum function against the C reference
#define CHECK_SUM(...) \
    do { \
        uint32_t result, result_asm; \
        result = sum(NUMARGS(__VA_ARGS__), ##__VA_ARGS__); \
        result_asm = sum_asm(NUMARGS(__VA_ARGS__), ##__VA_ARGS__); \
        const char *fmt = generate_format_string("Sum of : ", NUMARGS(__VA_ARGS__)); \
        if (result != result_asm) { \
            printf(__KO); \
            printf(fmt, ##__VA_ARGS__); \
            printf("should equal %u and not %u !\n", result, result_asm); \
        } else { \
            printf(__OK); \
            printf(fmt, ##__VA_ARGS__); \
            printf("is equal to %u, well done !\n", result); \
        } \
        free((void*)fmt); \
    } while(0) \

extern uint32_t sum_asm(uint32_t n, ...);

/// @brief variadic sum
uint32_t sum(uint32_t n, ...) {

    uint32_t sum = 0;

    va_list p;

    va_start(p, n);

    while(n--) {
        sum += va_arg(p, uint32_t);
    }

    va_end(p);

    return sum;
}

/// @brief generates a format string with prefix and n uint32_t elements
const char *generate_format_string(const char *PREFIX, uint32_t n) {
    const char* NUMBER = "%u ";
    char* result = malloc(strlen(PREFIX) + strlen(NUMBER)*n + 1);
    char* p = result;
    memcpy(p, PREFIX, strlen(PREFIX));
    p += strlen(PREFIX);
    while(n--) {
        memcpy(p, NUMBER, strlen(NUMBER));
        p += strlen(NUMBER);
    }
    *p = '\0';
    
    return result;
}

/// @brief checks the students variadic sum function against the C reference
int main(int argc, char *argv[]) {

    CHECK_SUM();
    CHECK_SUM(42);
    CHECK_SUM(555, 111);
    CHECK_SUM(1, 2, 3);
    CHECK_SUM(1, 2, 3, 4);
    CHECK_SUM(1, 2, 3, 4, 5);
    CHECK_SUM(1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1);

    return 0;
}
