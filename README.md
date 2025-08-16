# 🌱 Hydroponic System Controller

An ESP8266-based automated hydroponic system controller with web interface for managing water pumps, LED lighting, and automated scheduling.

## ✨ Features

### 🔧 Hardware Control
- **Motor Control**: Water pump management with variable speed control (PWM)
- **LED Control**: Grow light automation with full on/off control
- **Safety System**: Mutual exclusion prevents motors and LEDs from running simultaneously
- **L298N Integration**: Dual H-bridge driver support for both motors and LED strips

### 🌐 Web Interface
- **Modern UI**: Responsive web interface with gradient design and card-based layout
- **Real-time Control**: Instant motor/LED on/off controls with visual feedback
- **Speed Control**: Variable motor speed adjustment (0-100%) via slider
- **Status Monitoring**: Live status updates with error handling

### ⏰ Automation Features
- **Dual Intervals**: Separate configurable intervals for motors and LEDs (1-1440 minutes)
- **Smart Scheduling**: Motors have priority over LEDs in automatic mode
- **Conflict Resolution**: Automatic switching prevents simultaneous operation

### 📶 Connectivity
- **WiFi Management**: Built-in WiFi configuration with credential storage
- **Access Point Mode**: Fallback AP mode for initial setup
- **EEPROM Storage**: Persistent WiFi credentials storage
- **CORS Support**: Enhanced web performance with cross-origin resource sharing

## 🔌 Hardware Requirements

### Main Components
- **ESP8266 NodeMCU v2** (ESP-12E Module)
- **L298N Motor Driver Module** (Dual H-Bridge)
- **Water Pump** (12V DC recommended)
- **LED Strip/Grow Lights** (12V DC)
- **12V Power Supply** (adequate for pump + LEDs)

### Pin Configuration

#### Motor Control (L298N Motor A)
```
ESP8266 D3 → L298N IN1
ESP8266 D4 → L298N IN2
ESP8266 D5 → L298N ENA (PWM)
```

#### LED Control (L298N Motor B)
```
ESP8266 D1 → L298N IN3
ESP8266 D2 → L298N IN4
ESP8266 D6 → L298N ENB (Enable)
```

#### Power Connections
```
12V Supply → L298N VCC, Motor+, LED+
GND → L298N GND, Motor-, LED-, ESP8266 GND
ESP8266 VIN → 5V (or use separate 3.3V supply)
```

## 🚀 Getting Started

### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- ESP8266 board connected via USB

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/jarffs/hidroponic.git
   cd hidroponic
   ```

2. **Build and upload**
   ```bash
   platformio run --target upload
   ```

3. **Monitor serial output**
   ```bash
   platformio device monitor
   ```

### First Setup

1. **Power on the ESP8266** - It will create a WiFi access point
2. **Connect to WiFi** - Join "MotorControllerESP" network
3. **Access web interface** - Navigate to `http://192.168.4.1`
4. **Configure WiFi** - Enter your home WiFi credentials
5. **System restart** - Device will connect to your network
6. **Find new IP** - Check serial monitor or router for assigned IP

## 🎮 Usage

### Web Interface Access
- **Local Network**: `http://[ESP_IP_ADDRESS]`
- **Setup Mode**: `http://192.168.4.1`

### Controls

#### Motor Control
- **Manual**: On/Off buttons with immediate response
- **Speed**: Adjustable PWM control (0-100%)
- **Interval**: Configurable auto-cycling (default: 5 minutes)

#### LED Control
- **Manual**: On/Off buttons with status feedback
- **Interval**: Separate auto-cycling (default: 10 minutes)
- **Safety**: Auto-disabled when motors are active

#### Configuration
- **WiFi Settings**: Change network credentials
- **Motor Interval**: Set automatic pump cycles
- **LED Interval**: Configure lighting schedule

## 🔒 Safety Features

### Mutual Exclusion System
- **Priority**: Motors have priority over LEDs
- **Auto-switching**: System automatically turns off conflicting devices
- **User Feedback**: Web interface shows when auto-switching occurs
- **Debug Logging**: Serial output tracks all switching events

### Power Management
- **Current Limiting**: L298N provides overcurrent protection
- **Safe Startup**: All outputs disabled on boot
- **Emergency Stop**: Both devices can be turned off simultaneously

## 📁 Project Structure

```
src/
├── main.cpp              # Main program loop and setup
├── MotorControl.cpp/h    # Motor control functions
├── LedControl.cpp/h      # LED control functions
├── WebServerHandler.cpp/h # Web server and API endpoints
├── WiFiManager.cpp/h     # WiFi connection management
└── EEPROMManager.cpp/h   # Persistent storage functions

platformio.ini            # Build configuration
README.md                 # This file
LICENSE                   # License information
```

## 🔧 Configuration

### Default Settings
```cpp
motorInterval = 5 * 60 * 1000;   // 5 minutes
ledInterval = 10 * 60 * 1000;    // 10 minutes
motorSpeed = 1023;               // Full speed (PWM)
serialSpeed = 115200;            // Debug output
```

### Customization
- Modify intervals in `WebServerHandler.cpp`
- Adjust pin mappings in respective control files
- Change PWM ranges for different motor types
- Customize web interface in `handleRoot()` function

## 🐛 Troubleshooting

### Common Issues

**Motors/LEDs not responding**
- Verify L298N enable pins (D5/D6) are connected
- Check 12V power supply connections
- Monitor serial output for debug messages

**Web interface not loading**
- Confirm ESP8266 WiFi connection (check serial monitor)
- Try access point mode: connect to "MotorControllerESP"
- Verify IP address assignment

**Automatic intervals not working**
- Check serial monitor for timing messages
- Ensure intervals are set (default: 5min motors, 10min LEDs)
- Verify system isn't stuck in manual override

### Debug Output
The system provides extensive serial logging:
```
DEBUG: Motors turned on - A1=HIGH, A2=LOW, ENA=1023
DEBUG: LED off - B1=LOW, B2=LOW, ENB=0
Web request: Motor ON
Motor interval set to 5 minutes
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike (CC BY-NC-SA).

## 🙏 Acknowledgments

- ESP8266 Arduino Core community
- PlatformIO development team
- L298N motor driver documentation contributors

## 📊 System Status

- **Platform**: ESP8266 (Arduino Framework)
- **Memory Usage**: ~45% RAM, ~30% Flash
- **Build Status**: ✅ Compiled and tested
- **Last Updated**: August 2025

---

**Happy Growing! 🌱**