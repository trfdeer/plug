#include "DNSServer.h"
#include "routes.h"
#include "utils.h"

#include "ESP8266WebServer.h"
#include "ESP8266WiFi.h"

#include <Arduino.h>

IPAddress ip(192, 168, 137, 1), mask(255, 255, 255, 0);

DNSServer dnsServer;
ESP8266WebServer webServer;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, mask);
  WiFi.encryptionType(AUTH_WPA2_PSK);
  WiFi.softAP("Plug WiFi", "rx6900xt");

  dnsServer.start(53, "*", ip);

  webServer.on("/", routes::home);
  webServer.on("/on", routes::plug_on);
  webServer.on("/off", routes::plug_off);
  webServer.on("/toggle", routes::plug_toggle);
  webServer.on("/status", routes::plug_status);
  webServer.onNotFound(routes::not_found);

  webServer.begin(80);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}