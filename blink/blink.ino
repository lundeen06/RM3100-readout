// Arduino LED and Serial Test Sketch

const int ledPin = 13;  // Most Arduino boards have a built-in LED on pin 13

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(ledPin, OUTPUT);  // Initialize the LED pin as an output
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Turn the LED on
  Serial.println("LED should be ON");
  delay(1000);
  digitalWrite(ledPin, LOW);   // Turn the LED off
  Serial.println("LED should be OFF");
  delay(1000);
}