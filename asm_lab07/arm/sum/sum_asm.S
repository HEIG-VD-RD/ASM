@     ___   _____ __  ______   ____ ___  ___ 
@    /   | / ___//  |/  /__ \ / __ \__ \|__ \
@   / /| | \__ \/ /|_/ /__/ // / / /_/ /__/ /
@  / ___ |___/ / /  / // __// /_/ / __// __/ 
@ /_/  |_/____/_/  /_//____/\____/____/____/ 
@
@ Author : Rafael Dousse

@@ @brief uint32_t sum_asm(uint32_t n, ...)
@@ Computes the sum of ... (uint32_t), 0 if empty
@@ @param n (r0) the number of elements in ...
@@ @param ... variable number of uint32_t elements
@@ @return sum of ... or 0 if empty
.global sum_asm
sum_asm:
        @@@@@@@@@@@ A completer @@@@@@@@@@
        @ L'espace vide vous donne une idée de la taille de la solution
        @prologue

        stmfd   sp!, {r1-r3}    
        stmfd   sp!, {fp, lr}   @ save frame pointer and link register
        add     fp, sp, #4      @ set frame pointer             
        add     sp, sp, #8      @ move stack pointer to the arguments
        
        mov  r1, r0            @ r1 = n
        mov r2, #0             @ r2 = sum
        mov r3, #0             @ r3 = arguments

        loop:
           cmp r1, #0          @ if n == 0
           beq end             @ keep looping if not eq else go to end
           ldmfd sp!, {r3}     @ pop the argument, store it in r3 and increment the stack pointer
           add r2, r2, r3      @ sum += r3
           sub r1, r1, #1      @ n--    
           b loop              @ loop

        end:
           mov r0, r2          @ return sum

           @epilogue
           sub sp, fp, #4      @ restore stack pointer
           ldmfd sp!, {fp, lr} @ restore frame pointer and link register                
           mov pc, lr          @ return

        @@@@@@@@@@@ ----------- @@@@@@@@@@
