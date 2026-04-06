#include <SoftwareSerial.h>

// =========================
// PowerSat Mission Code
// STEMSat Educational CubeSat Program
// =========================

// Bluetooth setup
SoftwareSerial bt(10, 11);

// Light sensor pin
const int LIGHT_PIN = A0;

// Toggle Bluetooth
const bool USE_BLUETOOTH = true;

// Thresholds (adjust after testing)
const int LIGHT_ON_THRESHOLD  = 600;
const int LIGHT_DIM_THRESHOLD = 300;

// Timing
unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 1000;  // ms

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
// Determine system state
// -------------------------
String getSystemState(int lightValue) {
  if (lightValue >= LIGHT_ON_THRESHOLD) {
    return "ON";
  } else if (lightValue >= LIGHT_DIM_THRESHOLD) {
    return "DIM";
  } else {
    return "OFF";
  }
}

void setup() {
  Serial.begin(9600);

  if (USE_BLUETOOTH) {
    bt.begin(9600);
  }

  delay(1000);

  printlnBoth("PowerSat CubeSat Mission Starting...");
  printlnBoth("Telemetry Format:");
  printlnBoth("Light:<value> | System:<state>");
  printlnBoth("--------------------------------------");
}

void loop() {
  // Read light level
  int lightValue = analogRead(LIGHT_PIN);

  // Determine system state
  String systemState = getSystemState(lightValue);

  // Print telemetry at interval
  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis();

    String dataLine = "Light:" + String(lightValue) +
                      " | System:" + systemState;

    printlnBoth(dataLine);
  }
}