# 🌍 Air Quality Monitoring System with ESP8266

A smart IoT-based **Air Quality Monitoring System** built using the **ESP8266 Wi-Fi module**.  
This project continuously monitors air quality using gas sensors and uploads real-time data to the cloud, making it accessible from **anywhere, anytime**.

---

## ✨ Features

- 📡 **Real-time Air Quality Monitoring** using gas/air quality sensors  
- ☁️ **Cloud Integration**: View live data remotely from any device  
- 📊 **Data Visualization**: Graphs and dashboards for trends and analysis  
- ⚡ **ESP8266 Wi-Fi Connectivity**: No need for external hardware for networking  
- 🔔 **Threshold-based Alerts**: Detects poor air quality and alerts users  
- 🔌 **Low-Cost, DIY-Friendly**: Easy to build with commonly available components  

---

## 🧰 Hardware Requirements

| Component | Description |
|-----------|-------------|
| **ESP8266 board** | NodeMCU / Wemos D1 mini |
| **Gas sensor** | MQ-series sensor (e.g. MQ-135 for CO₂, NH₃, benzene, etc.) |
| **Jumper wires** | For connections |
| **Power source** | Micro USB or 5V regulated supply |
| *(Optional)* Additional sensors | DHT22 (Temperature & Humidity), BMP280 (Pressure) |

---

## ⚙️ System Architecture

1. Sensors collect air quality data.  
2. ESP8266 reads values and connects to Wi-Fi.  
3. Data is uploaded to a cloud platform (ThingSpeak, Firebase, Blynk, or similar).  
4. Users can monitor air quality remotely via dashboards, mobile apps, or web interfaces.  

---

## 📝 Code Details

- Written in **Arduino IDE** for ESP8266 boards.  
- Reads gas sensor values via `analogRead()`.  
- Connects to Wi-Fi using ESP8266 libraries.  
- Uploads sensor readings periodically to the cloud.  
- Cloud dashboards display real-time and historical data.  

---

## 🚀 How to Set Up

1. **Clone this repository**
   ```bash
   git clone https://github.com/DurgePratik/air-quality-monitoring.git
   cd air-quality-monitoring
