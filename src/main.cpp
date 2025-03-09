#include <Arduino.h>
#include <Servo.h>

// Constanta pin servo
constexpr int SERVO_PIN = 18;
constexpr int LED_PIN = 2;

// Constanta degree servo
constexpr int SERVO_LEFT = 0;
constexpr int SERVO_CENTER = 90;
constexpr int SERVO_RIGHT = 180;

Servo myServo;

// Set servo angle
void moveServo(int angle) {
    myServo.write(angle);
    Serial.printf("Servo moved to %d°\n", angle);
    delay(1500); // Wait for 1.5 seconds
    myServo.write(SERVO_CENTER);
}

void setup()
{
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT); // Set LED pin as output

  myServo.write(SERVO_CENTER);
  Serial.println("Servo Controller Initialized. Waiting for commands...");
}

void loop()
{
  // put your main code here, to run repeatedly:
}
