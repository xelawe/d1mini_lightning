
/*
   Blink and OTA
   Turns on the onboard LED on for one second, then off for one second, repeatedly.
   This uses delay() to pause between LED toggles.
*/
#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include "tools_wifi.h"
#include "ota_tool.h"
#include "mqtt_tool.h"
#include "lightning_tool.h"


void callback_mqtt(char* topic, byte* payload, unsigned int length) {

}

void setup() {
#ifdef serdebug
  Serial.begin(115200);
#endif

  wifi_init("D1miniLS");
  delay(500);

  init_ota("D1miniLS");

//  pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output

  init_mqtt(callback_mqtt);

  init_lightning();
}

void loop() {

  check_ota();

  check_lightning();

//  digitalWrite(BUILTIN_LED, HIGH);  // turn on LED with voltage HIGH
  delay(1000);                      // wait one second
//  digitalWrite(BUILTIN_LED, LOW);   // turn off LED with voltage LOW
//  delay(1000);                      // wait one second
}
