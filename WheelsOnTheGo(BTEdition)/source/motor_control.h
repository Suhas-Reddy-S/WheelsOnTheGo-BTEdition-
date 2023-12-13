#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

#include <stdint.h>

#define MIN_SPEED (0xFFFF)
#define MEDIUM_SPEED (0xFF)
#define MAX_SPEED (0x0)

/**
 * @brief Initializes the motor control system.
 *
 * This function initializes the necessary settings and configurations for motor control,
 * preparing the system for motor operation.
 */
void Init_Motors(void);

/**
 * @brief Starts the motors with specified speeds.
 *
 * This function starts the motors with the specified speeds for motor A and motor B.
 * The speed values are provided as parameters and control the rotation speed of each motor.
 *
 * @param speed_a Speed value for motor A.
 * @param speed_b Speed value for motor B.
 */
void Start_Motors(uint16_t speed_a, uint16_t speed_b);

/**
 * @brief Control the robot's movement based on the input character.
 *
 * This function interprets the input character 'ch' and performs corresponding
 * actions to control the robot's movement. It interacts with motor control functions
 * and updates the RGB LEDs based on the specified movements.
 *
 * @param ch Input character representing the desired robot movement.
 *           '1': Move forward.
 *           '2': Toggle between stop and backward movement.
 *           '3': Turn right with a brief delay and then stop.
 *           '4': Turn left with a brief delay and then stop.
 */
void Motor_Control(char ch);

#endif // _MOTOR_CONTROL_H_
