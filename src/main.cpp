#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "secrets.h"

// Debug logging over telnet. Just run:
// telnet garage-door.local
#if LOGGING
#include <RemoteDebug.h>
#define DLOG(msg, ...) if(Debug.isActive(Debug.DEBUG)){Debug.printf(msg, ##__VA_ARGS__);}
RemoteDebug Debug;
#else
#define DLOG(msg, ...)
#endif

// Network setup
#define HOSTNAME "fan"
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// MQTT Setup
const PROGMEM char *eventTopic = "fan/power";

// Define the pins
#define PWM_PIN D1

const char compile_date[] = __DATE__ " " __TIME__;

void callback(char *topic, byte *payload, unsigned int length) {
  DLOG("Received callback for topic %s\n", topic);
  // If the 'fan/power' topic has a payload > 0, activate the fan
  if (strcmp(eventTopic, topic) == 0) {
    char payloadString[10];
    byte* p = (byte*)malloc(length + 1);
    // Copy the payload to the new buffer (since payload is not null terminated)
    memcpy(p, payload, length);
    p[length] = 0;
    String inString((char *)p);
    int value = inString.toInt();
    int power = min(max((value*PWMRANGE)/100, 0), PWMRANGE);
    DLOG("Setting fan power to %d\n", power);
    analogWrite(PWM_PIN, power);
  }
}

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0);
  analogWriteFreq(20000);

  Serial.begin(115200);
  Serial.println("Built on");
  Serial.println(compile_date);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiSsid);

  // Set Hostname.
  String hostname(HOSTNAME);
  WiFi.hostname(hostname);
  WiFi.begin(wifiSsid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();

  #if LOGGING
  Debug.begin((const char *)hostname.c_str());
  Debug.setResetCmdEnabled(true);
  #endif
}

void reconnect() {
  DLOG("Attempting MQTT connection...\n");
  // Attempt to connect
  if (client.connect(HOSTNAME, mqttUser, mqttPassword)) {
    DLOG("MQTT connected\n");
    client.subscribe(eventTopic);
  } else {
    DLOG("MQTT failed rc=%d try again in 5 seconds\n", client.state());
  }
}

void loop() {
  // If MQTT client can't connect to broker, then reconnect
  if (!client.connected()) {
    reconnect();
  }
  ArduinoOTA.handle();
  yield();
  client.loop();
  Debug.handle();
}
