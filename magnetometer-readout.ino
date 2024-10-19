#include <Wire.h>
#include "Rm3100.h"

#define LED_PIN 13  // Built-in LED pin
#define BLINK_COUNT 2  // Number of fast blinks
#define BLINK_DURATION 100  // Duration of each blink (ms)
#define WAIT_DURATION 2000  // Wait duration after blink sequence (ms)

// Create an instance of the Rm3100 class
Rm3100 sensor(Wire, (Rm3100::RM3100_ADDR | Rm3100::RM3100_ADDR_SSN) << 1);

bool sensorConnected = false;

void blinkLED() {
  for (int i = 0; i < BLINK_COUNT; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(BLINK_DURATION);
    digitalWrite(LED_PIN, LOW);
    delay(BLINK_DURATION);
  }
  delay(WAIT_DURATION);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  
  // Initialize I2C communication
  Wire.begin();  // Use default I2C pins (A4 for SDA, A5 for SCL on most Arduino boards)
  
  Serial.println("Initializing RM3100 Magnetometer...");
  
  int result = sensor.Begin();
  if (result != Rm3100::RM3100_RET_OK) {
    Serial.println("Magnetometer not connected!");
    sensorConnected = false;
  } else {
    Serial.println("RM3100 Magnetometer detected and initialized successfully!");
    sensorConnected = true;
    
    sensor.SetCycleCounts(200);
    sensor.SetRate(100.0f);
    sensor.SetContinuousMeasurementMode(true);
  }
}

void loop() {
  if (!sensorConnected) {
    Serial.println("Magnetometer not connected!");
    
    blinkLED();  // Perform the blink sequence
    
    int result = sensor.Begin();
    if (result == Rm3100::RM3100_RET_OK) {
      Serial.println("RM3100 Magnetometer detected and initialized successfully!");
      sensorConnected = true;
      sensor.SetCycleCounts(200);
      sensor.SetRate(100.0f);
      sensor.SetContinuousMeasurementMode(true);
      digitalWrite(LED_PIN, LOW);  // Turn off LED when connected
    }
  } else {
    Rm3100::Status status;
    Rm3100::Sample sample;
    
    sensor.GetStatus(&status);
    if (status.drdy) {
      if (sensor.GetSample(&sample) == Rm3100::RM3100_RET_OK) {
        // Send out the data in a structured format
        Serial.print("MAG ");
        Serial.print(millis());  // Timestamp
        Serial.print(" ");
        Serial.print(sample.x, 4);
        Serial.print(" ");
        Serial.print(sample.y, 4);
        Serial.print(" ");
        Serial.print(sample.z, 4);
        Serial.println();
      } else {
        Serial.println("Error: Failed to read sample from RM3100");
        sensorConnected = false;  // Assume sensor disconnected if read fails
      }
    }
  }
  
  delay(10);  // Small delay to prevent overwhelming the serial output
}