# Author : Rafael Dousse

.global parity_word
.global check_parity

# int parity_word(int value)
# input value in eax
# output in eax
parity_word:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        movl 8(%ebp), %eax              # copy the argument in eax
        pushl %ebx

        # ######### A completer ##########
        # Storing eax in ebx to do the operations
        movl %eax, %ebx
        # Shift of 8 to the right to have the first 2 bytes (byte 0 and byte 1) for the xor operation
        shrl $8, %ebx
         # Saving the 2 bytes in register cx for later operations
        movw %bx, %cx
        # Shifting 16 to the right to have the missing byte 2 in register bl
        shrl $16, %ebx
        # Xor operation between 2 bytes
        xorb %cl, %bl
        # xor operation between the 2 bytes and the missing byte
        xorb %ch, %bl
        # Saving the result (checksum) in register al
        movb %bl, %al
        
        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller

# int check_word(int value)
# input value in eax
# output in eax
check_parity:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        movl 8(%ebp), %eax              # copy the argument in eax
        pushl %ebx

        # ######### A completer ##########

        # - First we do the same operations as in parity_word to get the checksum
        # Storing eax in ebx to do the operations
        movl %eax, %ebx
        # Shift of 8 to the right to have the first 2 bytes (byte 0 and byte 1) for the xor operation
        shrl $8, %ebx
         # Saving the 2 bytes in register cx for later operations
        movw %bx, %cx
        # Shifting 16 to the right to have the missing byte 2 in register bl
        shrl $16, %ebx
        # Xor operation between 2 bytes
        xorb %cl, %bl
        # xor operation between the 2 bytes and the missing byte
        xorb %ch, %bl

        # - Then we compare the checksum with the value of the last byte
        # Saving the byte and checksum in register edx
        movl %eax, %edx
        # Saving the checksum recently calculated in edx
        movb %bl, %dl
        # Comparing the checksum given with the checksum calculated
        cmpb %al, %dl
        # If the checksums are the same, we return 0 with the xor operation and we jump to the exit if not we jump to the error_detected and return -1
        jnz error_detected
        xorl %eax, %eax
exit:

        popl %ebx
        leave                           # Restore stack frame
        ret                             # Return to caller

error_detected:
        # We put -1 in eax and we jump to the exit
        movl $-1, %eax
        jmp exit
