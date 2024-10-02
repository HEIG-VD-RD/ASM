#include <common.h>
#include <exports.h>
#include <motor.h>

void motor_init(int motor_id)
{
	/* TODO: Initialize the motor corresponding
	 * to the <motor_id>
	 */
	printf("[MOTOR] Initialized id %d\n", motor_id);
}

void motor_set_power(int motor_id, int power_level)
{
	/* TODO: set the power level of the motor */
	printf("[MOTOR] Set power level of id %d at %d\n", motor_id, power_level);

}

int motor_get_power(int motor_id)
{
	/* TODO: return the power level for this motor */
	printf("[MOTOR] Power asked for id %d\n", motor_id);
	return 0;
}
