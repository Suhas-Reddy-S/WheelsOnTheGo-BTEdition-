
#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include<stdint.h>
typedef enum state {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	STOP
} car_state_t;

void Start_Motor(uint16_t speed_a, uint16_t speed_b);

void Set_Direction(car_state_t dir);

void state_machine(char ch);

#endif // _STATE_MACHINE_H_
