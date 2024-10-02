/*
 * exo3.c - Endianness determination exercise
 *
 * Author(s): Your Name(s)
 */

#include <stdio.h>

int check_endianness() {

	unsigned int x = 1;
	char *c = (char*) &x;

	return (int)*c;
}

int main(int argc, char **argv) {
    if (check_endianness())
        printf("This machine is little-endian.\n");
    else
        printf("This machine is big-endian.\n");
    
    return 0;
}
