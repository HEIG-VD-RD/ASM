#include <autopilot.h>
#include <gyroscope.h>
#include <navigation.h>
#include <common.h>
#include <exports.h>

void autopilot_init(void)
{
	/* TODO: initialize the autopilot system */

	gyro_init();
	printf("[AUTOPILOT] Initialized\n");
}

void autopilot_activate(void)
{
	int *gpx;
	struct coord *sys_coord;
	char *gyro_data;

	/* TODO: activate the autopilot system */
	gyro_calibrate();
	gyro_capture(gyro_data);

	navigation_compute_destination(gpx);

	sys_coord = navigation_get_coord();
	printf("[AUTOPILOT] Activated\n");

}

void autopilot_deactivate(void)
{
	/* TODO: deactivate the autopilot system */
	printf("[AUTOPILOT] Deactivated\n");

}
