# Predictive Electrical Fault Detection and Classification using IoT-Based Smart Circuit Breaker

## 📌 Project Overview
This project provides an intelligent and reliable solution for electrical safety, monitoring, and control using IoT technology. The system operates directly on 230V AC mains supply, converted to 5V DC using HLK-PM01 and further regulated to 3.3V using AMS1117 for sensor interfacing. The system continuously monitors electrical parameters and automatically disconnects the load when a fault is detected — making it smarter than traditional circuit breakers.

## ⚙️ Features
- ✅ Real-time voltage and current monitoring
- ✅ Automatic fault detection — Over-voltage, Under-voltage, Over-current
- ✅ Instant relay trip on fault detection
- ✅ Remote control via Blynk IoT mobile app
- ✅ Manual control using push button
- ✅ Mobile push notifications on fault events
- ✅ Real-time data dashboard on Blynk app

## 🛠️ Hardware Components

| Component | Purpose |
|-----------|---------|
| ESP32 Microcontroller | Central processing + WiFi |
| ZMPT101B Voltage Sensor | AC Voltage sensing |
| ACS712 Current Sensor | Current sensing |
| Relay Module | Load disconnection |
| Push Button | Manual control |
| HLK-PM01 Power Module | 230V AC to 5V DC conversion |
| AMS1117 Voltage Regulator | 5V to 3.3V DC for ZMPT101B |

## 📱 Software & Platform
- Arduino IDE
- Blynk IoT Platform
- Blynk Cloud (WiFi based)

## 🔌 How It Works
1. System powered directly from 230V AC mains via HLK-PM01
2. ZMPT101B and ACS712 continuously sense voltage and current
3. ESP32 processes the analog signals in real-time
4. If values cross predefined thresholds → fault is classified
5. Relay immediately trips → load disconnected
6. User gets instant notification on Blynk mobile app
7. User can also manually control load via push button or Blynk dashboard

## 📊 Fault Classification

| Fault Type | Condition |
|------------|-----------|
| Over-voltage | Voltage exceeds upper threshold |
| Under-voltage | Voltage drops below lower threshold |
| Over-current | Current exceeds safe limit |

## 🚀 Applications
- Smart Homes
- Industrial Electrical Systems
- Energy Management Systems

## 👨‍💻 Author
**Sulaiman D**
B.E. Electronics and Communication Engineering
📍 Madurai, Tamil Nadu, India
🔗 [GitHub](https://github.com/sulaimand-embedded)
