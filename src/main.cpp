#include <Arduino.h>
// #include <WiFi.h>
// #include <ArduinoWebsockets.h>

// Definisi pin untuk koneksi ke ESP32-CAM (UART2)
// #define CAM_RX 16 // Pin RX ESP32 untuk menerima data dari ESP32-CAM
// #define CAM_TX 17 // Pin TX jika diperlukan (biasanya tidak dipakai untuk hanya menerima)

// Ganti SSID dan password sesuai jaringan WiFi Anda
// const char *ssid = "YOUR_SSID";
// const char *password = "YOUR_PASSWORD";

// // IP server Python dan port WebSocket (misalnya port 8765)
// const char *websocket_server_host = "192.168.1.100";
// const uint16_t websocket_server_port = 8765;

// using namespace websockets;

// WebsocketsClient client;

// HardwareSerial camSerial(2); // Menggunakan UART2

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

// Buffer untuk mengakumulasi frame JPEG
// String frameBuffer = "";

// void connectToWiFi()
// {
//   Serial.print("Menghubungkan ke WiFi ");
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi terhubung. IP: " + WiFi.localIP().toString());
// }

// void connectWebSocket()
// {
//   // Membentuk URL misal "ws://192.168.1.100:8765/"
//   String url = "ws://";
//   url += websocket_server_host;
//   url += ":";
//   url += String(websocket_server_port);
//   url += "/";
//   client.connect(url);
//   Serial.println("Terhubung ke WebSocket server di " + url);
// }

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
      moveServo(PWM_MAX, 3); // 180°
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
  delay(1000);
  // connectToWiFi();
  // connectWebSocket();

  // // Inisialisasi serial untuk data dari ESP32-CAM
  // camSerial.begin(115200, SERIAL_8N1, CAM_RX, CAM_TX);
  // Serial.println("Inisialisasi koneksi ke ESP32-CAM selesai.");

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
  // Pastikan koneksi WebSocket tetap aktif
  // client.poll();

  // // Baca data dari ESP32-CAM (UART2)
  // while (camSerial.available())
  // {
  //   char c = camSerial.read();
  //   frameBuffer += c;

  //   // Jika mendeteksi akhir frame JPEG (marker 0xFFD9)
  //   if (frameBuffer.endsWith("\xFF\xD9"))
  //   {
  //     Serial.println("Frame lengkap diterima, mengirim lewat WebSocket...");
  //     // Kirim frame sebagai string (data biner dikirim sebagai string;
  //     // pastikan server Python melakukan decoding sesuai)
  //     client.send(frameBuffer);
  //     frameBuffer = ""; // Reset buffer untuk frame berikutnya
  //   }
  // }
}
