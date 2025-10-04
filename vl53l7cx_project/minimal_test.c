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
    
    // Wait a long time for USB to be ready
    sleep_ms(2000);
    
    // Turn on LED
    gpio_put(LED_PIN, 1);
    
    // Try to print immediately
    printf("MINIMAL TEST STARTING\n");
    printf("====================\n");
    
    int count = 0;
    while (true) {
        printf("Count: %d\n", count++);
        gpio_put(LED_PIN, count % 2);  // Toggle LED
        sleep_ms(1000);
    }
    
    return 0;
}
