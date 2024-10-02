# Author : Rafael Dousse

.global char_to_hex

.equiv ASCII_0, 0x30 # Hexadecimal value of '0' in the ascii table
.equiv ASCII_7, 0x37 # Hexadecimal value of 7 which is 'A' - 10 in the ascii table so 55
.equiv DEC_10, 0x0A   # Hexadecimal value of 10
.equiv ZERO, '0'
.equiv X, 'X'

char_to_hex:
    pushl %ebp                # Save old stack frame
    movl  %esp, %ebp          # Set new stack base
    movl  8(%ebp), %eax       # copy the argument in eax
    pushl %ebx

    # ######### A completer ##########

    movb %al, %bl # Copy the argument in bl
    movb %al, %bh # Copy the argument in bh

    # Nibbles isolation
    andb $0x0F, %bl # Low nibble isolation
    shrb $4, %bh    # High nibble isolation

    # Low nibble conversion
    cmp $DEC_10, %bl  # Test if the low nibble is a number or a letter
    jl .convert_to_number_low # If it's a number (so < A), jump to the conversion to number else it keeps going to the conversion to letter

    .convert_to_letter_low:
    add $ASCII_7, %bl  # Add 55/0x37 (so the ascii value of 7) to the low nibble to convert it to a letter
    jmp .next # Jump to the next step

    .convert_to_number_low:
    addb $ASCII_0, %bl # Add 48/0x30 (so the ascii value of 0) to the low nibble to convert it to a number

    .next:
    # High nibble conversion - all the same as before
    cmp $DEC_10, %bh
    jl .convert_to_number_high

    .convert_to_letter_high:
    addb $ASCII_7, %bh 
    jmp .end
    .convert_to_number_high:
    add $ASCII_0, %bh

    .end:
    xorl %eax, %eax # Clear eax
    movb $'0', %ah # Set the high part of ax to 0
    movb $'x', %al # Set the low part of ax to x
    shll $16, %eax # Shift ax to the left by 16 bits
    movb %bh, %ah # Copy the high nibble in the high part of ax
    movb %bl, %al # Copy the low nibble in the low part of ax

out:
    popl %ebx
    leave                     # Restore stack frame
    ret                       # Return to caller

