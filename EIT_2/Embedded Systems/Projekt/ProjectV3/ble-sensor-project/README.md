# BLE Sensor Project

## Overview
The BLE Sensor Project is designed to implement a Bluetooth Low Energy (BLE) application that reads pulse and SpO2 values from a sensor and transmits this data to connected devices. The project consists of several modules that handle BLE communication and sensor data processing.

## Project Structure
```
ble-sensor-project
├── main
│   ├── ble.c          # BLE implementation, advertising data, GATT server setup
│   ├── ble.h          # Header file for BLE functions and constants
│   ├── heart.c        # Sensor data handling for pulse and SpO2 values
│   ├── heart.h        # Header file for heart functions and constants
│   └── main.c         # Entry point of the application
├── CMakeLists.txt     # CMake configuration file
├── Makefile            # Makefile for building the project
└── README.md           # Project documentation
```

## Requirements
- ESP-IDF (Espressif IoT Development Framework)
- Compatible ESP32 development board
- Necessary libraries for BLE and sensor integration

## Setup Instructions
1. **Clone the Repository**
   ```bash
   git clone <repository-url>
   cd ble-sensor-project
   ```

2. **Install ESP-IDF**
   Follow the installation instructions from the [ESP-IDF documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).

3. **Build the Project**
   You can build the project using either CMake or Make:
   - Using CMake:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```
   - Using Make:
     ```bash
     make
     ```

4. **Flash the Firmware**
   Connect your ESP32 board and run:
   ```bash
   make flash
   ```

5. **Monitor the Output**
   Use the serial monitor to view logs and debug information:
   ```bash
   make monitor
   ```

## Usage
- Once the application is running, it will start advertising as a BLE device named "PulseOxy".
- Connect to the device using a BLE scanner app to view pulse and SpO2 values.
- The application will continuously read sensor data and send notifications to connected devices.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.