#include <Arduino.h>
#include <smoothie.h>

Smoothie<int> reading(0.1);

void setup() {

  Serial.begin( 9600 );
  while(!Serial);

  pinMode(A2, INPUT);
}

void loop() {
  int noisy = analogRead( A2 );
  int avg = analogReadAvg( A2 );
  int val = reading.filter( analogRead( A2 ) );

  Serial.print( noisy );
  Serial.print(", ");
  Serial.print( avg );
  Serial.print(", ");
  Serial.print( val );
  Serial.println();
}