#include <Servo.h>
#include <NewPing.h>

// Pin definitions
#define TRIG_PIN 9       // Ultrasonic sensor TRIG pin
#define ECHO_PIN 10      // Ultrasonic sensor ECHO pin
#define BUZZER_PIN 8     // Buzzer connected to pin 8
#define SERVO_PIN 6      // Servo motor connected to pin 6
#define LED_PIN 7        // LED connected to pin 7

// Ultrasonic sensor range
#define MAX_DISTANCE 50 // Maximum distance in cm for the ultrasonic sensor

Servo servo;                        // Servo motor object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // Ultrasonic sensor object

bool objectDetected = false; // Flag to track if an object is detected
int lastServoAngle = 0;      // Variable to store the last servo position

void setup() {
  Serial.begin(9600);         // Initialize serial communication for debugging
  servo.attach(SERVO_PIN);    // Attach servo to specified pin
  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
  pinMode(LED_PIN, OUTPUT);    // Set LED pin as output
}

void loop() {
  if (!objectDetected) {
    // Sweep the servo smoothly from 0 to 90 degrees and back
    for (int angle = 0; angle <= 90; angle += 5) {
      if (objectDetected) break; // Stop sweeping if an object is detected
      servo.write(angle);
      lastServoAngle = angle; // Update the last servo position
      delay(50); // Allow the servo to stabilize
      checkForIntrusion();
    }

    for (int angle = 90; angle >= 0; angle -= 5) {
      if (objectDetected) break; // Stop sweeping if an object is detected
      servo.write(angle);
      lastServoAngle = angle; // Update the last servo position
      delay(50); // Allow the servo to stabilize
      checkForIntrusion();
    }
  } else {
    // Keep checking for intrusion while the servo is stopped
    checkForIntrusion();
  }
}

void checkForIntrusion() {
  unsigned int distance = sonar.ping_cm(); // Get distance in cm

  if (distance > 0 && distance <= 50) { // If an obstacle is detected within 50 cm
    if (!objectDetected) { // Trigger actions only when detection starts
      objectDetected = true;
      Serial.println("Intruder detected!");
      digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
      digitalWrite(LED_PIN, HIGH);   // Turn on the LED
      servo.write(lastServoAngle);  // Stop the servo at its current position
    }
  } else {
    if (objectDetected) { // Trigger actions only when detection ends
      objectDetected = false;
      Serial.println("Object cleared!");
      digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
      digitalWrite(LED_PIN, LOW);   // Turn off the LED
    }
  }
}
