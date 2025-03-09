#include <Arduino.h>

// Constanta pin servo
constexpr int SERVO_PIN = 18;
constexpr int PWM_CHANNEL = 0;     // PWM Channel 0
constexpr int PWM_FREQ = 50;       // Frekuensi PWM 50Hz
constexpr int PWM_RESOLUTION = 16; // PWM Resolution 16 bit

// Range PWM based on microsecond (µs) from 500µs to 2500µs
constexpr int PWM_MIN = 1638;    // 500µs → 0°
constexpr int PWM_CENTER = 4915; // 1500µs → 90°
constexpr int PWM_MAX = 8192;    // 2500µs → 180°

// Constanta pin LED
constexpr int LED_PIN = 2;

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
  ledcWrite(PWM_CHANNEL, angle);
  Serial.printf("Servo moved to %d°\n", angle);
  blinkLED(blinkTimes);
  delay(1500); // Wait for 1.5 seconds
  ledcWrite(PWM_CHANNEL, PWM_CENTER);
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
      moveServo(PWM_FREQ, 3); // 180°
    }
    // for unrecycle trash
    else if (command == "S 0")
    {
      moveServo(PWM_MIN, 1); // 0°
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
  // Setup PWM for servo
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIN, PWM_CHANNEL);

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);

  ledcWrite(PWM_CHANNEL, PWM_CENTER);
  Serial.println("Servo Controller Initialized. Waiting for commands...");
}

void loop()
{
  readSerial();
}
