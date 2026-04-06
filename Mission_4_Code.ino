#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <SoftwareSerial.h>

// =========================
// FireSat Mission Code
// STEMSat Educational CubeSat Program
// =========================

// -------------------------
// Bluetooth setup
// D10 = Arduino RX (connect to HC-05 TXD)
// D11 = Arduino TX (connect to HC-05 RXD)
// -------------------------
SoftwareSerial bt(10, 11);

// -------------------------
// Sensor setup
// -------------------------
Adafruit_BME680 bme;   // I2C BME688
const int LIGHT_PIN = A0;

// Turn Bluetooth on/off here
const bool USE_BLUETOOTH = true;

// -------------------------
// Timing
// -------------------------
unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 1000;  // ms

// -------------------------
// Thresholds
// Adjust these after testing
// -------------------------
const float TEMP_WARNING = 30.0;
const float TEMP_HAZARD  = 34.0;

const int LIGHT_WARNING = 500;
const int LIGHT_HAZARD  = 800;

// -------------------------
// Helper print functions
// -------------------------
void printBoth(const String &s) {
  Serial.print(s);
  if (USE_BLUETOOTH) {
    bt.print(s);
  }
}

void printlnBoth(const String &s) {
  Serial.println(s);
  if (USE_BLUETOOTH) {
    bt.println(s);
  }
}

// -------------------------
// Determine FireSat status
// -------------------------
String getHazardStatus(float tempC, int lightValue) {
  if (tempC >= TEMP_HAZARD && lightValue >= LIGHT_HAZARD) {
    return "HAZARD";
  }
  else if (tempC >= TEMP_WARNING || lightValue >= LIGHT_WARNING) {
    return "WARNING";
  }
  else {
    return "NORMAL";
  }
}

void setup() {
  Serial.begin(9600);

  if (USE_BLUETOOTH) {
    bt.begin(9600);
  }

  delay(1000);

  printlnBoth("FireSat CubeSat Mission Starting...");
  printlnBoth("Initializing sensors...");

  // Initialize BME688
  if (!bme.begin()) {
    printlnBoth("ERROR: Could not find BME688 sensor. Check wiring.");
    while (1);
  }

  // Recommended settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setGasHeater(320, 150);

  printlnBoth("Sensors initialized successfully.");
  printlnBoth("Telemetry Format:");
  printlnBoth("Temp:<value C> | Light:<value> | Status:<state>");
  printlnBoth("------------------------------------------------");
}

void loop() {
  // Read BME688
  if (!bme.performReading()) {
    printlnBoth("ERROR: Failed to read BME688 sensor.");
    delay(1000);
    return;
  }

  float temperatureC = bme.temperature;
  int lightValue = analogRead(LIGHT_PIN);

  String status = getHazardStatus(temperatureC, lightValue);

  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis();

    String dataLine = "Temp:" + String(temperatureC, 1) +
                      " C | Light:" + String(lightValue) +
                      " | Status:" + status;

    printlnBoth(dataLine);
  }
}