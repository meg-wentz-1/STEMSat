#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <SoftwareSerial.h>

#define SEALEVELPRESSURE_HPA 1013.25

Adafruit_BME680 bme;  // I2C

// HC-05 on SoftwareSerial (D10 = RX, D11 = TX)
SoftwareSerial bt(10, 11);

// Analog pins for ADXL335
#define X_PIN A0
#define Y_PIN A1
#define Z_PIN A2

// Helper: print to BOTH USB Serial and Bluetooth
void printBoth(const String &s) {
  Serial.print(s);
  bt.print(s);
}
void printlnBoth(const String &s) {
  Serial.println(s);
  bt.println(s);
}

void setup() {
  Serial.begin(9600);
  bt.begin(9600);   // HC-05 data mode is usually 9600

  // (Optional) don't block forever if USB serial isn't opened
  // while (!Serial) { }  // <- remove/avoid for wireless-only operation

  printlnBoth("BME688 + ADXL335 Test Starting...");

  if (!bme.begin()) {
    printlnBoth("Could not find BME688 sensor! Check wiring.");
    while (1) { delay(10); }
  }

  // Configure BME688
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setGasHeater(320, 150);

  printlnBoth("Sensors initialized successfully!");
  printlnBoth("-----------------------------");
}

void loop() {
  // ----- BME688 -----
  if (!bme.performReading()) {
    printlnBoth("Failed to perform BME reading.");
    delay(500);
    return;
  }

  printBoth("Temperature = ");
  printBoth(String(bme.temperature, 2));
  printlnBoth(" °C");

  printBoth("Humidity = ");
  printBoth(String(bme.humidity, 2));
  printlnBoth(" %");

  printBoth("Pressure = ");
  printBoth(String(bme.pressure / 100.0, 2));
  printlnBoth(" hPa");

  printBoth("Gas Resistance = ");
  printBoth(String(bme.gas_resistance / 1000.0, 2));
  printlnBoth(" KOhms");

  // ----- ADXL335 -----
  int xRaw = analogRead(X_PIN);
  int yRaw = analogRead(Y_PIN);
  int zRaw = analogRead(Z_PIN);

  printBoth("Accel X: ");
  printBoth(String(xRaw));
  printBoth("  Y: ");
  printBoth(String(yRaw));
  printBoth("  Z: ");
  printlnBoth(String(zRaw));

  printlnBoth("-----------------------------");

  delay(2000);
}