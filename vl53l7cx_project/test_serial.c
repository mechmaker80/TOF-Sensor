#include "pico/stdlib.h"
#include <stdio.h>

int main() {
    stdio_init_all();
    
    printf("Hello from Pico 2!\n");
    printf("USB Serial is working!\n");
    
    int count = 0;
    while (true) {
        printf("Count: %d\n", count++);
        sleep_ms(1000);
    }
    
    return 0;
}
