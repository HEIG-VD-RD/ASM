# Author : Rafael Dousse

.global asm_config_register

# int asm_config_register(int gpio_reg)
# input gpio_reg in eax
# output in eax
asm_config_register:
        pushl %ebp                # Save old stack frame
        movl  %esp, %ebp          # Set new stack base
        movl  8(%ebp), %eax       # copy the argument in eax
        pushl %ebx

        ########## A completer ##########

        movl %eax, %edx

        #  Set bit 0 to 0
        andl $0xFFFFFFFE, %edx

        # Set bits 4,5,6, and 7 to 1
        orl $0x000000F0, %edx
        # Clear upper byte
        andl $0x00FFFFFF, %edx
        # Set upper byte
        orl $0x66000000, %edx
        # Divide value by 8
        sarl $3, %edx
        # Composite operation
        orl $0xA5, %edx
        movl $0xFFEEDD, %ebx
        sall $3, %ebx
        andl %ebx, %edx
        movl %edx, %eax

exit:
        popl  %ebx
        leave                     # Restore stack frame
        ret                       # Return to caller
