        # Authors : Rafael Dousse
        # File    : asm_filter.S
        # Date    :
        # AT&T Syntax

#include "image_processing.h"

.globl asm_filter
.type asm_filter, @function
.extern edge_detection_3x3

# macro to test if a pixel is on the border of the image
.macro isOnBoard position, limit
        movl    (\position), %eax # Load x or y into eax
        test    %eax, %eax        # Test if x or y is 0
        je      copy_pixel        # If 0, jump to copy_pixel

        movl    (\limit), %ebx    # Load width or height into ebx
        decl    %ebx              # Decrement width or height

        cmpl    %ebx, %eax        # Compare x or y with width or height
        je      copy_pixel        # If x or y is equal to width or height, jump to copy_pixel
.endm

.macro get_pix_offset reg_input

        movl  (y), \reg_input
        imull (width), \reg_input
        addl  (x), \reg_input

.endm


.text
        # Function arguments :
        # ebp + 8  : address of source image data (pointer to unsigned 8 bits)
        # ebp + 12 : address of destination image data (already allocated)
        # ebp + 16 : width of the image (signed 32 bits)
        # ebp + 20 : height of the image (signed 32 bits)
        # ebp + 24 : x position of pixel (signed 32 bits)
        # ebp + 28 : y position of pixel (signed 32 bits)

asm_filter:
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        pushl %esi                      # Save registers
        pushl %edi                      # Save registers
        pushl %ebx                      # Save registers

        # Copy of the arguments for easier access
        movl 8(%ebp),   %esi            # esi is for source
        movl 12(%ebp),  %edi            # edi is for destination

        movl 16(%ebp),  %eax
        movl %eax,      (width)         # width holds the width parameter
        movl 20(%ebp),  %eax
        movl %eax,      (height)        # height holds the height parameter
        movl 24(%ebp),  %eax
        movl %eax,      (x)             # x holds the x position
        movl 28(%ebp),  %eax
        movl %eax,      (y)             # y holds the y position



check_if_on_border:

        # TODO : Check if the current pixel is on the border of the image
        # when on border jump to copy_pixel else continue

        isOnBoard x, width # Test if x is on the border

        isOnBoard y, height # Test if y is on the border


        jmp     conv_filter  # If not on border, jump to conv_filter

conv_filter:

        # TODO : Implement the convolution filter

        
        get_pix_offset %ecx          # Calculate the pixel offset
        movl %esi, %edx            # Store base address of the picture in edx
        addl %ecx, %edx            # Add the offset to the base address
        incl  %edx                 # Move to the pixel at the position (x+1, y)
        addl (width), %edx         # Move to the pixel at the position (x+1, y+1)
        movl $edge_detection_3x3, %eax  # Load the matrix filter in eax

        movl $0, %ebx                   # Set ebx to 0 to then store the result
        movl $0, (row_it)               # Initiate the line iterator
        movl $0, (col_it)               # Initiate the column iterator
        movl $0, (accumulator)          # Initiate the accumulator

        row_loop:
                col_loop:
                        movl %ebx, (accumulator) # Load the accumulator with the pixel value
                        movsbl (%eax), %ecx      # Get byte from the kernel then extend the sign 
                        movzbl (%edx), %ebx      # Get byte from the picture and extend with zeros
                        imull %ebx, %ecx         # Multiply the values

                        movl (accumulator), %ebx # Load accumulator
                        addl %ecx, %ebx          # Add the result to the accumulator

                        incl %eax                # Move to the next element in the kernel
                        decl %edx                # Move to the next pixel in the picture
                        incl (col_it)            # Increment the column index

                        cmpl $MATRIX_SIZE, (col_it) # Check if all columns are processed
                        jne col_loop                # If not continue processing columns

                movl $0, (col_it)               # Reset the column index
                incl (row_it)                   # Increment the line index
                addl $MATRIX_SIZE, %edx         # Move to the next line 
                subl (width), %edx              # Adjust the pointer for the next line

                cmpl $MATRIX_SIZE, (row_it)     # Check if all lines are processed
                jne row_loop                    # If not continue processing lines

                cmpl $0, %ebx                   # Check if the accumulator is negative
                jge conv_filter_no_neg          # If not jump to no negative check
                negl %ebx                       # Negate the accumulator if negative
        conv_filter_no_neg:

                cmpl $MAX_PIXEL_VALUE, %ebx     # Limit the accumulator to 255
                jle conv_filter_no_max          # If less than or equal to 255, jump to max check
                movl $MAX_PIXEL_VALUE, %ebx     # Set the accumulator to 255 since its over
        conv_filter_no_max:

                get_pix_offset %edx             # Get the pyxel offset   
                movl %ebx, (res)                # Store the result in res variable

                movl %edi, %eax                 # Load the base address of the destination image
                addl %edx, %eax                 # Add the offset to the destination address

                movl %ebx, (%eax)               # Store the result in the destination image


exit:
        popl %ebx                       # Restore registers
        popl %edi       	        # Restore registers
    popl %esi                       # Restore registers
    leave                           # Restore stack frame
    ret                             # Return to caller

copy_pixel:

        # TODO : Copy pixel from source to destination image

        get_pix_offset %ecx            # get the offset of the pixel in the image

        # Calculer l'adresse source
        movl    %esi, %edx             # load the address of the source image in edx
        addl    %ecx, %edx             # add the offset to the source address

        # Calculer l'adresse destination
        movl    %edi, %ebx             # load the address of the destination image in ebx
        addl    %ecx, %ebx             # add the offset to the destination address

        # Copier le pixel
        movl    (%edx), %eax           # load the pixel from the source
        movl    %eax, (%ebx)           # store the pixel in the destination

        jmp exit                        # exit

.bss
        # Block Started by Symbol Section
        # space is 4 bytes because the values are 32 bit

width:                                  # image width
        .space 4
height:                                 # image height
        .space 4
x:                                      # x position of pixel
        .space 4
y:                                      # y position of pixel
        .space 4
res:                                    # result of convolution
        .space 4
row_it:                               # line iterator
        .space 4
col_it:                                # column iterator
        .space 4
accumulator:                            # accumulator for the convolution
        .space 4
.data


.equiv MAX_PIXEL_VALUE, 255
.equiv MATRIX_SIZE, 3