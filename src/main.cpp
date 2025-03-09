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

// Set LED blink
void blinkLED(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// Set servo angle
void moveServo(int angle, int blinkTimes)
{
  myServo.write(angle);
  Serial.printf("Servo moved to %d°\n", angle);
  blinkLED(blinkTimes);
  delay(1500); // Wait for 1.5 seconds
  myServo.write(SERVO_CENTER);
}

// Listen for serial command
void readSerial()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // for recycle trash
    if (command == "S 1")
    {
      moveServo(SERVO_RIGHT, 3);
    }
    // for unrecycle trash
    else if (command == "S 0")
    {
      moveServo(SERVO_LEFT, 1);
    }
    else
    {
      Serial.println("Invalid command! Use 'S 1' for RIGHT, 'S 0' for LEFT.");
    }
  }
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
