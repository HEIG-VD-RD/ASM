-include ../x86.mk

#########################################################################

########################
# Application specific #
########################

TARGETS = main

OBJ = copy_string_c.o string_asm.o

CRT = crt0.o stubs.o

#########################################################################

all : $(TARGETS)

$(TARGETS) : % : %.o $(OBJ) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGETS)

.PHONY : clean
