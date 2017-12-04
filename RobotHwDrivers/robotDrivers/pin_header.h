#ifndef _PIN_HEADER_H_
#define _PIN_HEADER_H_

// H-Bridge motor driver pins
#define    L_CTRL1   2
#define    L_CTRL2   4
#define    L_PWM     5

#define    R_CTRL1   7
#define    R_CTRL2   8
#define    R_PWM     6

// Ultra sound
//#define    TRIGGER   A0
//#define    ECHO      A1
#define TRIGGER_PIN   A0 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      A1 // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


#endif
