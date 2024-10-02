#include <camera.h>
#include <common.h>
#include <exports.h>


void camera_init(void)
{
	/* TODO: initialize the camera */
	printf("[CAM] Initialized\n");
}

void camera_on(int resolution)
{
	/* TODO: enable the camera with a certain resolution */
	printf("[CAM] Turned on with resolution %d\n", resolution);
}

int *camera_acquire(void)
{
	/* TODO: retrieve a pointer to an image. */
	printf("[CAM] Acquisition asked\n");
	return (int *) 0xa10000;
}
