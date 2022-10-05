#pragma once

#include <ArduinoOTA.h>
#include <Update.h>

void ota_init(const char *name);
void ota_loop();
