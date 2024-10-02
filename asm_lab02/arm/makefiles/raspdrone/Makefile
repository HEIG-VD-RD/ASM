##########################################################################################

#############
# Toolchain #
#############

CC 		= arm-linux-gnueabihf-gcc
AR 		= arm-linux-gnueabihf-ar
LD		= arm-linux-gnueabihf-ld
OBJCOPY = arm-linux-gnueabihf-objcopy

#########################################################################

#################
# General flags #
#################

CFLAGS = -fno-builtin -I../../../u-boot/include -I../../../u-boot/arch/arm/include -I./ -D__KERNEL__ -marm -g
LDFLAGS = -L. -ldriver -T asm.lds
#########################################################################

#################
# FILES #
#################

# Source files (.c)
SOURCE = $(wildcard *.c)
# Object files (.o)
OBJETS = $(SOURCE:.c=.o)
# Files required for the library
LIBRAIRIE_OBJ = motor.o gyroscope.o camera.o
# Desired library file
LIBRAIRIE = libdriver.a
# Desired executable file
TARGET = raspdrone

#########################################################################

all: $(TARGET).bin

# Link the final executable
$(TARGET): $(OBJETS) $(LIBRAIRIE)
	$(LD) $(LDFLAGS) $^ -o $@

# Compile the library from specified objects
$(LIBRAIRIE): $(LIBRAIRIE_OBJ)
	$(AR) -r $@ $^

# Generic rule to compile any .c file into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Convert the executable into binary format
$(TARGET).bin: $(TARGET)
	$(OBJCOPY) -O binary $< $@

# Clean up generated files
clean:
	rm -f $(OBJETS) $(LIBRAIRIE) $(TARGET) $(TARGET).bin

.PHONY: all clean
#########################################################################
