#include <common.h>
#include <exports.h>

extern int config_register(volatile int gpio_reg);
extern int asm_config_register(volatile int gpio_reg);

int main(int argc, char *argv[]) {

	volatile int gpio_c = 0xc001;
	volatile int gpio_S = 0xc001;

	gpio_c = config_register(gpio_c);
	gpio_S = asm_config_register(gpio_S);
        
	if (gpio_c == gpio_S) {
		printf("Same result with assembly and C\n");
	} else {
		printf("Result differs between assembly and C\n");
	}
	
	printf("Hit any key to exit ... ");
	while (!tstc());
	
	/* consume input */
	(void) getc();

	printf("\n\n");

	return 0;
}
