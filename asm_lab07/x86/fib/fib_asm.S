#     ___   _____ __  ______   ____ ___  ___ 
#    /   | / ___//  |/  /__ \ / __ \__ \|__ \
#   / /| | \__ \/ /|_/ /__/ // / / /_/ /__/ /
#  / ___ |___/ / /  / // __// /_/ / __// __/ 
# /_/  |_/____/_/  /_//____/\____/____/____/ 
#
# Author : Rafael Dousse

.text

## @brief uint32_t fib_asm(uint32_t n)
## Computes the nth Fibonacci number recursively
## Fib(0) = 0, Fib(1) = 1, Fib(n) = Fib(n-1) + Fib(n-2) for n > 1
## @param n the nth number we want as a result
## @return the nth Fibonacci number 0, 1, 1, 2, 3, 5, 8 etc...
.global silly_fib_asm
.type fib_asm, @function
silly_fib_asm:
#   @@@@@@@@@@@ A completer @@@@@@@@@@

    @ Pas fait par manque de temps, a cause de tous les labos a rendre en meme temps-


#   @@@@@@@@@@@ ----------- @@@@@@@@@@

## @brief uint32_t fib_asm(uint32_t n, uint32_t a = 0, uint32_t b = 1)
## Computes the nth Fibonacci number recursively
## Fib(0) = 0, Fib(1) = 1, Fib(n) = Fib(n-1) + Fib(n-2) for n > 1
## @param n the nth number we want as a result
## @param a,b used for recursion
## @return the nth Fibonacci number 0, 1, 1, 2, 3, 5, 8 etc...
.global fib_asm
.type fib_asm, @function
fib_asm:
#   @@@@@@@@@@@ A completer @@@@@@@@@@




#   @@@@@@@@@@@ ----------- @@@@@@@@@@

## @brief uint32_t fib_asm(uint32_t n, uint32_t a = 0, uint32_t b = 1)
## Computes the nth Fibonacci number recursively
## Fib(0) = 0, Fib(1) = 1, Fib(n) = Fib(n-1) + Fib(n-2) for n > 1
## @param n the nth number we want as a result
## @param a,b used for recursion
## @return the nth Fibonacci number 0, 1, 1, 2, 3, 5, 8 etc...
.global fib_asm_tail
.type fib_asm_tail, @function
fib_asm_tail:
#   @@@@@@@@@@@ A completer @@@@@@@@@@




#   @@@@@@@@@@@ ----------- @@@@@@@@@@
