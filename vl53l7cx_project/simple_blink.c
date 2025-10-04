#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define LED_PIN 25

int main() {
    // Initialize stdio for USB output
    stdio_init_all();
    
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    printf("Simple Blink Test Starting...\n");
    
    int count = 0;
    while (true) {
        // Blink LED
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
