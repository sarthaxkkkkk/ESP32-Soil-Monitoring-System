
# 🌱 ESP32 Soil Monitoring System

An IoT-based soil monitoring system built using an ESP32 to monitor environmental and soil conditions in real time.

The system collects **temperature, humidity, soil moisture, and rain sensor data**, displays the readings on an OLED display, and uploads the sensor data to **ThingSpeak** for remote monitoring and visualization.

---

## 📌 Project Overview

The objective of this project is to develop a low-cost IoT-based system for monitoring soil and surrounding environmental conditions.

The ESP32 acts as the main controller and:

- Reads temperature and humidity using a DHT22 sensor
- Monitors soil moisture using a capacitive soil moisture sensor
- Detects rain using an FC-37 rain sensor
- Displays sensor readings on a 0.96-inch OLED display
- Connects to Wi-Fi
- Sends sensor data to ThingSpeak
- Provides real-time cloud-based visualization of sensor readings

---

## ⚙️ System Architecture

```text
                 ┌──────────────────┐
                 │      DHT22       │
                 │ Temperature &    │
                 │     Humidity     │
                 └────────┬─────────┘
                          │
                          │
┌──────────────────┐      │      ┌──────────────────┐
│ Soil Moisture    │──────┼──────│                  │
│     Sensor       │      │      │      ESP32       │
└──────────────────┘      ├──────│  Microcontroller │
                          │      │                  │
┌──────────────────┐      │      └───────┬──────────┘
│   FC-37 Rain     │──────┘              │
│     Sensor       │                     │
└──────────────────┘                     │
                                         │
                              ┌──────────┴──────────┐
                              │                     │
                              ▼                     ▼
                       ┌─────────────┐       ┌─────────────┐
                       │    OLED     │       │  ThingSpeak │
                       │   Display   │       │    Cloud    │
                       └─────────────┘       └─────────────┘
