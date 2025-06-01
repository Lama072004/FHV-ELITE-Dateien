#include "heart.h"
#include "esp_log.h"

// Simulated sensor data for pulse and SpO2
static uint8_t pulse = 60; // Default pulse value
static float spo2 = 98.0;  // Default SpO2 value

// Function to simulate reading pulse and SpO2 values from a sensor
void read_sensor_data(void) {
    // In a real implementation, this function would interface with a sensor
    // For simulation, we will just increment the pulse value and keep SpO2 constant
    pulse++;
    if (pulse > 100) {
        pulse = 60; // Reset pulse value for simulation
    }
}

// Function to send the sensor data to the BLE module
void send_sensor_data(void) {
    read_sensor_data(); // Read the latest sensor data
    ble_notify_values(pulse, spo2); // Send the data via BLE
}