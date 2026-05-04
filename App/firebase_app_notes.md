# Firebase App Integration Notes

## Overview

This project integrates a **Firebase Realtime Database** with an ESP32-based solar EV charging system to enable remote monitoring and control through a mobile or web application.

The system allows users to:

* Monitor charging status in real-time
* Control charging (Start/Stop) remotely
* Track voltage, current, and energy consumption
* Receive alerts for faults or low solar power

---

## Firebase Setup

### 1. Create Firebase Project

* Go to Firebase Console
* Create a new project
* Enable **Realtime Database**
* Set rules (for testing):

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

---

## Database Structure

Example structure used in this project:

```json
{
  "charging": {
    "status": true
  },
  "sensorData": {
    "voltage": 220,
    "current": 5,
    "battery": 12.5,
    "solar": 18.2,
    "energy": 150
  },
  "alerts": {
    "lowBattery": false,
    "overCurrent": false,
    "lowSolar": false
  }
}
```

---

## ESP32 Integration (REST API)

ESP32 communicates with Firebase using HTTP requests.

### Libraries Required

* WiFi.h
* HTTPClient.h

### Send Data to Firebase

```cpp
HTTPClient http;
http.begin("https://your-project-id.firebaseio.com/sensorData.json");
http.addHeader("Content-Type", "application/json");

String json = "{\"voltage\":220,\"current\":5}";
http.POST(json);
http.end();
```

### Read Data (Control Relay)

```cpp
http.begin("https://your-project-id.firebaseio.com/charging/status.json");
int httpCode = http.GET();

if (httpCode > 0) {
  String payload = http.getString();
  if (payload == "true") {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}
http.end();
```

---

## Mobile App Integration

The app (Flutter / Android / Web) connects to Firebase to:

* Display real-time sensor data
* Toggle charging ON/OFF
* Show alerts and notifications

### Example Logic

* Button ON → sets `charging/status = true`
* Button OFF → sets `charging/status = false`

---

## Features Enabled

* Remote charging control
* Live monitoring dashboard
* Energy tracking
* Smart alerts system
* Scalable cloud-based architecture

---

## Future Enhancements

* User authentication (Firebase Auth)
* Payment gateway integration
* Slot booking system
* Push notifications using Firebase Cloud Messaging (FCM)
* AI-based energy optimization

---

## Notes

* Ensure ESP32 and Firebase are connected to stable internet
* Use secure rules in production (avoid public access)
* Optimize API calls to reduce latency and power usage

---
