#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <AccelStepper.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
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
  Serial.print("Listening on port: ");
  Serial.println(Udp.localPort());

//   stepper_init();
}

// things that we can do to the stepper:
// - move to <position>
// - spin continuously in <direction> at a <speed>
// - change speed
// - change acceleration
// - set direction  <L | R>

void on_moveto(OSCMessage &msg) {
  int pos;
  Serial.println("<-- moveto received");

  if( msg.isInt(0) ) {
    pos = msg.getInt(0);
  } else {
    pos = floor(msg.getFloat(0));
  }


  // @TODO here is where I have to put the code, to move the stepper to the given position
  stepper.moveTo(pos);
}

//  /spin <speed> <direction>
void on_spin(OSCMessage &msg) {
  int speed, direction;
  Serial.println("<-- spin received");
    
  if( msg.isInt(0) && msg.isInt(1) ) {
    speed = msg.getInt(0);
    direction = msg.getInt(1);
  } else {
    speed = floor(msg.getFloat(0));
    direction = floor(msg.getFloat(1));
  }

  // @TODO now here goes the code to spin the motor in the given speed and direction
  stepper.setSpeed(speed);
  //direction depends on wheter the value is under/above 0 
}

void on_stop(OSCMessage &msg) {

  // @TODO here is where I have to put the code, to move the stepper to the given position
  stepper.setMaxSpeed(0);
  Serial.println("<-- stop received");
}

void on_set_accel(OSCMessage &msg) {
  int setAcc;
  Serial.println("<-- set_accel received");

  if( msg.isInt(0) ) {
    setAcc = msg.getInt(0);
  } else {
    setAcc = floor(msg.getFloat(0));
  }
  // @TODO here is where I have to put the code, to move the stepper to the given position
  stepper.setAcceleration(setAcc);
}

void on_set_speed(OSCMessage &msg) {
  int setSpeed;
  Serial.println("<-- set_speed received");

  if( msg.isInt(0) ) {
    setSpeed = msg.getInt(0);
  } else {
    setSpeed = floor(msg.getFloat(0));
  }
  
  // @TODO here is where I have to put the code, to move the stepper to the given position
  stepper.setMaxSpeed(setSpeed);
}

void on_set_dir(OSCMessage &msg) {
  int dir;
  Serial.println("<-- set_dir received");

  if( msg.isInt(0) ) {
    dir = msg.getInt(0);
  } else {
    dir = floor(msg.getFloat(0));
  }
  // @TODO here is where I have to put the code, to move the stepper to the given position
  //what do we need this for? direction is alresday set in continious movement
}


void osc_message_pump() {
  OSCBundle bundle;
  int size;
  char addr[80];

  if( (size = Udp.parsePacket()) > 0)
  {
    //Serial.println("(.) processing OSC package");
    // parse incoming OSC message
    while(size--) {
      bundle.fill( Udp.read() );
    }
  
    if(!bundle.hasError()) {
        // bundle.getAddress(addr);
        // Serial.print("[!!] osc address -> ");
        // Serial.println( String(addr) );

      // check the address of the message to perform the action
      // address map:
      //  - /moveto <position>
      //  - /spin <speed> <direction>
      //  - /set_accel <accel>
      //  - /set_speed <speed>
      //  - /set_dir <direction>
      //  - /stop
      bundle.dispatch("/moveto", on_moveto);
      bundle.dispatch("/spin", on_spin);
      bundle.dispatch("/stop", on_stop);
      bundle.dispatch("/set_accel", on_set_accel);
      bundle.dispatch("/set_speed", on_set_speed);
      bundle.dispatch("/set_dir", on_set_dir);
    } else {
      OSCErrorCode error = bundle.getError();
      Serial.print("[!!] osc err -> ");
      Serial.println( error );
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
    stepper.run();
}
