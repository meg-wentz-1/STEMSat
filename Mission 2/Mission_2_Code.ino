#include <SoftwareSerial.h>

// -----------------------------
// MotionLab CubeSat Final Code
// Accelerometer + Piezo + HC-05
// Uses latched piezo impact detection
// -----------------------------

// Accelerometer pins
const int accelXPin = A0;
const int accelYPin = A1;
const int accelZPin = A2;

// Piezo pin
const int piezoPin = A3;

// HC-05 Bluetooth pins
SoftwareSerial bluetooth(10, 11);  // RX, TX

// Settings
const int impactThreshold = 10;            // start low, adjust later
const unsigned long printInterval = 1000;  // ms between output lines
const unsigned long impactCooldown = 300;  // ms between separate impacts

// Timing
unsigned long lastPrintTime = 0;
unsigned long lastImpactTime = 0;
unsigned long packetCount = 0;

// Piezo baseline
float piezoBaseline = 0;

// Latched impact value
int latchedImpact = 0;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Start baseline near current resting value
  piezoBaseline = analogRead(piezoPin);

  Serial.println("MotionLab CubeSat Final Code Starting...");
  Serial.println("Format: Packet | X | Y | Z | Impact");
  Serial.println("------------------------------------");

  bluetooth.println("MotionLab CubeSat Online");
  bluetooth.println("Format: Packet | X | Y | Z | Impact");
  bluetooth.println("------------------------------------");
}

void loop() {
  unsigned long currentTime = millis();

  // Read sensors
  int x = analogRead(accelXPin);
  int y = analogRead(accelYPin);
  int z = analogRead(accelZPin);
  int piezoRaw = analogRead(piezoPin);

  // Slowly update baseline
  piezoBaseline = 0.98 * piezoBaseline + 0.02 * piezoRaw;

  // Compute spike above baseline
  int piezoSpike = piezoRaw - (int)piezoBaseline;
  if (piezoSpike < 0) {
    piezoSpike = 0;
  }

  // Latch the largest impact seen since last print
  if (piezoSpike > impactThreshold && (currentTime - lastImpactTime >= impactCooldown)) {
    if (piezoSpike > latchedImpact) {
      latchedImpact = piezoSpike;
    }
    lastImpactTime = currentTime;
  }

  // Print once per interval
  if (currentTime - lastPrintTime >= printInterval) {
    lastPrintTime = currentTime;
    packetCount++;

    Serial.print("Packet ");
    Serial.print(packetCount);
    Serial.print(" | X:");
    Serial.print(x);
    Serial.print(" | Y:");
    Serial.print(y);
    Serial.print(" | Z:");
    Serial.print(z);
    Serial.print(" | Impact:");
    Serial.println(latchedImpact);

    bluetooth.print("Packet ");
    bluetooth.print(packetCount);
    bluetooth.print(" | X:");
    bluetooth.print(x);
    bluetooth.print(" | Y:");
    bluetooth.print(y);
    bluetooth.print(" | Z:");
    bluetooth.print(z);
    bluetooth.print(" | Impact:");
    bluetooth.println(latchedImpact);

    // Reset after printing
    latchedImpact = 0;
  }
}