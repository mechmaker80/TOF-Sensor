#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "vl53l7cx_driver.h"
#include <stdio.h>
#include <stdbool.h>

// I2C Configuration
#define I2C_PORT i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5
#define I2C_FREQ 400000  // 400 kHz

// LED pin for status indication
#define LED_PIN 25

int main() {
    // Initialize stdio for USB output
    stdio_init_all();
    
    // Wait for USB serial to be ready
    sleep_ms(2000);
    
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);  // Turn on LED to indicate startup
    
    printf("VL53L7CX Multizone ToF Sensor Driver\n");
    printf("====================================\n");
    
    // Initialize I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    
    printf("I2C initialized on pins SDA=%d, SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    
    // Initialize VL53L7CX sensor
    vl53l7cx_device_t sensor;
    int result = vl53l7cx_init(&sensor, I2C_PORT, VL53L7CX_I2C_ADDR);
    
    if (result != VL53L7CX_STATUS_OK) {
        printf("ERROR: Failed to initialize VL53L7CX sensor\n");
        printf("Please check:\n");
        printf("1. Sensor is connected to I2C pins %d (SDA) and %d (SCL)\n", I2C_SDA_PIN, I2C_SCL_PIN);
        printf("2. Sensor is powered (3.3V)\n");
        printf("3. I2C address is correct (0x%02X)\n", VL53L7CX_I2C_ADDR);
        printf("4. Pull-up resistors are present on I2C lines\n");
        
        // Blink LED to indicate error
        while (true) {
            gpio_put(LED_PIN, 0);
            sleep_ms(100);
            gpio_put(LED_PIN, 1);
            sleep_ms(100);
        }
    }
    
    // Start ranging
    result = vl53l7cx_start_ranging(&sensor);
    if (result != VL53L7CX_STATUS_OK) {
        printf("ERROR: Failed to start ranging\n");
        while (true) {
            gpio_put(LED_PIN, 0);
            sleep_ms(500);
            gpio_put(LED_PIN, 1);
            sleep_ms(500);
        }
    }
    
    printf("VL53L7CX sensor started successfully!\n");
    printf("Reading distance data from all 64 zones (8x8 grid)...\n");
    printf("Press Ctrl+C to stop\n\n");
    
    // Turn off LED to indicate successful initialization
    gpio_put(LED_PIN, 0);
    
    vl53l7cx_zone_data_t zone_data;
    uint32_t measurement_count = 0;
    
    while (true) {
        // Check if data is ready
        bool data_ready = false;
        result = vl53l7cx_get_data_ready(&sensor, &data_ready);
        
        if (result == VL53L7CX_STATUS_OK && data_ready) {
            // Get zone data
            result = vl53l7cx_get_zone_data(&sensor, &zone_data);
            
            if (result == VL53L7CX_STATUS_OK) {
                measurement_count++;
                printf("Measurement #%lu:\n", measurement_count);
                vl53l7cx_print_zone_data(&zone_data);
                
                // Clear interrupt for next measurement
                vl53l7cx_clear_interrupt(&sensor);
                
                // Brief LED flash to indicate successful reading
                gpio_put(LED_PIN, 1);
                sleep_ms(50);
                gpio_put(LED_PIN, 0);
            } else {
                printf("ERROR: Failed to read zone data\n");
            }
        } else if (result != VL53L7CX_STATUS_OK) {
            printf("ERROR: Failed to check data ready status\n");
        }
        
        // Print status every 100 iterations (about 1 second)
        static uint32_t status_counter = 0;
        status_counter++;
        if (status_counter % 100 == 0) {
            printf("Status: Waiting for data ready... (checked %lu times)\n", status_counter);
        }
        
        // Small delay to prevent excessive CPU usage
        sleep_ms(10);
    }
    
    return 0;
}
