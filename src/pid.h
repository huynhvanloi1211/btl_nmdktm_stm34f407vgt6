#ifndef PID_H
#define PID_H

#include <stdint.h>

/* --- MACROS C?U HÌNH --- */
#define PID_MAX_OUT       1000.0f  // Giá tr? PWM t?i da (Ví d?: ARR c?a Timer)
#define PID_MIN_OUT       -1000.0f // Giá tr? PWM t?i thi?u (Quay ngu?c)
#define PID_MAX_INT       800.0f   // Gi?i h?n khâu Tích phân (Anti-windup clamp)
#define PID_MIN_INT       -800.0f  // Gi?i h?n khâu Tích phân (Anti-windup clamp)

/* --- C?U TRÚC D? LI?U PID --- */
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float T;          // Th?i gian l?y m?u (Sample time) tính b?ng giây (Ví d?: 0.01s = 10ms)
    
    // Bi?n tr?ng thái luu gi?a các chu k?
    float integrator;
    float prevError;
    
    // Ngõ ra
    float out;
} PID_Controller;

/* --- PROTOTYPE HÀM --- */
void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float T);
float PID_Update(PID_Controller *pid, float setpoint, float measurement);
void PID_Reset(PID_Controller *pid);

#endif /* PID_H */
