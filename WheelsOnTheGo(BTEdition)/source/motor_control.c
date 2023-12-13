#include "motor_control.h"
#include "MKL25Z4.h"
#include "tpm.h"

#define MASK(x) (1UL << (x))
bool stop = true;

void Start_Motor(uint16_t speed_a, uint16_t speed_b) {
	TPM0->CONTROLS[5].CnV = speed_a;
	TPM0->CONTROLS[0].CnV = speed_b;
}

void Set_Direction(car_state_t dir) {
	switch (dir) {
	case FORWARD: {
		PTB->PSOR |= MASK(9);
		PTB->PSOR |= MASK(11);
		PTB->PCOR |= MASK(8);
		PTB->PCOR |= MASK(10);
		break;
	}
	case BACKWARD: {
		PTB->PCOR |= MASK(9);
		PTB->PCOR |= MASK(11);
		PTB->PSOR |= MASK(8);
		PTB->PSOR |= MASK(10);
		break;
	}
	case RIGHT: {
		PTB->PSOR |= MASK(9);
		PTB->PCOR |= MASK(11);
		PTB->PCOR |= MASK(8);
		PTB->PSOR |= MASK(10);
		break;
	}
	case LEFT: {
		PTB->PCOR |= MASK(9);
		PTB->PSOR |= MASK(11);
		PTB->PSOR |= MASK(8);
		PTB->PCOR |= MASK(10);
		break;
	}
	case STOP: {
		PTB->PCOR |= MASK(8) | MASK(9) | MASK(10) | MASK(11);
	}
	}
}

void state_machine(char ch) {
	if (ch == '1') {
		Set_RGB(0xFF00);
		Set_Direction(FORWARD);
		stop = true;
	} else if (ch == '2') {
		Set_RGB(0xFF0000);
		if (stop) {
			Set_Direction(STOP);
			stop = false;
		} else {
			Set_Direction(BACKWARD);
			stop = true;
		}
	} else if (ch == '3') {
		Set_RGB(0xFFFF);
		Set_Direction(RIGHT);
		stop = true;
	} else if (ch == '4') {
		Set_RGB(0xFFFF00);
		Set_Direction(LEFT);
		stop = true;
	}
}

