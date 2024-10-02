@ ASM
@ Author :  Rafael Dousse

@@ @brief void show_backtrace() prints the backtrace up to main()
.global show_backtrace
show_backtrace:
        stmfd sp!, {fp, lr}
        add fp, sp, #4

        @@@@@@@@@@@ A completer @@@@@@@@@@

        ldr r0, =backtrace_string
        bl printf

        @ TODO

        mov r2, fp	        @ Get the current frame pointer

        loop:

                ldr r1, [r2]		@ Read the adress in the lr
                cmp r1, #-1		@ Check if it is the end
                beq end			@ Jump to the end if it is
                stmfd sp!, {r2}		@ Save r2 on the stack
                ldr r0, =format_string	@ Load the format string
                bl printf		@ Call printf
                ldmfd sp!, {r2}	        @ Restore r2
                ldr r2, [r2, #-4]	@ Get the previous frame pointer
                b loop			
        end:
	@@@@@@@@@@@ ----------- @@@@@@@@@@
        sub sp, fp, #4
        ldmfd sp!, {fp, pc}

.data

backtrace_string:
        .string "\nBacktrace :\n----\n"

format_string:
        .string "Previous link register : 0x%p\n"