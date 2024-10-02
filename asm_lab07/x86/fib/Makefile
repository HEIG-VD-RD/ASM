-include ../x86.mk

#########################################################################

########################
# Application specific #
########################

# Used in target application
CFLAGS += -D__QEMU_BARE__=1

TARGETS = fib_silly fib fib_tail

OBJ = fib_asm.o

CRT = crt0.o stubs.o

#########################################################################

all : $(TARGETS)

fib_silly : main_silly.o $(OBJ) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

fib_tail : main_tail.o $(OBJ) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

main_silly.o : main.c
	$(CC) -c $(CPPFLAGS) -DFUN_TO_CHECK=silly_fib_asm $(CFLAGS) -c $< -o $@

main_tail.o : main.c
	$(CC) -c $(CPPFLAGS) -DFUN_TO_CHECK=fib_tail $(CFLAGS) -c $< -o $@

fib : main.o $(OBJ) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGETS)

.PHONY : clean