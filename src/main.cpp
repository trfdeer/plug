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

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, mask);
  WiFi.softAP("Plug WiFi");

  dnsServer.start(53, "*", ip);
  digitalWrite(LED_BUILTIN, HIGH);

  webServer.on("/", routes::home);
  webServer.on("/on", routes::plug_on);
  webServer.on("/off", routes::plug_off);
  webServer.on("/toggle", routes::plug_toggle);
  webServer.on("/status", routes::plug_status);
  webServer.onNotFound(routes::not_found);
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}