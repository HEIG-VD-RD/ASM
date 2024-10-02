#include <common.h>
#include <exports.h>
#include <pwm_drone.h>

void pwm_init(void)
{
	/* TODO: initialize the PWM */
	printf("[PWM] Initialized\n");
}

int pwm_enable(struct pwm_config *cfg)
{
	/* TODO: enable the PWM with correct config */
	printf("[PWM] Enabled\n");
	return 0;
}

void pwm_disable(void)
{
	/* TODO: disable the PWM with correct config */
	printf("[PWM] Disabled\n");
}

