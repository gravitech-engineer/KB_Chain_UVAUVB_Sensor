/*
 * KBChain_UVAUVB_Sensor.cpp
 *
 * Arduino library for the Vishay KBChain_UVAUVB_Sensor UVA/UVB i2c sensor.
 *
 *
 */

#include "KBChain_UVAUVB_Sensor.h"

KBChain_UVAUVB_Sensor::KBChain_UVAUVB_Sensor() {

  // Despite the datasheet saying this isn't the default on startup, it appears
  // like it is. So tell the thing to actually start gathering data.
  this->config = 0;
  this->config |= CONF_SD_OFF;

  // App note only provided math for this one...
  this->config |= CONF_IT_100MS;
}

bool KBChain_UVAUVB_Sensor::begin() {

  Wire.begin();
  if (this->getDevID() != Sensor_DEVID) {
    return false;
  }

  // Write config to make sure device is enabled
  this->write16(REG_CONF, this->config);

  return true;
}

// Poll sensor for latest values and cache them
void KBChain_UVAUVB_Sensor::poll() {
  this->raw_uva = this->read16(REG_UVA);
  this->raw_uvb = this->read16(REG_UVB);
  this->raw_dark = this->read16(REG_DUMMY);
  this->raw_vis = this->read16(REG_UVCOMP1);
  this->raw_ir = this->read16(REG_UVCOMP2);
}

uint16_t KBChain_UVAUVB_Sensor::getRawUVA() {
  return this->raw_uva;
}

uint16_t KBChain_UVAUVB_Sensor::getRawUVB() {
  return this->raw_uvb;
}

uint16_t KBChain_UVAUVB_Sensor::getRawDark() {
  return this->raw_dark;
}

uint16_t KBChain_UVAUVB_Sensor::getRawVisComp() {
  return this->raw_vis;
}

uint16_t KBChain_UVAUVB_Sensor::getRawIRComp() {
  return this->raw_ir;
}


uint16_t KBChain_UVAUVB_Sensor::getDevID() {
  return this->read16(REG_DEVID);
}

float KBChain_UVAUVB_Sensor::getUVA() {
  float comp_vis = this->raw_vis - this->raw_dark;
  float comp_ir = this->raw_ir - this->raw_dark;
  float comp_uva = this->raw_uva - this->raw_dark;

  comp_uva -= (UVI_UVA_VIS_COEFF * comp_vis) - (UVI_UVA_IR_COEFF * comp_ir);

  return comp_uva;
}

float KBChain_UVAUVB_Sensor::getUVB() {
  float comp_vis = this->raw_vis - this->raw_dark;
  float comp_ir = this->raw_ir - this->raw_dark;
  float comp_uvb = this->raw_uvb - this->raw_dark;

  comp_uvb -= (UVI_UVB_VIS_COEFF * comp_vis) - (UVI_UVB_IR_COEFF * comp_ir);

  return comp_uvb;
}

float KBChain_UVAUVB_Sensor::getUVIndex() {
  float uva_weighted = this->getUVA() * UVI_UVA_RESPONSE;
  float uvb_weighted = this->getUVB() * UVI_UVB_RESPONSE;
  return (uva_weighted + uvb_weighted) / 2.0;
}

uint16_t KBChain_UVAUVB_Sensor::read16(uint8_t reg) {
  uint8_t msb = 0;
  uint8_t lsb = 0;

  Wire.beginTransmission(Sensor_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(Sensor_ADDR, 2, true);
  lsb = Wire.read();
  msb = Wire.read();

  return (msb << 8) | lsb;
}

void KBChain_UVAUVB_Sensor::write16(uint8_t reg, uint16_t data) {
  Wire.beginTransmission(Sensor_ADDR);
  Wire.write(reg);
  Wire.write((uint8_t)(0xFF & (data >> 0))); // LSB
  Wire.write((uint8_t)(0xFF & (data >> 8))); // MSB
  Wire.endTransmission();
}
