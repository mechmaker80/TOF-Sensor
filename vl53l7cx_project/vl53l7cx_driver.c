#include "vl53l7cx_driver.h"
#include <stdio.h>
#include <string.h>

// Helper function to write to VL53L7CX register
static int vl53l7cx_write_register(vl53l7cx_device_t *dev, uint16_t reg, const uint8_t *data, uint8_t len) {
    uint8_t buffer[len + 2];
    buffer[0] = (reg >> 8) & 0xFF;
    buffer[1] = reg & 0xFF;
    memcpy(&buffer[2], data, len);
    
    int result = i2c_write_blocking(dev->i2c, dev->address, buffer, len + 2, false);
    return (result == len + 2) ? VL53L7CX_STATUS_OK : VL53L7CX_STATUS_ERROR;
}

// Helper function to read from VL53L7CX register
static int vl53l7cx_read_register(vl53l7cx_device_t *dev, uint16_t reg, uint8_t *data, uint8_t len) {
    uint8_t reg_addr[2] = {(reg >> 8) & 0xFF, reg & 0xFF};
    
    int result = i2c_write_blocking(dev->i2c, dev->address, reg_addr, 2, true);
    if (result != 2) return VL53L7CX_STATUS_ERROR;
    
    result = i2c_read_blocking(dev->i2c, dev->address, data, len, false);
    return (result == len) ? VL53L7CX_STATUS_OK : VL53L7CX_STATUS_ERROR;
}

// Initialize VL53L7CX sensor
int vl53l7cx_init(vl53l7cx_device_t *dev, i2c_inst_t *i2c, uint8_t address) {
    if (!dev || !i2c) return VL53L7CX_STATUS_ERROR;
    
    dev->i2c = i2c;
    dev->address = address;
    dev->initialized = false;
    
    printf("VL53L7CX: Attempting to initialize sensor at address 0x%02X\n", address);
    
    // Try a simple I2C read first to test communication
    uint8_t test_data;
    int result = i2c_read_blocking(dev->i2c, dev->address, &test_data, 1, false);
    
    if (result != 1) {
        printf("VL53L7CX: I2C communication failed (result=%d)\n", result);
        printf("VL53L7CX: No sensor found at address 0x%02X\n", address);
        return VL53L7CX_STATUS_ERROR;
    }
    
    printf("VL53L7CX: I2C communication successful, sensor found!\n");
    printf("VL53L7CX: Raw sensor data: 0x%02X\n", test_data);
    
    // For now, we'll use a simplified approach
    // The VL53L7CX is a complex sensor that typically requires firmware loading
    // and specific initialization sequences. For this demo, we'll mark it as
    // initialized and try to read data directly.
    
    printf("VL53L7CX: Using simplified initialization (demo mode)\n");
    printf("VL53L7CX: Note: Full VL53L7CX initialization requires firmware loading\n");
    
    dev->initialized = true;
    printf("VL53L7CX: Sensor marked as initialized (demo mode)\n");
    return VL53L7CX_STATUS_OK;
}

// Start ranging measurements
int vl53l7cx_start_ranging(vl53l7cx_device_t *dev) {
    if (!dev || !dev->initialized) return VL53L7CX_STATUS_ERROR;
    
    uint8_t start_cmd = 0x40;  // Start ranging command
    int result = vl53l7cx_write_register(dev, VL53L7CX_REG_SYSTEM__MODE_START, &start_cmd, 1);
    
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Ranging started\n");
    }
    return result;
}

// Stop ranging measurements
int vl53l7cx_stop_ranging(vl53l7cx_device_t *dev) {
    if (!dev || !dev->initialized) return VL53L7CX_STATUS_ERROR;
    
    uint8_t stop_cmd = 0x00;  // Stop ranging command
    int result = vl53l7cx_write_register(dev, VL53L7CX_REG_SYSTEM__MODE_START, &stop_cmd, 1);
    
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Ranging stopped\n");
    }
    return result;
}

// Check if measurement data is ready
int vl53l7cx_get_data_ready(vl53l7cx_device_t *dev, bool *ready) {
    if (!dev || !dev->initialized || !ready) return VL53L7CX_STATUS_ERROR;
    
    // For demo purposes, we'll simulate data ready after a delay
    // In a real implementation, you'd check the actual interrupt status register
    static uint32_t last_check = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Simulate data ready every 100ms for demo
    if (current_time - last_check > 100) {
        *ready = true;
        last_check = current_time;
    } else {
        *ready = false;
    }
    
    return VL53L7CX_STATUS_OK;
}

// Clear interrupt status
int vl53l7cx_clear_interrupt(vl53l7cx_device_t *dev) {
    if (!dev || !dev->initialized) return VL53L7CX_STATUS_ERROR;
    
    uint8_t clear_cmd = 0x01;
    return vl53l7cx_write_register(dev, VL53L7CX_REG_SYSTEM__INTERRUPT_CLEAR, &clear_cmd, 1);
}

// Get zone distance data
int vl53l7cx_get_zone_data(vl53l7cx_device_t *dev, vl53l7cx_zone_data_t *data) {
    if (!dev || !dev->initialized || !data) return VL53L7CX_STATUS_ERROR;
    
    // Clear the data structure
    memset(data, 0, sizeof(vl53l7cx_zone_data_t));
    
    printf("VL53L7CX: Attempting to read real sensor data...\n");
    
    // Try to read actual data from the sensor
    // We'll try different register addresses to find where the data is stored
    
    // Method 1: Try reading from common VL53L7CX data registers
    uint8_t raw_data[128]; // Buffer for raw sensor data
    
    // Try reading from register 0x0006 (common data register)
    int result = vl53l7cx_read_register(dev, 0x0006, raw_data, 64);
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Successfully read 64 bytes from register 0x0006\n");
        printf("VL53L7CX: Raw data: ");
        for (int i = 0; i < 16; i++) { // Show first 16 bytes
            printf("%02X ", raw_data[i]);
        }
        printf("...\n");
        
        // Try to interpret as distance data (assuming 16-bit values)
        for (int i = 0; i < VL53L7CX_NB_ZONES && i < 32; i++) {
            uint16_t raw_value = (raw_data[i * 2 + 1] << 8) | raw_data[i * 2];
            
            // Interpret the raw value
            if (raw_value == 0) {
                data->distance_mm[i] = 0; // No object detected
                data->range_status[i] = 0; // OK
            } else if (raw_value > 8000) {
                data->distance_mm[i] = 0; // Likely an error value
                data->range_status[i] = 1; // Error
            } else {
                data->distance_mm[i] = raw_value; // Real distance measurement
                data->range_status[i] = 0; // OK
            }
        }
        
        // Try to read more data from register 0x0007 to get all 64 zones
        result = vl53l7cx_read_register(dev, 0x0007, raw_data, 64);
        if (result == VL53L7CX_STATUS_OK) {
            printf("VL53L7CX: Successfully read 64 bytes from register 0x0007\n");
            printf("VL53L7CX: Raw data: ");
            for (int i = 0; i < 16; i++) {
                printf("%02X ", raw_data[i]);
            }
            printf("...\n");
            
            // Fill in the remaining zones
            for (int i = 32; i < VL53L7CX_NB_ZONES && i < 64; i++) {
                uint16_t raw_value = (raw_data[(i-32) * 2 + 1] << 8) | raw_data[(i-32) * 2];
                
                // Interpret the raw value
                if (raw_value == 0) {
                    data->distance_mm[i] = 0; // No object detected
                    data->range_status[i] = 0; // OK
                } else if (raw_value > 8000) {
                    data->distance_mm[i] = 0; // Likely an error value
                    data->range_status[i] = 1; // Error
                } else {
                    data->distance_mm[i] = raw_value; // Real distance measurement
                    data->range_status[i] = 0; // OK
                }
            }
        }
        
        data->data_ready = true;
        return VL53L7CX_STATUS_OK;
    }
    
    // Method 2: Try reading from register 0x0007
    result = vl53l7cx_read_register(dev, 0x0007, raw_data, 64);
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Successfully read 64 bytes from register 0x0007\n");
        printf("VL53L7CX: Raw data: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", raw_data[i]);
        }
        printf("...\n");
        
        for (int i = 0; i < VL53L7CX_NB_ZONES && i < 32; i++) {
            data->distance_mm[i] = (raw_data[i * 2 + 1] << 8) | raw_data[i * 2];
            data->range_status[i] = 0;
        }
        data->data_ready = true;
        return VL53L7CX_STATUS_OK;
    }
    
    // Method 3: Try reading from register 0x0008
    result = vl53l7cx_read_register(dev, 0x0008, raw_data, 64);
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Successfully read 64 bytes from register 0x0008\n");
        printf("VL53L7CX: Raw data: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", raw_data[i]);
        }
        printf("...\n");
        
        for (int i = 0; i < VL53L7CX_NB_ZONES && i < 32; i++) {
            data->distance_mm[i] = (raw_data[i * 2 + 1] << 8) | raw_data[i * 2];
            data->range_status[i] = 0;
        }
        data->data_ready = true;
        return VL53L7CX_STATUS_OK;
    }
    
    // Method 4: Try reading from register 0x0009
    result = vl53l7cx_read_register(dev, 0x0009, raw_data, 64);
    if (result == VL53L7CX_STATUS_OK) {
        printf("VL53L7CX: Successfully read 64 bytes from register 0x0009\n");
        printf("VL53L7CX: Raw data: ");
        for (int i = 0; i < 16; i++) {
            printf("%02X ", raw_data[i]);
        }
        printf("...\n");
        
        for (int i = 0; i < VL53L7CX_NB_ZONES && i < 32; i++) {
            data->distance_mm[i] = (raw_data[i * 2 + 1] << 8) | raw_data[i * 2];
            data->range_status[i] = 0;
        }
        data->data_ready = true;
        return VL53L7CX_STATUS_OK;
    }
    
    // If all methods fail, show error
    printf("VL53L7CX: Failed to read data from any register\n");
    printf("VL53L7CX: This suggests the sensor needs proper initialization\n");
    printf("VL53L7CX: or firmware loading to produce distance data\n");
    
    return VL53L7CX_STATUS_ERROR;
}

// Print zone data in a formatted way
void vl53l7cx_print_zone_data(const vl53l7cx_zone_data_t *data) {
    if (!data) return;
    
    printf("\n=== VL53L7CX Zone Distance Data (8x8 grid) ===\n");
    printf("Zone distances in mm:\n");
    
    for (int row = 0; row < 8; row++) {
        printf("Row %d: ", row);
        for (int col = 0; col < 8; col++) {
            int zone = row * 8 + col;
            printf("%4d ", data->distance_mm[zone]);
        }
        printf("\n");
    }
    
    printf("\nZone status (0=OK, 1=Error):\n");
    for (int row = 0; row < 8; row++) {
        printf("Row %d: ", row);
        for (int col = 0; col < 8; col++) {
            int zone = row * 8 + col;
            printf("%4d ", data->range_status[zone]);
        }
        printf("\n");
    }
    printf("===============================================\n\n");
}
