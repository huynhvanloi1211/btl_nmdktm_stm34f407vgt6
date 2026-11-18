
#include "encoder.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

/* =========================================
   GLOBAL VARIABLES
   ========================================= */

/* Encoder hardware counter */
volatile uint32_t Encoder_Count = 0;

/* Count difference */
volatile int32_t Encoder_Delta = 0;

/* Relative accumulated count */
volatile int32_t Encoder_Total_Count = 0;

/* Motor speed */
volatile float Encoder_Speed_RPM = 0.0f;

/* Direction */
volatile int8_t Encoder_Direction = 0;

/* Robot motion */
volatile float Robot_Linear_Velocity = 0.0f;
volatile float Robot_Angular_Velocity = 0.0f;
volatile float Robot_Distance = 0.0f;
volatile float Robot_Speed_KMH = 0.0f;
volatile float Robot_Total_Distance = 0.0f;


/* =========================================
   INTERNAL VARIABLES
   ========================================= */

/* Middle point of 32-bit counter */
#define ENCODER_MIDPOINT  0x7FFFFFFFU

static uint32_t encoder_previous_count = ENCODER_MIDPOINT;


/* =========================================
   INITIALIZATION
   ========================================= */

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    /* Enable GPIOA clock */
    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA,
        ENABLE
    );

    /* Enable TIM2 clock */
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_TIM2,
        ENABLE
    );

    /* PA0 = TIM2_CH1 */
    GPIO_PinAFConfig(
        GPIOA,
        GPIO_PinSource0,
        GPIO_AF_TIM2
    );

    /* PA1 = TIM2_CH2 */
    GPIO_PinAFConfig(
        GPIOA,
        GPIO_PinSource1,
        GPIO_AF_TIM2
    );

    /* GPIO configuration */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_0 | GPIO_Pin_1;

    GPIO_InitStructure.GPIO_Mode =
        GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Speed =
        GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_OType =
        GPIO_OType_PP;

    GPIO_InitStructure.GPIO_PuPd =
        GPIO_PuPd_UP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /* =====================================
       TIM2 CONFIGURATION
       ===================================== */

    TIM_BaseInitStructure.TIM_Prescaler = 0;

    TIM_BaseInitStructure.TIM_CounterMode =
        TIM_CounterMode_Up;

    TIM_BaseInitStructure.TIM_Period =
        0xFFFFFFFFU;

    TIM_BaseInitStructure.TIM_ClockDivision =
        TIM_CKD_DIV1;

    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(
        TIM2,
        &TIM_BaseInitStructure
    );


    /* =====================================
       ENCODER INPUT CONFIGURATION
       ===================================== */

    TIM_ICStructInit(&TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel =
        TIM_Channel_1;

    TIM_ICInitStructure.TIM_ICPolarity =
        TIM_ICPolarity_Rising;

    TIM_ICInitStructure.TIM_ICSelection =
        TIM_ICSelection_DirectTI;

    TIM_ICInitStructure.TIM_ICPrescaler =
        TIM_ICPSC_DIV1;

    TIM_ICInitStructure.TIM_ICFilter = 6;

    TIM_ICInit(TIM2, &TIM_ICInitStructure);


    /* Channel 2 */
    TIM_ICInitStructure.TIM_Channel =
        TIM_Channel_2;

    TIM_ICInit(TIM2, &TIM_ICInitStructure);


    /* =====================================
       QUADRATURE ENCODER MODE
       ===================================== */

    TIM_EncoderInterfaceConfig(
        TIM2,
        TIM_EncoderMode_TI12,
        TIM_ICPolarity_Rising,
        TIM_ICPolarity_Rising
    );


    /* =====================================
       START COUNTER AT MIDDLE
       ===================================== */

    TIM_SetCounter(
        TIM2,
        ENCODER_MIDPOINT
    );

    encoder_previous_count = ENCODER_MIDPOINT;

    /* Enable TIM2 */
    TIM_Cmd(TIM2, ENABLE);


    /* =====================================
       RESET SOFTWARE VARIABLES
       ===================================== */

    Encoder_Count = ENCODER_MIDPOINT;

    Encoder_Delta = 0;

    Encoder_Total_Count = 0;

    Encoder_Speed_RPM = 0.0f;

    Encoder_Direction = 0;

    Robot_Linear_Velocity = 0.0f;

    Robot_Angular_Velocity = 0.0f;

    Robot_Distance = 0.0f;

    Robot_Speed_KMH = 0.0f;

    Robot_Total_Distance = 0.0f;
}


/* =========================================
   ENCODER UPDATE
   Call every 50ms
   ========================================= */

void Encoder_Update(void)
{
    uint32_t current_count;

    int32_t delta_count;

    float abs_delta;


    /* Read hardware counter */
    current_count = TIM_GetCounter(TIM2);

    Encoder_Count = current_count;


    /* Calculate signed count difference.
       Unsigned subtraction handles counter rollover.
    */
    delta_count = (int32_t)(
        current_count - encoder_previous_count
    );

    encoder_previous_count = current_count;

    Encoder_Delta = delta_count;


    /* Accumulate relative position */
    Encoder_Total_Count += delta_count;


    /* =====================================
       DIRECTION
       ===================================== */

    if (delta_count > 0)
    {
        Encoder_Direction = 1;
    }
    else if (delta_count < 0)
    {
        Encoder_Direction = -1;
    }
    else
    {
        Encoder_Direction = 0;
    }


    /* =====================================
       ABSOLUTE COUNT
       ===================================== */

    if (delta_count >= 0)
    {
        abs_delta = (float)delta_count;
    }
    else
    {
        abs_delta = -(float)delta_count;
    }


    /* =====================================
       SPEED RPM
       ===================================== */

    Encoder_Speed_RPM =
        ((float)delta_count * 60000.0f) /
        (
            (float)ENCODER_CPR *
            (float)ENCODER_SAMPLE_TIME_MS
        );


    /* =====================================
       ANGULAR VELOCITY
       ===================================== */

    Robot_Angular_Velocity =
        Encoder_Speed_RPM *
        2.0f * 3.14159265f / 60.0f;


    /* =====================================
       LINEAR VELOCITY
       ===================================== */

    Robot_Linear_Velocity =
        Encoder_Speed_RPM *
        WHEEL_CIRCUMFERENCE / 60.0f;


    /* =====================================
       SPEED KM/H
       ===================================== */

    Robot_Speed_KMH =
        Robot_Linear_Velocity * 3.6f;


    /* =====================================
       RELATIVE DISPLACEMENT
       ===================================== */

    Robot_Distance =
        (
            (float)Encoder_Total_Count /
            (float)ENCODER_CPR
        ) * WHEEL_CIRCUMFERENCE;


    /* =====================================
       TOTAL TRAVELED DISTANCE
       ===================================== */

    Robot_Total_Distance +=
        (
            abs_delta /
            (float)ENCODER_CPR
        ) * WHEEL_CIRCUMFERENCE;
}


/* =========================================
   RESET ENCODER
   ========================================= */

void Encoder_Reset(void)
{
    /* Reset hardware counter to midpoint */
    TIM_SetCounter(
        TIM2,
        ENCODER_MIDPOINT
    );

    encoder_previous_count = ENCODER_MIDPOINT;

    Encoder_Count = ENCODER_MIDPOINT;

    Encoder_Delta = 0;

    Encoder_Total_Count = 0;

    Encoder_Speed_RPM = 0.0f;

    Encoder_Direction = 0;

    Robot_Linear_Velocity = 0.0f;

    Robot_Angular_Velocity = 0.0f;

    Robot_Distance = 0.0f;

    Robot_Speed_KMH = 0.0f;

    Robot_Total_Distance = 0.0f;
}
