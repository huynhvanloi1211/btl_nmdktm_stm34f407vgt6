
#include "stm32f4xx.h"

#include "motor.h"
#include "encoder.h"
#include "delay.h"
#include "data_logger.h"

/* =====================================
   MOTOR CONTROL + DATA LOGGING

   Moi mau (sample) = 1 chu ky ENCODER_SAMPLE_TIME_MS:
     1. Cho den moc thoi gian ke tiep (khong troi)
     2. Encoder_Update()  -> delta/rpm TRUNG BINH trong chu ky vua qua
     3. Ghi log voi duty DANG DUOC AP DUNG trong chu ky do

   => u(k) = duty, y(k) = rpm cung chu ky, khong bi lech 1 mau.
   ===================================== */

/* Thoi gian thi nghiem lien tuc (ms), KHONG reset giua cac doan */
static uint32_t log_time_ms = 0;

/* Moc thoi gian lay mau ke tiep (SysTick ms) */
static uint32_t next_tick = 0;


void Motor_Run_Log(
    uint8_t duty,
    uint32_t duration_ms
)
{
    uint32_t samples = duration_ms / ENCODER_SAMPLE_TIME_MS;

    /* Buffer day roi thi khong chay dong co nua */
    if (Motor_Log_Finished)
    {
        Motor_Stop();
        return;
    }

    /* Set motor duty (ap dung cho toan bo doan nay) */
    if (duty == 0)
    {
        Motor_Stop();
    }
    else
    {
        Motor_Forward(duty);
    }

    while (samples > 0U)
    {
        samples--;

        /* Cho den moc lay mau (chong troi thoi gian) */
        next_tick += ENCODER_SAMPLE_TIME_MS;

        while ((int32_t)(Delay_GetTick() - next_tick) < 0)
        {
            /* Wait */
        }

        /* Doc encoder: delta cua chu ky vua qua */
        Encoder_Update();

        /* Thoi diem cuoi chu ky */
        log_time_ms += ENCODER_SAMPLE_TIME_MS;

        /* Save data into RAM */
        DataLogger_Record(
            log_time_ms,
            duty,
            Encoder_Delta,
            Encoder_Speed_RPM
        );

        /* Stop if buffer is full */
        if (Motor_Log_Finished)
        {
            Motor_Stop();
            return;
        }
    }
}


/* =====================================
   MAIN
   ===================================== */

int main(void)
{
    Delay_Init();

    Motor_Init();

    Encoder_Init();

    DataLogger_Init();

    /* Bat dau lich lay mau tu bay gio */
    next_tick = Delay_GetTick();

    /* Initial settling (0%) */
    Motor_Run_Log(0, 2000);

    /* 30% PWM */
    Motor_Run_Log(30, 6000);

    /* Stop */
    Motor_Run_Log(0, 4000);

    /* 50% PWM */
    Motor_Run_Log(50, 6000);

    /* Stop */
    Motor_Run_Log(0, 4000);

    /* 70% PWM */
    Motor_Run_Log(70, 6000);

    /* Stop */
    Motor_Run_Log(0, 5000);

    /* Finish experiment */
    Motor_Stop();

    Motor_Log_Finished = 1;

    /* Giu du lieu trong RAM.
       Bam STOP trong debugger roi go: SaveLog()  */
    while (1)
    {
        __NOP();
    }
}
