-include ../arm.mk

#########################################################################

########################
# Application specific #
########################

# Used in target application
CFLAGS += -D__QEMU_BARE__=1

TARGETS = print path1 path2
OBJS = student_functions_asm.o

CRT = crt0.o stubs.o

#########################################################################

all : $(TARGETS)

$(TARGETS) : % : %.o $(OBJS) $(CRT)
	$(LD) -g -T asm.lds -o $@ $^
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

print.o : path_finding.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -DPRINT -c -o $@ $<

path1.o : path_finding.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

path2.o : path_finding.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -DNEIGHBORS_STUDENT -c -o $@ $<

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGETS)

.PHONY : clean