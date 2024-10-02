@ ASM
@ Author : Rafael Dousse

@@ @brief uint32_t discrete_distance_asm(const uint32_t a, const uint32_t b)
@@ Computes the discrete distance between a and b
@@ @param a (r0)
@@ @param b (r1)
@@ @return distance in r0
.global discrete_distance_asm
discrete_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@
        @ Distance = 0 if a = b else distance = 1

        cmp     r0, r1 @ We compare r0 and r1             
        moveq   r0, #0 @ We return 0 if a == b
        movne   r0, #1 @ return 1 if a != b

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr

@@ @brief uint32_t hamming_distance_asm(const uint32_t a, const uint32_t b)
@@ Computes the Hamming distance between a and b
@@ @param a (r0)
@@ @param b (r1)
@@ @return distance in r0
.global hamming_distance_asm
hamming_distance_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@
        @ Value distance between 2 points is 1 if 2 points are different on 1 axe
        @ 2 if the are different on 2 axex or 0 if they are equals

        mov   r4, #0
        movw r5,  #0xffff

        and   r2, r0, r5         @ r2 = y_a from position a
        and   r3, r1, r5         @ r3 = y_b from position b
        cmp   r2, r3
        addne r4, #1             @ Increment distance if y_a != y_b

        mov   r2, r0, lsr #16    @ r2 = x_a from position a
        mov   r3, r1, lsr #16    @ r3 = x_b from position b
        cmp   r2, r3
        addne r4, #1             @ Increment distance if x_a != x_b

        mov r0, r4               @ Return value

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr

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
        @ distance = |x_a - x_b| + |y_a - y_b|

        mov r4, #0

       
        movw r5, #0xffff      @ Load mask for lower 16 bit
        and   r2, r0, r5      @ Extract y_a from position a
        and   r3, r1, r5      @ Extract y_b from position b
        sub   r2, r2, r3      @ Subtract y_b from y_a
        cmp   r2, #0          @ Compare result to zero
        rsblt r2, r2, #0      @ If less than zero, negate to get absolute value -> instruction found here: https://developer.arm.com/documentation/dui0473/m/arm-and-thumb-instructions/rsb
        add   r4, r4, r2      @ Add absolute difference to result

        
        mov   r2, r0, lsr #16  @ Shift right to extract x_a
        mov   r3, r1, lsr #16  @ Shift right to extract x_b
        sub   r2, r2, r3       @ Subtract x_b from x_a
        cmp   r2, #0           @ Compare result to zero
        rsblt r2, r2, #0       @ If less than zero, negate to get absolute value
        add   r0, r4, r2       @ Add absolute difference to result, store in r0

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
        @ distance = (x_a - x_b)^2 + (y_a - y_b)^2

        movw  r5, #0xFFFF 
        and   r2, r0, r5              @ r2 = y_a
        and   r3, r1, r5              @ r3 = y_b
        sub   r2, r2, r3              @ r2 = y_a - y_b
        mul   r2, r2, r2              @ r2 = (y_a - y_b)^2
        mov   r4, r2                  @ Store result in r4

        mov   r2, r0, lsr #16         @ r2 = x_a
        mov   r3, r1, lsr #16         @ r3 = x_b
        sub   r2, r2, r3              @ r2 = x_a - x_b
        mul   r2, r2, r2              @ r2 = (x_a - x_b)^2

        add   r0, r4, r2              @ r0 = (y_a - y_b)^2 + (x_a - x_b)^2

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr




@@ @brief uint32_t get_directions_asm(const char* map, const uint32_t position)
@@ Returns possible directions given a position on a map
@@ @param pointer on map (r0)
@@ @param position (r1)
@@ @return possible directions
.equiv MAP_SIZE_X, 8
.equiv MAP_SIZE_Y, 10
.equiv WALL_ASCII, 0x57 @ ASCII for 'W'
.equiv NORTH_MASK, 0x8  @0b1000
.equiv SOUTH_MASK, 0x4  @0b0100
.equiv EAST_MASK,  0x2  @0b0010
.equiv WEST_MASK,  0x1  @0b0001
.global get_directions_asm
get_directions_asm:
	mov   r12, sp
        stmfd sp!, {r4-r12, lr}

        @@@@@@@@@@@ A completer @@@@@@@@@@

        mov     r4, #0xF             @ We intitialize the result register with all the directions 0xF

        movw    r5, #0xFFFF
        mov     r2, r1, lsr #16      @ Get x position r2 = x 
        and     r3, r1, r5           @ Get y position r3 = y
        
        mov     r6, #MAP_SIZE_Y
        mla     r5, r2, r6, r3       @ Used to get the position in a linear format r5 = x * MAP_SIZE_Y + y 
        add     r5, r0, r5           @ r5 = address of current position on the map
      
check_north:
        cmp       r2, #0             @ Compare with 0 to know if our x position is at the beginning and we cannot go further up
        biceq r4, r4, #NORTH_MASK    @ If equals we clear the bit and go to the next check
        beq check_east

        sub     r7, r5, #MAP_SIZE_Y  @ We check on the position "one up"
        ldrb    r8, [r7]             @ Load the Value    
        cmp     r8, #WALL_ASCII      @ We check if there is a wall    
        biceq r4, r4, #NORTH_MASK    @ If there is, then we clear the bit  
    
check_east:
 
        cmp     r3, #MAP_SIZE_Y - 1  @ Compare with the size to know if our y position is at end
        biceq   r4, r4, #EAST_MASK   @ If equals we clear the bit and go to the next check
        beq     check_west

        add     r7, r5, #1          @ We check on the position "one on the right"
        ldrb    r8, [r7]            @ Load the Value    
        cmp     r8, #WALL_ASCII     @ We check if there is a wall 
        biceq   r4, r4, #EAST_MASK  @ If there is, then we clear the bit  

check_west:

        @ West
        cmp     r3, #0               @ Compare with 0 to know if our y position is at the beginning and we cannot go further left
        biceq   r4, r4, #WEST_MASK   @ If equals we clear the bit and go to the next check
        beq     check_south

        sub     r7, r5, #1           @ We check on the position "one on the left"
        ldrb    r8, [r7]             @ Load the Value     
        cmp     r8, #WALL_ASCII      @ We check if there is a wall 
        biceq   r4, r4, #WEST_MASK   @ If there is, then we clear the bit  
     
check_south:
        @ South
        cmp     r2, #MAP_SIZE_X - 1  @ Compare with the size to know if our x position is at end
        biceq   r4, r4, #SOUTH_MASK  @ If equals we clear the bit and go to the end
        beq     end

        add     r7, r5, #MAP_SIZE_Y  @ We check on the position "one down"
        ldrb    r8, [r7]             @ Load the Value  
        cmp     r8, #WALL_ASCII      @ We check if there is a wall  
        biceq   r4, r4, #SOUTH_MASK  @ If there is, then we clear the bit 
      
end:
        mov     r0, r4 @ Set final result to r0

	@@@@@@@@@@@ ----------- @@@@@@@@@@

        ldmfd sp!, {r4-r12, lr}
        mov   sp, r12
        mov   pc, lr