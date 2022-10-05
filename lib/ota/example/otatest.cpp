#define LOG_ENABLED
#include <Arduino.h>
#include <derfunke.h>
#include <easywifi.h>
#include <ota.h>

// i like to put these two in another file, so that it's 
// not checked in git commits
#define SSID "some_ssid"
#define PASSWD "some_password"

void setup() {
  Serial.begin(115200);
  while(!Serial) ;

  // welcome serial output header
  df_header_print();

  // we need to be connected to wifi for OTA to 
  // initialize properly
  if( wifi_connect_as_client(SSID, PASSWD) ) {
    wifi_print_mode();
    wifi_print_ip();
  } else {
    Serial.print("Failed to connect to wifi ");
    Serial.print( SSID );
    Serial.println();
  }

  // this is the name the device will take as an OTA port
  ota_init("racoon");
}

void loop() {
  ota_loop();

  delay(100); // chill for a bit
}
