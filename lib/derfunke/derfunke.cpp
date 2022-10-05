#include <Arduino.h>
#include "derfunke.h"

void df_header_print() {
    Serial.println("     __        ___          __      ");
    Serial.println(" ___/ /__ ____/ _/_ _____  / /_____ ");
    Serial.println("/ _  / -_) __/ _/ // / _ \\/  '_/ -_)");
    Serial.println("\\_,_/\\__/_/ /_/ \\_,_/_//_/_/\\_\\__/ ");
    Serial.print  (" M  C  U  K  I  T               ");
    Serial.println(MCUKIT_RELEASE);
    Serial.println();
}
