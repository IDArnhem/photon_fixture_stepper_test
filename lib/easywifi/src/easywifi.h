#ifndef __EASYWIFI_H__
#define __EASYWIFI_H__

#include <nanolog.h>

#ifndef __NANOLOG_H__
#warning "the easywifi library needs the nanolog library as well"
#endif

enum WifiMode {
    CLIENT = 10,        // station node
    HYBRID_AP = 15,     // station + ap
    ACCESS_POINT = 20,  // ap only
    MESH = 30           // mesh
};

struct tWifiSettings {
  IPAddress thisip;
  WifiMode mode;
  String apssid;
  String ssid, password;
};

extern tWifiSettings wifi;

void wifi_init();
void wifi_set_credentials(const char *ssid, const char *passw);
void wifi_set_default_ap_name(const char *ssid);
void wifi_print_ip();
void wifi_print_mode();
bool wifi_connect_as_client(const char *ssid, const char *passw, int timeouts = 10);
bool wifi_create_ap(const char *myssid);

#endif // __EASYWIFI_H__