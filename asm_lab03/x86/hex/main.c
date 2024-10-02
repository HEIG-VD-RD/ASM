#include <common.h>
#include <exports.h>

extern int char_to_hex(int hex_string);

int main(int argc, char *argv[]) {
	/* Example input */
	int input = 'A';

	/* Convert input */
	int hex = char_to_hex(input);

	printf("Result: '%c' = %c%c%c%c\n", input, hex >> 24, hex >> 16, hex >> 8, hex);

	input = 'R';
	hex = char_to_hex(input);
	printf("Result: '%c' = %c%c%c%c\n", input, hex >> 24, hex >> 16, hex >> 8, hex);

	input = 'M';
	hex = char_to_hex(input);
	printf("Result: '%c' = %c%c%c%c\n", input, hex >> 24, hex >> 16, hex >> 8, hex);

	input = 'Z';
	hex = char_to_hex(input);
	printf("Result: '%c' = %c%c%c%c\n", input, hex >> 24, hex >> 16, hex >> 8, hex);

	return 0;
}
