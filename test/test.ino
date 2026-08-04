#include <IRremote.hpp>
#define receiver 13

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(receiver, ENABLE_LED_FEEDBACK);
  Serial.println("---Let's begin---");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.address);
    IrReceiver.resume();
  }
}
