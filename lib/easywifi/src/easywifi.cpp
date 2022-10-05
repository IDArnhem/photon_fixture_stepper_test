#define LOG_ENABLED

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include <WiFiUdp.h>
#include "easywifi.h"
#include <nanolog.h>

// IPAddress thisip;
// WifiMode mode;
// String apssid = "default_ap";
// String _ssid = "unknown", _password = "unknown";

tWifiSettings wifi;

// default network settings when operating as an AP
IPAddress ip_static = IPAddress(10, 0, 0, 1);
IPAddress gw_static = IPAddress(10, 0, 0, 1);
IPAddress sn_static = IPAddress(255, 255, 255, 0);

void wifi_set_credentials(const char *ssid, const char *passw) {
  wifi.ssid = ssid;
  wifi.password = passw;
}

void wifi_set_default_ap_name(const char *name) {
  wifi.apssid = name;
}

/**
 * connect to WiFi access point as client station
 * 
 * @param ssid to connect to
 * @param passw password
 * @param timeout is in seconds
 */
bool wifi_connect_as_client(const char *ssid, const char *passw, int timeouts) {
  boolean succeeded = true;

  wifi_set_credentials(ssid, passw);

  WiFi.mode(WIFI_STA);  // both hotspot and client are enabled
  WiFi.begin(ssid, passw);
  LOG("Connecting to WiFi ");
  LOG(ssid);
  LOG_NEW_LINE

  // wait for connection
  LOG("Connecting");
  unsigned long tout = millis() + (timeouts * 1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (millis() > tout) {
      succeeded = false;
      break;
    }
  }
  LOG_NEW_LINE

  // if we are connected we probably were assigned an IP
  if (succeeded) {
    wifi.thisip = WiFi.localIP();
  }

  wifi.mode = WifiMode::CLIENT;

  return succeeded;
}

bool wifi_create_ap(const char *myssid) {
  WiFi.mode(WIFI_AP);

  // optional soft ip config
  LOG("Custom AP IP/GW/Subnet");
  LOG_NEW_LINE
  WiFi.softAPConfig(ip_static, gw_static, sn_static);

  WiFi.softAP(myssid);
  LOG("Creating WiFi access point with name ");
  LOG(myssid);
  LOG_NEW_LINE

  // let a long delay happen here, otherwise we might not get an IP
  delay(1000);

  // // setup the DNS server redirecting all the domains to the ap IP
  // dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  // dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  wifi.thisip = WiFi.softAPIP();

  wifi.apssid = myssid;
  wifi.mode = WifiMode::ACCESS_POINT;

  return true;
}

void wifi_print_ip() {
    LOG("IP address: ");
    LOG( wifi.thisip );
}

void wifi_print_mode() {
  if( WifiMode::CLIENT == wifi.mode ) {
    LOG("Connected to ");
    LOG(wifi.ssid);
    LOG(" as client");
    LOG_NEW_LINE
  } else if (WifiMode::ACCESS_POINT == wifi.mode) {
    LOG("Created ");
    LOG(wifi.apssid);
    LOG(" access point");
    LOG_NEW_LINE
  }
}

void wifi_init()
{
  // first try to connect to a given WiFi AP
  if( wifi_connect_as_client( wifi.ssid.c_str(), wifi.password.c_str()) ) {
    // // save IP
    // persistence_save_settings();

    // // create mDNS entry
    // String mdns = "pyropanda" + thisip[3];
    // network_create_mdns_entry( mdns.c_str() );
  } else {
    // if we do not succeed at connecting to an existing AP, create our own
    // and setup DNS to channel all connections to the captive portal
    LOG("Connection failed.");
    LOG_NEW_LINE

    wifi_create_ap( wifi.apssid.c_str() );
  }

  // always print debug info
  wifi_print_mode();
  wifi_print_ip();
  LOG_NEW_LINE
} // wifi_init
