#ifndef VL53L7CX_DRIVER_H
#define VL53L7CX_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdint.h>
#include <stdbool.h>

// VL53L7CX I2C Address
#define VL53L7CX_I2C_ADDR 0x29

// VL53L7CX Register Addresses
#define VL53L7CX_REG_SYSTEM__INTERRUPT_CLEAR 0x0086
#define VL53L7CX_REG_SYSTEM__MODE_START 0x0087
#define VL53L7CX_REG_SYSTEM__FRESH_OUT_OF_RESET 0x0089
#define VL53L7CX_REG_SYSTEM__INTERRUPT_STATUS 0x0088
#define VL53L7CX_REG_SYSTEM__RESULT__INTERRUPT_STATUS 0x0089
#define VL53L7CX_REG_SYSTEM__RESULT__RANGE_STATUS 0x008A
#define VL53L7CX_REG_SYSTEM__RESULT__SPAD_NB_DETECTED 0x008B
#define VL53L7CX_REG_SYSTEM__RESULT__SIGNAL_RATE 0x008C
#define VL53L7CX_REG_SYSTEM__RESULT__AMBIENT_RATE 0x008D
#define VL53L7CX_REG_SYSTEM__RESULT__SIGMA 0x008E
#define VL53L7CX_REG_SYSTEM__RESULT__DISTANCE 0x008F
#define VL53L7CX_REG_SYSTEM__RESULT__REFLECTANCE 0x0090
#define VL53L7CX_REG_SYSTEM__RESULT__AMBIENT_RATE_KCPS 0x0091
#define VL53L7CX_REG_SYSTEM__RESULT__DISTANCE_MM 0x0092

// VL53L7CX Configuration
#define VL53L7CX_NB_ZONES 64  // 8x8 zones
#define VL53L7CX_MAX_DISTANCE_MM 4000
#define VL53L7CX_TIMEOUT_MS 1000

// VL53L7CX Status codes
#define VL53L7CX_STATUS_OK 0
#define VL53L7CX_STATUS_ERROR -1
#define VL53L7CX_STATUS_TIMEOUT -2

// VL53L7CX Zone data structure
typedef struct {
    uint16_t distance_mm[VL53L7CX_NB_ZONES];
    uint16_t signal_rate[VL53L7CX_NB_ZONES];
    uint16_t ambient_rate[VL53L7CX_NB_ZONES];
    uint16_t sigma[VL53L7CX_NB_ZONES];
    uint8_t range_status[VL53L7CX_NB_ZONES];
    bool data_ready;
} vl53l7cx_zone_data_t;

// VL53L7CX Device structure
typedef struct {
    i2c_inst_t *i2c;
    uint8_t address;
    bool initialized;
} vl53l7cx_device_t;

// Function prototypes
int vl53l7cx_init(vl53l7cx_device_t *dev, i2c_inst_t *i2c, uint8_t address);
int vl53l7cx_start_ranging(vl53l7cx_device_t *dev);
int vl53l7cx_stop_ranging(vl53l7cx_device_t *dev);
int vl53l7cx_get_data_ready(vl53l7cx_device_t *dev, bool *ready);
int vl53l7cx_get_zone_data(vl53l7cx_device_t *dev, vl53l7cx_zone_data_t *data);
int vl53l7cx_clear_interrupt(vl53l7cx_device_t *dev);
void vl53l7cx_print_zone_data(const vl53l7cx_zone_data_t *data);

#endif // VL53L7CX_DRIVER_H
