## 🔧 Sortify Hardware – Dokumentasi Sistem Servo & Kamera Berbasis ESP32

### 🧩 Deskripsi Umum

**Sortify Hardware** adalah sistem berbasis **ESP32** yang berfungsi untuk:

- Mengambil data gambar dari kamera dan mengirimkannya ke servis Python untuk klasifikasi.
- Menerima perintah klasifikasi sampah dari servis Python melalui komunikasi serial.
- Mengendalikan servo motor untuk memisahkan sampah berdasarkan kategori:

  - **Organik**: Servo bergerak ke kanan (perintah `"S 0"`).
  - **Recycle**: Servo bergerak ke kiri (perintah `"S 1"`).

### 🔀 Arsitektur Komunikasi

```
[ Kamera (Webcam/ESP32-CAM) ]
           │
           ▼
[ Servis Python (Sortify Camera) ]
           │
           ▼
[ ESP32 (Sortify Hardware) ]
           │
           ▼
[ Servo Motor ]
```

### 🌿 Branch Utama

#### 1. `test-servo`

- **Tujuan**: Menguji dan mengendalikan servo motor berdasarkan perintah serial.
- **Fungsi Utama**:

  - Menerima perintah melalui serial untuk menggerakkan servo ke posisi tertentu.
  - Menggunakan sinyal PWM untuk mengendalikan posisi servo.

- **Catatan**: Cocok untuk pengujian awal sistem servo tanpa integrasi kamera.([Gist][1])

#### 2. `test-camera` / `main`

- **Tujuan**: Mengintegrasikan kamera ESP32-CAM untuk mengambil gambar dan mengirimkannya ke servis Python.
- **Fungsi Utama**:

  - Menghubungkan ESP32 dengan modul kamera untuk mengambil gambar.
  - Mengirim data gambar melalui komunikasi serial ke servis Python untuk klasifikasi.

- **Catatan**: Mengurangi beban komputasi pada perangkat seperti Raspberry Pi dengan memindahkan proses pengambilan gambar ke ESP32.

### 🚀 Peningkatan Mendatang

- **Integrasi Kamera**:

  - Menggunakan **ESP32-S3-WROOM-1** yang dilengkapi dengan port kamera untuk mengambil data gambar secara langsung.
  - Mengirimkan data gambar ke servis Python melalui komunikasi serial, mengurangi beban komputasi pada perangkat seperti Raspberry Pi.

- **Optimasi Performa**:

  - Mengurangi ketergantungan pada paket seperti OpenCV di servis Python.
  - Meningkatkan efisiensi dan responsivitas sistem secara keseluruhan.

### 📁 Struktur Repositori

```
Sortify-Hardware/
├── .pio/                   # Folder build PlatformIO
├── .vscode/                # Konfigurasi VSCode
├── include/                # Header files jika dibutuhkan
├── lib/                    # Library eksternal (misal untuk kontrol servo)
├── src/
│   └── main.cpp            # Kode utama (bisa test-servo atau test-camera)
├── test/                   # Unit test (opsional)
├── platformio.ini          # Konfigurasi proyek PlatformIO
├── README.md               # Dokumentasi proyek
```

### 📌 Catatan Tambahan

- Pastikan baud rate komunikasi serial antara ESP32 dan servis Python sesuai (115200 bps).
- Gunakan catu daya eksternal yang stabil untuk servo motor untuk menghindari fluktuasi tegangan.
- Periksa kompatibilitas pin PWM pada ESP32 dengan model servo yang digunakan.

🚀 Happy Coding!
