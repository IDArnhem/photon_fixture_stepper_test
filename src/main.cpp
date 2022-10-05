#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <AccelStepper.h>
#include <OSCMessage.h>
#include <easywifi.h>
#include <config.h> // credentials and pin layouts
#include <datstepper.h> // our own stepper library, ripped from the internet

// see config.h for WiFi credentials
WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress dest(192, 168, 8, 100);
const unsigned int rxport = 54321;        // remote port to receive OSC
const unsigned int txport = 12345;        // local port to listen for OSC packets (actually not used for sending)


AccelStepper stepper(1, STEP_PIN, DIR_PIN); // not used yet

void wifi_connect() {
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

  wifi_connect();

  // listen for incoming OSC traffic
  Udp.begin(rxport);

  Serial.println("Starting UDP");
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

//   stepper_init();
}

// things that we can do to the stepper:
// - move to <position>
// - spin continuously in <direction> at a <speed>
// - change speed
// - change acceleration
// - set direction  <L | R>

void on_moveto(OSCMessage &msg, int addrOffset) {
  int pos;

  if( msg.isInt(0) ) {
    pos = msg.getInt(0);
  } else {
    pos = floor(msg.getFloat(0));
  }

  // @TODO here is where I have to put the code, to move the stepper to the given position
}

//  /spin <speed> <direction>
void on_spin(OSCMessage &msg, int addrOffset) {
  int speed, direction;
  
  if( msg.isInt(0) && msg.isInt(1) ) {
    speed = msg.getInt(0);
    direction = msg.getInt(1);
  } else {
    speed = floor(msg.getFloat(0));
    direction = floor(msg.getFloat(1));
  }

  // @TODO now here goes the code to spin the motor in the given speed and direction
}

void on_stop(OSCMessage &msg, int addrOffset) {
  // @TODO here is where I have to put the code, to move the stepper to the given position
}

void on_set_accel(OSCMessage &msg, int addrOffset) {
  // @TODO here is where I have to put the code, to move the stepper to the given position
}

void on_set_speed(OSCMessage &msg, int addrOffset) {
  // @TODO here is where I have to put the code, to move the stepper to the given position
}

void on_set_dir(OSCMessage &msg, int addrOffset) {
  // @TODO here is where I have to put the code, to move the stepper to the given position
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
  // osc_message_pump();

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
