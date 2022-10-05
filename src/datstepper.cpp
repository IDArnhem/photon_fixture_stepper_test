#include <Arduino.h>
#include "datstepper.h"
#include "config.h"


//********************************************************
int     steps = 0;      // # of steps to be performed
int Periode   = 2000;   // [µsec] !!!  period time to next step
int Next_Step = Periode;// [µsec] !!!  count down timer current step
int m_step_count_down = 0;//[0,5msec]!!!  count down timer to disable motor driver
int RUN = HIGH ; //if set t0 HIGH the motor will run for ever
int dir = LEFT;


void timer0_ISR (void)
{
  timer0_write(ESP.getCycleCount() + int_period * 80 ); //40000L); // 80MHz == int_period[msec]
  if (m_step_count_down-- <= 1) digitalWrite(ENABLE_PIN, DISABLE); // disable motor driver when step is performed
  digitalWrite(DIR_PIN, dir);

  if (steps > 0) // are there steps to be performed ?
  {
    Next_Step -= int_period ; // [µs];
    if (Next_Step <= Periode) //[µsec]period time to next step
    {
      digitalWrite(ENABLE_PIN, ENABLE); // enable motor driver 
      Next_Step += Periode;  //    Next_Step = Next_Step + Periode;
      digitalWrite( STEP_PIN, LOW);
      steps--; // it will decrement two times when performing one single step
      m_step_count_down = m_step_count_down_time ; // m_step_count_down_time*int_period[msec] count down timer to disable motor driver
      digitalWrite( STEP_PIN, HIGH);// perform one step

    }
  }
}

void stepper_init() {
  pinMode (ENABLE_PIN , OUTPUT);
  digitalWrite(ENABLE_PIN, DISABLE); // disable motor driver_2
  pinMode (STEP_PIN , OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  pinMode (DIR_PIN , OUTPUT);
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timer0_ISR);
  timer0_write(ESP.getCycleCount() + 80000000L); // 80MHz == 1sec
  interrupts();
}

void stepper_drive(int d, int s /* ??? */) {
  dir = d;
  steps = s;
  RUN = HIGH;
}

void stepper_stop(){
  RUN = LOW;
}
