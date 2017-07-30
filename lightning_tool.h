
#include <Wire.h>
#include <AS3935.h>

#define IRQ_PIN 14 //2
//#define CS_PIN 10

const int sclPin = D1;
const int sdaPin = D2;

volatile bool detected = false;

void printDistance() {
  int distance = mod1016.calculateDistance();

  if (distance == -1) {
    DebugPrintln("Lightning out of range");
  }
  else if (distance == 1) {
    DebugPrintln("Distance not in table");
  }
  else if (distance == 0) {
    DebugPrintln("Lightning overhead");
    send_val(21, distance);
  }
  else {
    DebugPrint("Lightning ~");
    DebugPrint(distance);
    DebugPrintln("km away\n");
    
    send_val(21, distance);
  }
}

void translateIRQ(uns8 irq) {
  switch (irq) {
    case 1:
      DebugPrintln("NOISE DETECTED");
      client.publish(mqtt_pubtopic, "NOISE");
      break;
    case 4:
      DebugPrintln("DISTURBER DETECTED");
      client.publish(mqtt_pubtopic, "DISTURBER");
      break;
    case 8:
      DebugPrintln("LIGHTNING DETECTED");
      client.publish(mqtt_pubtopic, "LIGHTNING");
      printDistance();
      break;
  }
}


void alert() {
  detected = true;
}

void init_lightning() {


  //I2C

  Wire.begin(sdaPin, sclPin); // Wire.begin();
  mod1016.init(IRQ_PIN);

  //Tune Caps, Set AFE, Set Noise Floor
  //autoTuneCaps(IRQ_PIN);
  delay(500);
  mod1016.setTuneCaps(6);
  mod1016.calibrateRCO();
  //mod1016.setOutdoors();
  mod1016.setIndoors();
  mod1016.setNoiseFloor(3);

  DebugPrintln("TUNE\tIN/OUT\tNOISEFLOOR");
  DebugPrint(mod1016.getTuneCaps(), HEX);
  DebugPrint("\t");
  DebugPrint(mod1016.getAFE(), BIN);
  DebugPrint("\t");
  DebugPrintln(mod1016.getNoiseFloor(), HEX);
  DebugPrint("\n");

  pinMode(IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), alert, RISING);
  mod1016.getIRQ();
  DebugPrintln("after interrupt");
}

void check_lightning() {
  if (detected) {
    translateIRQ(mod1016.getIRQ());
    detected = false;
  }
}

