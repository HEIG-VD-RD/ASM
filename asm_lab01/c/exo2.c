/*
 * exo2.c - Manipulation d'opérateurs logiques
 *
 * Author(s) : Name Surname, Name Surname
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <libleds.h>

#define MASK 0xFF000000


int main(int argc, char **argv) {
    init_reg_leds();

    uint32_t leds = get_reg_leds();

    // TODO
	printLed(leds);

	uint32_t fort = leds & MASK;
	printLed(fort);
	leds = ~leds;
	leds = leds & ~MASK;
	leds = leds | fort;
	printLed(leds);


    set_reg_leds(leds);

    return 0;
}

void printLed(uint32_t leds){

    for(int i = 0; i < 32; i++){
    	int value = (leds>> i) & 0x1;
    	printf("%d", value);
    }
    printf("\n");

}
