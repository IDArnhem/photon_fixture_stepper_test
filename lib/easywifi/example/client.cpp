#include <Arduino.h>
#include <easywifi.h>

#define WIFI_SSID "somessid"
#define WIFI_PASSWORD "somepassword"

void setup() {
  Serial.begin( 9600 );
  while(!Serial); // wait until the serial connection is stablished

  // connect to existing access point as client
  if( wifi_connect_as_client(WIFI_SSID, WIFI_PASSWORD) ) {
    // print debugging information
    wifi_print_mode();
    wifi_print_ip();
  } else {
    Serial.print("Failed to connect to wifi ");
    Serial.print( WIFI_SSID );
    Serial.println();
  }
}

void loop() {
}
