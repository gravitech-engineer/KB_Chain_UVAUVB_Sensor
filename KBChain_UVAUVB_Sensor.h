/*
 * KBChain_UVAUVB_Sensor.h
 *
 * Arduino library for the Vishay KBChain_UVAUVB_Sensor  i2c sensor.
 *
 */

#ifndef KBChain_UVAUVB_Sensor_H
#define KBChain_UVAUVB_Sensor_H

#include <Arduino.h>
#include <Wire.h>

#define Sensor_ADDR  0x10
#define Sensor_DEVID 0x26

// Reading the application note on calculation of UV index, the "dummy" channel
// value is actually not a dummy value at all, but the dark current count.
// NAMES ARE IMPORTANT PEOPLE.

#define REG_CONF        (0x00) // Configuration register (options below)
#define REG_UVA         (0x07) // UVA register
#define REG_DUMMY       (0x08) // Dark current register (NOT DUMMY)
#define REG_UVB         (0x09) // UVB register
#define REG_UVCOMP1     (0x0A) // Visible compensation register
#define REG_UVCOMP2     (0x0B) // IR compensation register
#define REG_DEVID       (0x0C) // Device ID register

#define CONF_IT_50MS    (0x00) // Integration time = 50ms (default)
#define CONF_IT_100MS   (0x10) // Integration time = 100ms
#define CONF_IT_200MS   (0x20) // Integration time = 200ms
#define CONF_IT_400MS   (0x30) // Integration time = 400ms
#define CONF_IT_800MS   (0x40) // Integration time = 800ms
#define CONF_IT_MASK    (0x8F) // Mask off other config bits

#define CONF_HD_NORM    (0x00) // Normal dynamic seetting (default)
#define CONF_HD_HIGH    (0x08) // High dynamic seetting

#define CONF_TRIG       (0x04) // Trigger measurement, clears by itself

#define CONF_AF_OFF     (0x00) // Active force mode disabled (default)
#define CONF_AF_ON      (0x02) // Active force mode enabled (?)

#define CONF_SD_OFF     (0x00) // Power up
#define CONF_SD_ON      (0x01) // Power down

// To calculate the UV Index, a bunch of empirical/magical coefficients need to
// be applied to UVA and UVB readings to get a proper composite index value.
// Seems pretty hand wavey, though not nearly as annoying as the dark current
// not being subtracted out by default.

#define UVI_UVA_VIS_COEFF (3.33)
#define UVI_UVA_IR_COEFF  (2.5)
#define UVI_UVB_VIS_COEFF (3.66)
#define UVI_UVB_IR_COEFF  (2.75)

// Once the above offsets and crunching is done, there's a last weighting
// function to convert the ADC counts into the UV index values. This handles
// both the conversion into irradiance (W/m^2) and the skin erythema weighting
// by wavelength--UVB is way more dangerous than UVA, due to shorter
// wavelengths and thus more energy per photon. These values convert the compensated values 

#define UVI_UVA_RESPONSE (1.0 / 909.0)
#define UVI_UVB_RESPONSE (1.0 / 800.0)

enum int_time {
  IT_50MS,
  IT_100MS,
  IT_200MS,
  IT_400MS,
  IT_800MS
};
typedef enum int_time int_time_t;

class KBChain_UVAUVB_Sensor {

  public:

    KBChain_UVAUVB_Sensor();
    bool begin();

    void poll();
    float getUVA();
    float getUVB();
    float getUVIndex();
    uint16_t getDevID();

    uint16_t getRawUVA();
    uint16_t getRawUVB();
    uint16_t getRawDark();
    uint16_t getRawVisComp();
    uint16_t getRawIRComp();

    void setIntegrationTime(int_time_t it);

  private:

    uint8_t config;

    uint16_t raw_uva;
    uint16_t raw_uvb;
    uint16_t raw_dark;
    uint16_t raw_vis;
    uint16_t raw_ir;

    uint16_t read16(uint8_t reg);
    void write16(uint8_t reg, uint16_t data);

};

#endif
