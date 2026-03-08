// sensor.cpp — updated for Adafruit_VL53L0X
#include "sensor.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_VL53L0X.h>  // Install: "Adafruit_VL53L0X" by Adafruit

static Adafruit_VL53L0X sensor;

bool initSensor() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  if (!sensor.begin()) {
    Serial.println("[SENSOR] ERROR: VL53L0X not found!");
    return false;
  }

  // High speed continuous mode — ~20ms per reading
  sensor.startRangeContinuous(20);
  Serial.println("[SENSOR] VL53L0X initialized OK (Adafruit library).");
  return true;
}

int readDistanceMM() {
  if (!sensor.isRangeComplete()) return -1;

  VL53L0X_RangingMeasurementData_t measure;
  sensor.getRangingMeasurement(&measure, false);

  // RangeStatus 4 = phase failure (no object / out of range)
  if (measure.RangeStatus == 4) return -1;

  return (int)measure.RangeMilliMeter;
}
