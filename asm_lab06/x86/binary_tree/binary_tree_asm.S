# Author : Rafael Dousse

.global traverse_tree_asm

.extern printf

# Macro that will print what is at the end of string_ptr (char *)
# with an extra space at the end (through the format string used).
.macro PUT_S string_ptr
    pushal
    push \string_ptr
    push $format_string
    call printf
    add  $0x8, %esp
    popal
.endm

## @brief void traverse_tree_asm(Node *root);
## Traverses a binary search tree in natural order and prints data if available
##
## @param Node *root the root of the tree to traverse and prints
traverse_tree_asm:
        # Intro
        pushl %ebp                      # Save old stack frame
        movl  %esp, %ebp                # Set new stack base
        pushal                          # Save all registers
        movl  8(%ebp), %eax             # copy the argument in eax
	####### Student code below
	
        # TODO

        movl  $0, %ecx        # size of the stack

        start_loop:
        cmpl  $0, %eax
        je    check_stack     # if current is null, check stack

        left_descend:
        cmpl  $0, %eax
        je    check_stack     # if current is null, check stack
        pushl %eax            # push current onto stack
        incl  %ecx            # increment stack size
        movl  8(%eax), %eax   # move to left child
        jmp   left_descend    # we go left again

        check_stack:
        cmpl  $0, %ecx        # if stack is empty and current is null, end
        je    end_traversal
        
        right_descend:
        popl  %eax            # pop from stack
        decl  %ecx            # decrement stack size
        movl  4(%eax), %ebx   # get data from node
        PUT_S %ebx            # print data
        movl  12(%eax), %eax  # move to right child
        jmp   start_loop

	end_traversal:
	######## End of student code
        # Outro
        popal                           # Restore all register
        leave                           # Restore stack frame
        ret                             # Return to caller


.data
format_string:
    .string "%s "
example_string:
    .string "Hello from ASM !"
