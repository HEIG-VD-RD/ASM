#include <gyroscope.h>
#include <common.h>
#include <exports.h>

void gyro_init(void)
{
	/* TODO: initialize the gyroscope */
	printf("[GYRO] Initialized\n");

}

void gyro_capture(char *gyro_data)
{
	/* TODO: change the state of the GPIO corresponding to the LED */
	printf("[GYRO] Capture %s\n", gyro_data);
}

void gyro_calibrate(void)
{
	/* TODO: calibrate the gyroscope */
	printf("[GYRO] Calibrate\n");

}
