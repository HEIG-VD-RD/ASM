-include ../x86.mk

#########################################################################

########################
# Application specific #
########################

CPPFLAGS += -mlzcnt

TARGET = main

CRT = crt0.o stubs.o

#########################################################################

all : $(TARGET)

$(TARGET) : % : %.o $(CRT)
	$(LD) -g -T asm.lds -o $@ $^ -L/opt/toolchains/i686-linux_6.4.0/lib/gcc/i686-buildroot-linux-gnu/6.4.0/ -lgcc
	$(OBJCOPY) -O binary $@ $@.bin 2>/dev/null

#########################################################################

clean :
	rm -f *.o *.a *.bin *.su
	rm -f $(TARGET)

.PHONY : clean