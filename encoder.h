
#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h"

/* =========================================
   MOTOR GB37-520 CONFIGURATION
   ========================================= */

#define ENCODER_PPR             11U
#define ENCODER_GEAR_RATIO      30U
#define ENCODER_MULTIPLIER      4U

/* Expected quadrature x4 counts per output revolution */
#define ENCODER_CPR \
    (ENCODER_PPR * ENCODER_GEAR_RATIO * ENCODER_MULTIPLIER)

/* Sampling time */
#define ENCODER_SAMPLE_TIME_MS  50U

/* =========================================
   WHEEL CONFIGURATION
   ========================================= */

#define WHEEL_DIAMETER_M        0.065f

#define WHEEL_CIRCUMFERENCE \
    (3.14159265f * WHEEL_DIAMETER_M)

/* =========================================
   ENCODER GLOBAL VARIABLES
   ========================================= */

/* Hardware counter */
extern volatile uint32_t Encoder_Count;

/* Count difference per sample */
extern volatile int32_t Encoder_Delta;

/* Relative accumulated position */
extern volatile int32_t Encoder_Total_Count;

/* Motor speed */
extern volatile float Encoder_Speed_RPM;

/* Direction: +1 / -1 / 0 */
extern volatile int8_t Encoder_Direction;

/* =========================================
   ROBOT MOTION VARIABLES
   ========================================= */

/* Signed linear velocity (m/s) */
extern volatile float Robot_Linear_Velocity;

/* Signed angular velocity (rad/s) */
extern volatile float Robot_Angular_Velocity;

/* Signed displacement from starting point (m) */
extern volatile float Robot_Distance;

/* Signed speed (km/h) */
extern volatile float Robot_Speed_KMH;

/* Total traveled distance (m), always non-decreasing */
extern volatile float Robot_Total_Distance;

/* =========================================
   FUNCTION PROTOTYPES
   ========================================= */

void Encoder_Init(void);

void Encoder_Update(void);

void Encoder_Reset(void);

void Motor_Reverse(uint8_t duty_percent);

#endif
