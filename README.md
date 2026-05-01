# 🐶 IoT-Based Smart Dog Collar

An IoT-enabled smart dog collar that monitors:
- 📍 GPS Location
- ❤️ Heart Rate
- 🌡 Temperature
- 🔊 Barking Activity

---

## 🚀 Features
- Real-time monitoring using ESP32
- Web server-based dashboard
- Sensor-based health tracking
- Alert detection (barking, abnormal values)

---

## 🧠 System Architecture
Sensors → ESP32 → WiFi → Web Server → User Interface

---

## 🛠 Hardware Used
- ESP32
- GPS Module
- MAX30105 (Heart Sensor)
- DS18B20 (Temperature)
- Microphone Sensor

---

## 💻 Software
- Arduino IDE
- Embedded C++
- Web-based UI

---

## 🔌 Setup Instructions
1. Upload code to ESP32
2. Connect sensors properly
3. Power the device
4. Connect to WiFi: `DogCollar`
5. Open browser: `192.168.4.1`

---

## 📊 Output
- JSON API: `/data`
- Displays real-time sensor values

---

## 🔮 Future Scope
- AI-based health prediction
- Geo-fencing alerts
- Mobile app integration
