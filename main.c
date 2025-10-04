#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>

// Pin definitions for Pico 2
#define LED_PIN 25  // Built-in LED on Pico 2

int main() {
    // Initialize stdio for USB output
    stdio_init_all();
    
    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    printf("Pico 2 Blink Example Started!\n");
    
    while (true) {
        // Turn LED on
        gpio_put(LED_PIN, 1);
        printf("LED ON\n");
        sleep_ms(100);
        
        // Turn LED off
        gpio_put(LED_PIN, 0);
        printf("LED OFF\n");
        sleep_ms(100);
    }
    
    return 0;
}
