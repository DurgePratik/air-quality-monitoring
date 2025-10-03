 # 🌍 Air Quality Monitoring System (ESP8266 + MQ135 + DHT22)

An **IoT-based Smart Air Quality Monitoring System** built using the **ESP8266 Wi-Fi module**,  
**MQ135 gas sensor**, and **DHT22 temperature & humidity sensor**.  

This system continuously measures **air quality, temperature, and humidity**, and uploads real-time data to the **cloud**, making it accessible **anytime, anywhere** through a web dashboard or mobile device.  

---

## ✨ Features

- 📡 **Real-time monitoring** of air quality, temperature, and humidity  
- ☁️ **Cloud Integration**: Access data remotely from any device  
- 📊 **Graphical Dashboard**: Visualize trends with charts & logs  
- 🌡️ **DHT22 Sensor**: Accurate temperature & humidity readings  
- 🛡️ **MQ135 Gas Sensor**: Detects harmful gases & pollution levels  
- ⚡ **Wi-Fi Connectivity** with ESP8266 — no extra networking hardware needed  
- 🔔 **Threshold-based Alerts** (e.g., Poor Air Quality notifications)  
- 🔌 **DIY Friendly & Low Cost**  

---

## 🧰 Hardware Requirements

| Component       | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| **ESP8266**     | NodeMCU / Wemos D1 Mini (Wi-Fi-enabled microcontroller)                     |
| **MQ135 Sensor**| Gas sensor for detecting air pollutants (NH₃, NOₓ, CO₂, benzene, etc.)      |
| **DHT22 Sensor**| Digital sensor for temperature & humidity                                   |
| **Jumper Wires**| For connections                                                            |
| **Power Supply**| Micro USB cable or 5V regulated adapter                                     |
| *(Optional)*    | Breadboard / Enclosure for housing the system                              |

---

## ⚙️ System Architecture


1. Sensors collect **air quality, temperature, and humidity data**.  
2. ESP8266 processes values and connects to Wi-Fi.  
3. Data is **uploaded to the cloud** (ThingSpeak / Firebase / Blynk).  
4. Users access readings via **cloud dashboard, PC, or mobile device**.  

---

## 📝 Code Details

- **MQ135** provides analog gas concentration values.  
- **DHT22** provides temperature (°C) and humidity (%) readings.  
- **ESP8266** handles:
  - Wi-Fi connectivity  
  - Uploading data to cloud servers  
  - Serial debugging/logging  

The main sketch is `swsmaincode.ino`.

---

## 🚀 Getting Started

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/DurgePratik/air-quality-monitoring.git
cd air-quality-monitoring
