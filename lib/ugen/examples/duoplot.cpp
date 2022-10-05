#include <ugen.h>

Ugen gen1;
Ugen gen2;


void setup()
{
  Serial.begin(115200);

  gen1.setFrequency(13);
  gen1.setAmplitude(50);
  gen1.setPhase(0);
  gen1.setYShift(0);

  gen2.setFrequency(17);
  gen2.setAmplitude(25);
  gen2.setPhase(0.25);
  gen2.setYShift(25);
}


void loop()
{
  float t = millis() * 0.001;
  float x = gen1.sinus(t);
  float y = gen2.sinus(t);
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(x + y);
  Serial.println();
}

