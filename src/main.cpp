#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AccelStepper.h>

#include <config.h> // credentials and pin layouts
#include <datstepper.h> // our own stepper library, ripped from the internet

// see config.h for WiFi credentials

AccelStepper stepper(1, STEP_PIN, DIR_PIN); // not used yet

void setup()
{ 
  Serial.begin(115200);

    stepper.setSpeed(0);

//   stepper_init();
}

void loop()
{
//   stepper_drive(LEFT, 20);
//   stepper_stop();
//   stepper_drive(RIGHT, 20);
//   stepper_stop();

// if (stepper.distanceToGo() == 0)
//     {
// 	// Random change to speed, position and acceleration
// 	// Make sure we dont get 0 speed or accelerations
// 	delay(1000);
// 	stepper.moveTo(rand() % 200);
// 	stepper.setMaxSpeed((rand() % 200) + 1);
// 	stepper.setAcceleration((rand() % 200) + 1);
//     }
    // stepper.run();


    stepper.runSpeed();
}
