#include <Servo.h>

Servo sl;   // 180° positional servo
Servo cr;   // 360° continuous servo

int trigPin = 5;
int echoPin = 6;
long duration;
int cm;

// Store whether the object was previously close
bool objectClose = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  sl.attach(9);   // 180° servo signal pin
  cr.attach(10);  // 360° continuous servo signal pin

  sl.write(0);    // Ensure starting position
  cr.write(90);   // Stop continuous servo
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo (with timeout)
  duration = pulseIn(echoPin, HIGH, 20000); // timeout after 20 ms (~3.4 m range)

  // Calculate distance in cm
  if (duration == 0) {
    Serial.println("No echo");
    return; 
  }

  cm = duration / 2 / 29.1;

  // Limit detection range
  if (cm <= 0 || cm > 300) {
    Serial.println("Out of range");
    return;
  }

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  // Control the servos based on distance
  if (cm <= 15 && !objectClose) {
    // Object just came close
    sl.write(110);        // Move 180° servo ~110°
    cr.write(0);          // Rotate 360° servo CW
    delay(420);           // ~100° rotation (tune this if needed)
    cr.write(90);         // Stop continuous servo
    objectClose = true;   // Mark object as close
  } 
  else if (cm > 15 && objectClose) {
    // Object moved away, return servos to initial position
    sl.write(0);          // Reset 180° servo
    cr.write(180);        // Rotate 360° servo CCW
    delay(420);           // ~100° back rotation (same timing as forward)
    cr.write(90);         // Stop continuous servo
    objectClose = false;  // Mark object as far
  }

  delay(100); // Small delay to avoid too frequent readings
}
