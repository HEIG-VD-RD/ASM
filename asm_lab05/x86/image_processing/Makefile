# ASM 2023
# Makefile for image_processing
# Author : Rick Wertenbroek

# Targets and source files
##########################
TARGET = image_processing
C_SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.S)
OBJECTS = $(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

# Flags
#######
ASFLAGS += -m32 -g
CFLAGS += -m32 -O0 -std=c11 -g -fverbose-asm -Wno-misleading-indentation
LDFLAGS += -m32
LDLIBS += -lm

# Rules
#######
all : $(DEPENDENCIES) $(TARGET)

# Rule to build the target
$(TARGET) : $(OBJECTS)

# Do not include the depency rules for "clean"
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif

# Rule to generate the dependency files
%.d : %.S
	$(CC) $(INCLUDE_DIRS) -MG -MP -MM -MT '$(@:.d=.o)' $< -MF $@
%.d : %.c
	$(CC) $(INCLUDE_DIRS) -MG -MP -MM -MT '$(@:.d=.o)' $< -MF $@

clean :
	rm -rf $(TARGET) $(OBJECTS) $(DEPENDENCIES)

.PHONY : all clean
