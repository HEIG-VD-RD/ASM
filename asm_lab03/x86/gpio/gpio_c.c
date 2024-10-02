/* Ne pas modifier */

int config_register(volatile int gpio_reg) {
        const unsigned int UPPER_BYTE_MASK = 0xFF000000;

        // Set bit 0 to 0
        gpio_reg &= ~(unsigned int)0x1;

        // Set bits 4,5,6, and 7 to 1
        gpio_reg |= 0xF << 4;

        // Clear upper byte
        gpio_reg &= ~UPPER_BYTE_MASK;

        // Set upper byte
        gpio_reg |= (0x66 << 3*8);

        // Divide value by 8
        gpio_reg /= 8;

        // Composite operation
        gpio_reg = (gpio_reg | 0xA5) & (0xFFEEDD << 3);

        return gpio_reg;
}
