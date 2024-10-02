#include <navigation.h>
#include <gyroscope.h>
#include <camera.h>
#include <common.h>
#include <exports.h>

int navigation_compute_destination(int *gpx)
{
	int *img;
	char *gyro_data;

	/* TODO: compute the navigation based on GPS coordinates. */
	printf("[NAV] Computing destination..\n");
	gyro_capture(gyro_data);
	img = camera_acquire();

	printf("[NAV] Ended computaion\n");
	return 0;
}

struct coord *navigation_get_coord(void)
{
	/* TODO: returns the internal coordinate based on the navigation plan */
	printf("[NAV] Coordonate asked\n");
	return (struct coord *) 0x1000;
}
