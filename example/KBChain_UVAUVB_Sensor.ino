#include <KBChain_UVAUVB_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

KBChain_UVAUVB_Sensor veml6075 = KBChain_UVAUVB_Sensor();
bool found = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(4,5);
  if (!veml6075.begin()) {
  } else {
    found = true;
  }
}

void loop() {

  if (found) {
    float value;

    // Poll sensor
    veml6075.poll();

    Serial.print(F("t = "));
    Serial.println(millis());

    value = veml6075.getUVA();
    Serial.print(F("UVA = "));
    Serial.println(value, 2);

    value = veml6075.getUVB();
    Serial.print(F("UVB = "));
    Serial.println(value, 2);

    value = veml6075.getUVIndex();
    Serial.print(F("UV Index = "));
    Serial.println(value, 1);

    uint16_t devid = veml6075.getDevID();
    Serial.print(F("Device ID = "));
    Serial.println(devid, HEX);

    Serial.println(F("----------------"));
  }

  delay(1000);
}
