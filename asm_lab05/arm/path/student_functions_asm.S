@ ASM
@ Author : Rafael Dousse


@ X_r : the register for the X position
@ Y_r : the register for the Y position
@ Return_r : the register for the result
.macro ABSDIFF X_r, Y_r, Return_r

sub   \X_r, \X_r, \Y_r  @ Subtract y_b from y_a
cmp   \X_r, #0          @ Compare result to zero
rsblt \X_r, \X_r, #0    @ If less than zero, negate to get absolute value 
mov   \Return_r, \X_r   @ Move the result to the return register

.endm


@ Will unpack the value kept in the var to_unp_r and save the x value 
@ in the X register and the y value in the Y register
@ to_unp_r:  Register with the 2 positions to unpack
@ X_r : Register for the X position
@ Y_r : Register for the Y position 
.macro unpack to_unp_r, X_r, Y_r 

movw \Y_r, #0xffff  @ Mask for the lower 16 bit

and \Y_r, \Y_r, \to_unp_r  @ And with the mask to get the Y position
mov   \X_r, \to_unp_r, lsr #16  @ Shift right to extract X_r


.endm

@@ @brief uint32_t manhattan_distance_asm(const uint32_t a, const uint32_t b)
@@ Computes the Manhattan distance between a and b
@@ @param a (r0)
@@ @param b (r1)
@@ @return distance in r0
.global manhattan_distance_asm
manhattan_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@
        @ Reminder: distance = |x_a - x_b| + |y_a - y_b|

        unpack r0, r2, r3 @ r2 = x_a, r3 = y_a
        unpack r1, r4, r5 @ r4 = x_b, r5 = y_b

        ABSDIFF r2, r4, r0 @ r0 = |x_a - x_b|
        ABSDIFF r3, r5, r1 @ r1 = |y_a - y_b|

        add r0, r0, r1 @ r0 = |x_a - x_b| + |y_a - y_b|

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr

@@ @brief uint32_t ssd_semi_distance_asm(const uint32_t a, const uint32_t b)
@@ Computes the Sum of Squared Differences semi distance between a and b
@@ @param a (r0)
@@ @param b (r1)
@@ @return distance in r0
.global ssd_semi_distance_asm
ssd_semi_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@
        @ reminder: distance = (x_a - x_b)^2 + (y_a - y_b)^2

        unpack r0, r2, r3 @ r2 = x_a, r3 = y_a
        unpack r1, r4, r5 @ r4 = x_b, r5 = y_b
        
        @ The formula doesnt need the absolute value of the difference but it doesnt
        @ matter since the square of a negative number is the same as the square of the positive number 
        ABSDIFF r2, r4, r0 @ r0 = |x_a - x_b|
        ABSDIFF r3, r5, r1 @ r1 = |y_a - y_b|

        mul r0, r0, r0 @ r0 = (x_a - x_b)^2
        mul r1, r1, r1 @ r1 = (y_a - y_b)^2
        add r0, r0, r1 @ r0 = (x_a - x_b)^2 + (y_a - y_b)^2

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr

