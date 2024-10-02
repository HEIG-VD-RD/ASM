
TOOL_PREFIX	=

CC		= gcc

STRIP		= $(TOOL_PREFIX)-strip
OBJCOPY		= $(TOOL_PREFIX)-objcopy
OBJDUMP		= $(TOOL_PREFIX)-objdump

TARGET	=  exo1 exo2 exo3 exo4-debug exo4-optimized

LIB = test_leds.a

all:	$(TARGET)

clean:
	rm -f *.o
	rm $(TARGET)

%.o: %.c
	$(CC) -g -I include -c $<

exo4-optimized.o: exo4.c
	$(CC) -O3 -g -I include -c $< -o $@

exo4-debug.o: exo4.c
	$(CC) -O0 -g -I include -c $< -o $@

# Generate an executable
$(TARGET): %: %.o $(LIB)
	gcc $< $(LIB) -o $@ 

.PHONY: all clean


