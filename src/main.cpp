#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AccelStepper.h>

#include <config.h> // credentials and pin layouts
#include <datstepper.h> // our own stepper library, ripped from the internet

// see config.h for WiFi credentials

//AccelStepper stepper(1, D3, D2); // not used yet

void setup()
{ 
  Serial.begin(115200);
  stepper_init();
}

void loop()
{
  stepper_drive(LEFT, 20);
  stepper_stop();
  stepper_drive(RIGHT, 20);
  stepper_stop();
}
