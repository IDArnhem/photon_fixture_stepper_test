#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AccelStepper.h>
#include <OSCMessage.h>
#include <easywifi/easywifi.h>
#include <config.h> // credentials and pin layouts
#include <datstepper.h> // our own stepper library, ripped from the internet

// see config.h for WiFi credentials

AccelStepper stepper(1, STEP_PIN, DIR_PIN); // not used yet

void wifi_init() {
  // connect to existing wifi access point as client
  if( wifi_connect_as_client(WIFI_SSID, WIFI_PASSWORD) ) {
    // print debugging information
    wifi_print_mode();
    wifi_print_ip(); // print our known ip address
  } else {
    Serial.print("Failed to connect to wifi ");
    Serial.print( WIFI_SSID );
    Serial.println();
  }
}


void setup()
{ 
  Serial.begin(115200);

    stepper.setSpeed(0);

  wifi_init();
//   stepper_init();
}

// things that we can do to the stepper:
// - move to position
// - spin continuously
// - change speed
// - change acceleration
// - set direction  L | R

void on_moveto(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}

void on_spin(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}

void on_stop(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}

void on_set_accel(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}

void on_set_speed(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}

void on_set_dir(OSCMessage &msg, int addrOffset) {
  // here is where I have to put the code, to move the stepper to the given position
}


void osc_message_pump() {
  OSCMessage in;
  int size;

  if( (size = Udp.parsePacket()) > 0)
  {
    Serial.println("processing OSC package");
    // parse incoming OSC message
    while(size--) {
      in.fill( Udp.read() );
    }

    if(!in.hasError()) {
      // check the address of the message to perform the action
      // address map:
      //  - /moveto <position>
      //  - /spin <speed> <direction>
      //  - /set_accel <accel>
      //  - /set_speed <speed>
      //  - /set_dir <direction>
      //  - /stop
      in.route("/moveto", on_moveto);
      in.route("/spin", on_spin);
      in.route("/stop", on_stop);
      in.route("/set_accel", on_set_accel);
      in.route("/set_speed", on_set_speed);
      in.route("/set_dir", on_set_dir);
    }
  } // if
}

void loop()
{
  osc_message_pump();
  
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
