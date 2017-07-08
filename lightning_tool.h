
#include <Wire.h>
#include <AS3935.h>

#define IRQ_PIN 2
//#define CS_PIN 10

volatile bool detected = false;

void printDistance() {
  int distance = mod1016.calculateDistance();
  
  if (distance == -1){
    DebugPrintln("Lightning out of range");
  }
  else if (distance == 1){
    DebugPrintln("Distance not in table");
  }
  else if (distance == 0){
    DebugPrintln("Lightning overhead");
  }
  else {
    DebugPrint("Lightning ~");
    DebugPrint(distance);
    DebugPrintln("km away\n");
  }
}

void translateIRQ(uns8 irq) {
  switch (irq) {
    case 1:
      DebugPrintln("NOISE DETECTED");
      break;
    case 4:
      DebugPrintln("DISTURBER DETECTED");
      break;
    case 8:
      DebugPrintln("LIGHTNING DETECTED");
      printDistance();
      break;
  }
}


void alert() {
  detected = true;
}

void init_lightning() {


  //I2C
  Wire.begin();
  mod1016.init(IRQ_PIN);

  //Tune Caps, Set AFE, Set Noise Floor
  autoTuneCaps(IRQ_PIN);

  //mod1016.setTuneCaps(6);
  mod1016.setOutdoors();
  mod1016.setNoiseFloor(5);

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

