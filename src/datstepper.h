// constants used in the datstepper library

#define DISABLE     HIGH
#define ENABLE      LOW
#define int_period 500L // [µsec] time to next interrupt
#define m_step_count_down_time 4 // m_step_count_down_time*int_period[msec] count down timer to disable motor driver minimum time the stepper needs to perform one step keep it low to save current
#define LEFT LOW
#define RIGHT HIGH

void stepper_init();
void stepper_drive(int d, int s /* ??? */);
void stepper_stop();
