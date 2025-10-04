/**
 * VL53L7CX ST Driver Example for Pico 2
 * 
 * This example uses the official STMicroelectronics VL53L7CX driver
 * adapted for the Raspberry Pi Pico 2.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "vl53l7cx_api.h"

// I2C Configuration for Pico 2
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
    
    printf("VL53L7CX ST Driver Example for Pico 2\n");
    printf("=====================================\n");
    
    // Initialize I2C
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    
    printf("I2C initialized on pins SDA=%d, SCL=%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    
    /*********************************/
    /*   VL53L7CX ranging variables  */
    /*********************************/
    
    uint8_t 				status, loop, isAlive, isReady, i;
    VL53L7CX_Configuration 	Dev;			/* Sensor configuration */
    VL53L7CX_ResultsData 	Results;		/* Results data from VL53L7CX */
    
    /*********************************/
    /*      Customer platform        */
    /*********************************/
    
    /* Fill the platform structure with Pico 2 implementation */
    Dev.platform.address = 0x29;  // Use the correct I2C address for your sensor
    Dev.platform.i2c_instance = I2C_PORT;
    Dev.platform.sda_pin = I2C_SDA_PIN;
    Dev.platform.scl_pin = I2C_SCL_PIN;
    
    /* (Optional) Reset sensor toggling PINs (see platform, not in API) */
    //VL53L7CX_Reset_Sensor(&(Dev.platform));
    
    /*********************************/
    /*   Power on sensor and init    */
    /*********************************/
    
    /* (Optional) Check if there is a VL53L7CX sensor connected */
    printf("Checking if VL53L7CX sensor is alive...\n");
    status = vl53l7cx_is_alive(&Dev, &isAlive);
    if(!isAlive || status)
    {
        printf("VL53L7CX not detected at requested address 0x%02X\n", VL53L7CX_DEFAULT_I2C_ADDRESS);
        printf("Please check:\n");
        printf("1. Sensor is connected to I2C pins %d (SDA) and %d (SCL)\n", I2C_SDA_PIN, I2C_SCL_PIN);
        printf("2. Sensor is powered (3.3V)\n");
        printf("3. I2C address is correct (0x%02X)\n", VL53L7CX_DEFAULT_I2C_ADDRESS);
        printf("4. Pull-up resistors are present on I2C lines\n");
        
        // Blink LED to indicate error
        while (true) {
            gpio_put(LED_PIN, 0);
            sleep_ms(100);
            gpio_put(LED_PIN, 1);
            sleep_ms(100);
        }
    }
    
    printf("VL53L7CX sensor detected!\n");
    
    /* (Mandatory) Init VL53L7CX sensor */
    printf("Initializing VL53L7CX sensor...\n");
    status = vl53l7cx_init(&Dev);
    if(status)
    {
        printf("VL53L7CX ULD Loading failed (status: %d)\n", status);
        while (true) {
            gpio_put(LED_PIN, 0);
            sleep_ms(500);
            gpio_put(LED_PIN, 1);
            sleep_ms(500);
        }
    }
    
    printf("VL53L7CX ULD ready ! (Version : %s)\n", VL53L7CX_API_REVISION);
    
    // Set sensor to 8x8 mode for full resolution
    printf("Setting sensor to 8x8 mode...\n");
    status = vl53l7cx_set_resolution(&Dev, VL53L7CX_RESOLUTION_8X8);
    if(status) {
        printf("Failed to set 8x8 resolution (status: %d)\n", status);
    } else {
        printf("Sensor set to 8x8 mode successfully\n");
    }
    
    // Turn off LED to indicate successful initialization
    gpio_put(LED_PIN, 0);
    
    /*********************************/
    /*         Ranging loop          */
    /*********************************/
    
    printf("Starting ranging...\n");
    status = vl53l7cx_start_ranging(&Dev);
    if(status) {
        printf("Failed to start ranging (status: %d)\n", status);
        while (true) {
            gpio_put(LED_PIN, 0);
            sleep_ms(500);
            gpio_put(LED_PIN, 1);
            sleep_ms(500);
        }
    }
    
    printf("Ranging started successfully!\n");
    printf("Reading distance data from all zones (8x8 grid)...\n");
    printf("Press Ctrl+C to stop\n\n");
    
    loop = 0;
    while(loop < 100)  // Read 100 measurements
    {
        /* Use polling function to know when a new measurement is ready */
        status = vl53l7cx_check_data_ready(&Dev, &isReady);
        
        if(isReady)
        {
            vl53l7cx_get_ranging_data(&Dev, &Results);
            
            /* Print data for all 64 zones (8x8 mode) */
            printf("Measurement #%3u:\n", Dev.streamcount);
            printf("=== VL53L7CX Zone Distance Data (8x8 grid) ===\n");
            printf("Zone distances in mm:\n");
            
            for(int row = 0; row < 8; row++) {
                printf("Row %d: ", row);
                for(int col = 0; col < 8; col++) {
                    int zone = row * 8 + col;
                    printf("%4d ", Results.distance_mm[VL53L7CX_NB_TARGET_PER_ZONE * zone]);
                }
                printf("\n");
            }
            
            printf("\nZone status (0=OK, 1=Error):\n");
            for(int row = 0; row < 8; row++) {
                printf("Row %d: ", row);
                for(int col = 0; col < 8; col++) {
                    int zone = row * 8 + col;
                    printf("%4d ", Results.target_status[VL53L7CX_NB_TARGET_PER_ZONE * zone]);
                }
                printf("\n");
            }
            printf("===============================================\n\n");
            
            // Brief LED flash to indicate successful reading
            gpio_put(LED_PIN, 1);
            sleep_ms(50);
            gpio_put(LED_PIN, 0);
            
            loop++;
        }
        
        /* Wait a few ms to avoid too high polling */
        VL53L7CX_WaitMs(&(Dev.platform), 10);
    }
    
    status = vl53l7cx_stop_ranging(&Dev);
    printf("End of VL53L7CX demo\n");
    
    // Turn on LED to indicate completion
    gpio_put(LED_PIN, 1);
    
    return 0;
}
