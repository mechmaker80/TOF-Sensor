#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
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
    
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);  // Turn on LED to indicate startup
    
    printf("VL53L7CX Debug Version Starting...\n");
    printf("====================================\n");
    
    // Wait a moment for USB to be ready
    sleep_ms(1000);
    
    printf("Step 1: Initializing I2C...\n");
    
    // Initialize I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    
    printf("Step 2: I2C initialized on pins SDA=%d, SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    
    // Test I2C communication
    printf("Step 3: Testing I2C communication...\n");
    
    uint8_t test_data;
    int result = i2c_read_blocking(I2C_PORT, 0x29, &test_data, 1, false);
    
    if (result == 1) {
        printf("Step 4: I2C communication successful! Sensor found at address 0x29\n");
        printf("Sensor data: 0x%02X\n", test_data);
    } else {
        printf("Step 4: I2C communication failed (result=%d)\n", result);
        printf("This is expected if no VL53L7CX sensor is connected\n");
    }
    
    printf("Step 5: Entering main loop...\n");
    printf("LED will blink every second\n");
    printf("Press Ctrl+C to stop\n\n");
    
    // Turn off LED to indicate successful initialization
    gpio_put(LED_PIN, 0);
    
    int count = 0;
    while (true) {
        // Blink LED and print status
        gpio_put(LED_PIN, 1);
        printf("LED ON - Count: %d\n", count);
        sleep_ms(500);
        
        gpio_put(LED_PIN, 0);
        printf("LED OFF - Count: %d\n", count);
        sleep_ms(500);
        
        count++;
    }
    
    return 0;
}
