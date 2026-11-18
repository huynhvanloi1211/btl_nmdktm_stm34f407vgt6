#include "pid.h"

void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float T) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->T  = T;
    
    PID_Reset(pid);
}

void PID_Reset(PID_Controller *pid) {
    pid->integrator = 0.0f;
    pid->prevError  = 0.0f;
    pid->out        = 0.0f;
}

float PID_Update(PID_Controller *pid, float setpoint, float measurement) {
    // Khai báo bi?n ? d?u hàm (Tuân th? chu?n C90 c?a Keil C)
    float error;
    float P_out;
    float D_out;
    
    // 1. Tính sai s?
    error = setpoint - measurement;
    
    // 2. Khâu T? l? (Proportional)
    P_out = pid->Kp * error;
    
    // 3. Khâu Tích phân (Integral) kèm Anti-windup (Clamping)
    pid->integrator += 0.5f * pid->Ki * pid->T * (error + pid->prevError);
    if (pid->integrator > PID_MAX_INT) {
        pid->integrator = PID_MAX_INT;
    } else if (pid->integrator < PID_MIN_INT) {
        pid->integrator = PID_MIN_INT;
    }
    
    // 4. Khâu Ğ?o hàm (Derivative)
    D_out = pid->Kd * (error - pid->prevError) / pid->T;
    
    // 5. T?ng h?p ngõ ra và gi?i h?n (Clamp Output)
    pid->out = P_out + pid->integrator + D_out;
    if (pid->out > PID_MAX_OUT) {
        pid->out = PID_MAX_OUT;
    } else if (pid->out < PID_MIN_OUT) {
        pid->out = PID_MIN_OUT;
    }
    
    // 6. Luu tr?ng thái cho chu k? ti?p theo
    pid->prevError = error;
    
    return pid->out;
}
