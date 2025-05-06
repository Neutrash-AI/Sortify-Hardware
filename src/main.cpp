#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <esp_camera.h>

// Ganti SSID dan password sesuai jaringan WiFi Anda
const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

// IP server Python dan port WebSocket (misalnya port 8765)
const char *websocket_server_host = "192.168.1.100";
const uint16_t websocket_server_port = 8765;

using namespace websockets;
WebsocketsClient client;

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

// Konfigurasi pin OV2640 untuk ESP32-S3 WROOM
static camera_config_t camera_config = {
    .pin_pwdn = -1,
    .pin_reset = -1,
    .pin_xclk = 21,
    .pin_sccb_sda = 26,
    .pin_sccb_scl = 27,

    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 19,
    .pin_d2 = 18,
    .pin_d1 = 5,
    .pin_d0 = 4,

    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 12,
    .fb_count = 1};

void connectToWiFi()
{
  Serial.print("Menghubungkan ke WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung. IP: " + WiFi.localIP().toString());
}

void connectWebSocket()
{
  // Membentuk URL misal "ws://192.168.1.100:8765/"
  String url = "ws://";
  url += websocket_server_host;
  url += ":";
  url += String(websocket_server_port);
  url += "/";
  client.connect(url);
  Serial.println("Terhubung ke WebSocket server di " + url);
}

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
// void readSerial()
// {
//   if (Serial.available() > 0)
//   {
//     String command = Serial.readStringUntil('\n');
//     command.trim();

//     // for recycle trash
//     if (command == "S 1")
//     {
//       moveServo(PWM_MAX, 3); // 180°
//     }
//     // for unrecycle trash
//     else if (command == "S 0")
//     {
//       moveServo(PWM_MIN, 1); // 0°
//     }
//     else
//     {
//       Serial.println("Invalid command! Use 'S 1' for RIGHT, 'S 0' for LEFT.");
//     }
//   }
// }

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // WiFi & WebSocket
  connectToWiFi();
  connectWebSocket();

  // Inisialisasi kamera
  if (esp_camera_init(&camera_config) != ESP_OK)
  {
    Serial.println("Camera init failed!");
    while (true)
    {
      delay(1000);
    }
  }
  Serial.println("Inisialisasi koneksi kamera selesai.");

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
  // readSerial();
  // Pastikan koneksi WebSocket tetap aktif
  client.poll();

  // Ambil satu frame dari kamera
  camera_fb_t *fb = esp_camera_fb_get();
  if (fb)
  {
    // Kirim data frame sebagai binary lewat WebSocket
    client.sendBinary((const char*)fb->buf, fb->len);
    esp_camera_fb_return(fb);
    Serial.println("Frame dikirim via WebSocket.");
  }
  else
  {
    Serial.println("Gagal mengambil frame.");
  }

  // Delay antar frame (atur sesuai kebutuhan, misal 100 ms untuk ~10 fps)
  delay(100);
}
