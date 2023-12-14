# Wheels On The Go ( BT Edition )
Code for Final Project of PES, Fall 2023, ECEN-5813


## Table of Contents

- [Project Functionality](#project-functionality)
- [Components Used](#components-used)
- [PIN Connection](#pin-connection)
- [Instructions To Drive](#instructions-to-drive)
- [Report](#report)
- [Challenges](#challenges)
- [Project Videos](#project-videos)
- [References](#references)
- [Acknowledgements](#acknowledgements)


## Project Functionality 

The core objective of this project is to engineer a Bluetooth-controlled car, utilizing the KL25Z4 microcontroller,
a Bluetooth module from sprakfun, FreeRTOS for multitasking, and a state machine for precise motor control. The 
project will demonstrate the following functionality:
- Bluetooth Control: The car will be remotely controlled via Arduino Bluetooth Controller app. The app will 
provide UI to direct its movements.
- State Machine: A meticulously designed state machine will govern the car's behavior, facilitating seamless
transitions between operational states. States may include forward motion, backward motion, turning, and
stopping.
- Concurrency: FreeRTOS will be harnessed to efficiently manage concurrent tasks, including Bluetooth
communication, motor control, and state transitions.

## Components Used
- KL25Z4 MCU from NXP
- Bluetooth Module from sparkfun with RN-41
- Motor Driver that supports two motors
- Power breakout board
- Two gear motors
- Two Wheels

## PIN Connection
- BT-Module Vcc to 3.3 V source
- Motot Drive VCC, V_M and Stand BY to 5.0 V source
- PTD[6] -> BT-Module TX
- PTD[7] -> BT-Module RX
- PTB[11] -> Motor Driver AIN 2
- PTB[10] -> Motor Driver AIN 1
- PTB[9] -> Motor Driver BIN 2
- PTB[8] -> Motor Driver BIN 1
- PTD[0] -> Motor Driver PWM_A
- PTD[5] -> Motor Driver PWM_B
- Make all ground pins common
- Connect Motor A and B to Motor Driver A and B OUTPUT Pins

## Instructions To Drive
- Install Arduino Bluetooth Controller App
- Connect to device
- Choose arrow option
- UP arrow: Moves the car forward and turns the on-board LED green.
- DOWN arrow: Stops the car on first click from any state and second click moves it backwards, turns the on-board LED red.
- Right arrow: Turns the car towards right about 90 degrees and stops it, turns the on-board LED cyan.
- Left arrow: Turns the car towards left about 90 degrees and stops it, turns the on-board LED yellow.

## Report

A brief report of the project can be found [here]().

## Challenges

The initial project proposal involved integrating a Wi-Fi module. Despite dedicating substantial time to 
referencing the module's datasheet, AT command set, and developing a UART driver, it was discovered that both the 
firmware and hardware of the Wi-Fi module were corrupted. 
Later, I switched to a bluetooth module from sparkfun and interfaced it to KL25Z4 over UART.

## Project Videos

## References 

[Alexander G. Dean for initialization routines](https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code)  
[Music Reactive LEDS by Ritika R. for FreeRTOS implementation](https://github.com/ritikar97/Music_Reactive_LEDs/tree/main)  
[Sparkfun for RN-41 AT command set](https://cdn.sparkfun.com/datasheets/Wireless/Bluetooth/bluetooth_cr_UG-v1.0r.pdf?_gl=1*lfkcle*_ga*MjQ3NzQwODA5LjE3MDA3ODY2NzQ.*_ga_T369JS7J9N*MTcwMjUxMjg0OS4xNS4wLjE3MDI1MTI4NDkuNjAuMC4w)
[Sparkfun for RN-41 DataSheet](http://cdn.sparkfun.com/datasheets/Wireless/Bluetooth/Bluetooth-RN-41-DS.pdf?_gl=1*1eytsyb*_ga*MjQ3NzQwODA5LjE3MDA3ODY2NzQ.*_ga_T369JS7J9N*MTcwMjUxMjg0OS4xNS4xLjE3MDI1MTI5OTAuNTUuMC4w)
