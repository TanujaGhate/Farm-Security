#include <Ultrasonic.h>

// Pin definitions (using GPIO numbers)
#define TRIGGER_PIN 5     // GPIO5 for ultrasonic sensor trigger
#define ECHO_PIN 4        // GPIO4 for ultrasonic sensor echo
#define BUZZER_PIN 14     // GPIO14 for buzzer

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN); // Ultrasonic sensor setup

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  long distance = ultrasonic.read();
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check for intruder within a specific distance
  if (distance < 30) {  // Adjust distance threshold as necessary
    triggerAlarm();      // Activate the alarm
    delay(1000);         // Avoid constant buzzing by adding delay
  }
}

void triggerAlarm() {
  digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
  delay(1000);                     // Delay to keep buzzer on
  digitalWrite(BUZZER_PIN, LOW);   // Turn off buzzer
}
