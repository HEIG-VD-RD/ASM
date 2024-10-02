/*
 * exo1.c - String allocation and copy exercise
 *
 * Author(s) : Dousse Rafael
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

char *allocate_string(const char *string, int length) {
    // TODO
	char* allocate = (char*)malloc(length*sizeof(char));
	for(int i = 0; i < length; i++){
		*(allocate + i) = *(string + i);


	}

	return allocate;
}

int main(int argc, char **argv) {

    char* string = "Welcome to ASM\n";

    char* allocate = allocate_string(string,strlen(string));
    printf("%s",allocate);
    free(allocate);

    return 0;
}
