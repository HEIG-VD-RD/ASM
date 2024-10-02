#include <common.h>
#include <exports.h>

extern int parity_word(int value);
extern int check_parity(int value);

int main(int argc, char *argv[]) {
	unsigned char byte2 = 0x13;
	unsigned char byte1 = 0x0A;
	unsigned char byte0 = 0xB2;
	int result;
	
	/* Create a word with the 3 bytes */
	int value = ((unsigned int)byte2 << 24) | ((unsigned int)byte1 << 16) | ((unsigned int)byte0 << 8);

	printf("Input : 0x%08x\n", value);
	int checksum = parity_word(value);
	printf("Value with checksum : 0x%08x\n", checksum);
	
	result = check_parity(checksum);
	printf("Checksum corresponds to bytes : %s\n", result ? "no" : "yes");

	int corrupted = checksum | 0x10000;
	printf("Corrupting value from 0x%08x to 0x%08x\n", checksum, corrupted);
	printf("Checksum corresponds to bytes : %s\n", check_parity(corrupted) ? "no" : "yes");

	printf("Hit any key to exit ... ");
	while (!tstc());
	
	/* consume input */
	(void) getc();

	printf("\n\n");

	return 0;
}
