/**
 * Pico 2 Platform Layer Implementation for VL53L7CX Driver
 * 
 * This file implements the platform abstraction layer for the VL53L7CX sensor
 * driver, adapted for the Raspberry Pi Pico 2 using the Pico SDK.
 */

#include "platform_pico.h"

/**
 * @brief Read a single byte from VL53L7CX sensor
 * @param p_platform: Pointer to platform structure
 * @param RegisterAdress: Register address to read from
 * @param p_value: Pointer to store the read value
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_RdByte(
        VL53L7CX_Platform *p_platform,
        uint16_t RegisterAdress,
        uint8_t *p_value)
{
    uint8_t status = 0;
    
    if (!p_platform || !p_value) {
        return 255; // Error: invalid parameters
    }
    
    // Prepare register address (16-bit, big-endian)
    uint8_t reg_addr[2] = {(RegisterAdress >> 8) & 0xFF, RegisterAdress & 0xFF};
    
    // Write register address
    int result = i2c_write_blocking(p_platform->i2c_instance, p_platform->address, reg_addr, 2, true);
    if (result != 2) {
        return 255; // Error: failed to write register address
    }
    
    // Read the value
    result = i2c_read_blocking(p_platform->i2c_instance, p_platform->address, p_value, 1, false);
    if (result != 1) {
        return 255; // Error: failed to read value
    }
    
    return status;
}

/**
 * @brief Write a single byte to VL53L7CX sensor
 * @param p_platform: Pointer to platform structure
 * @param RegisterAdress: Register address to write to
 * @param value: Value to write
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_WrByte(
        VL53L7CX_Platform *p_platform,
        uint16_t RegisterAdress,
        uint8_t value)
{
    uint8_t status = 0;
    
    if (!p_platform) {
        return 255; // Error: invalid parameters
    }
    
    // Prepare data buffer (register address + value)
    uint8_t buffer[3] = {(RegisterAdress >> 8) & 0xFF, RegisterAdress & 0xFF, value};
    
    // Write register address and value
    int result = i2c_write_blocking(p_platform->i2c_instance, p_platform->address, buffer, 3, false);
    if (result != 3) {
        return 255; // Error: failed to write
    }
    
    return status;
}

/**
 * @brief Write multiple bytes to VL53L7CX sensor
 * @param p_platform: Pointer to platform structure
 * @param RegisterAdress: Register address to write to
 * @param p_values: Pointer to data to write
 * @param size: Number of bytes to write
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_WrMulti(
        VL53L7CX_Platform *p_platform,
        uint16_t RegisterAdress,
        uint8_t *p_values,
        uint32_t size)
{
    uint8_t status = 0;
    
    if (!p_platform || !p_values || size == 0) {
        return 255; // Error: invalid parameters
    }
    
    // Prepare data buffer (register address + values)
    uint8_t buffer[size + 2];
    buffer[0] = (RegisterAdress >> 8) & 0xFF;
    buffer[1] = RegisterAdress & 0xFF;
    memcpy(&buffer[2], p_values, size);
    
    // Write register address and values
    int result = i2c_write_blocking(p_platform->i2c_instance, p_platform->address, buffer, size + 2, false);
    if (result != (int)(size + 2)) {
        return 255; // Error: failed to write
    }
    
    return status;
}

/**
 * @brief Read multiple bytes from VL53L7CX sensor
 * @param p_platform: Pointer to platform structure
 * @param RegisterAdress: Register address to read from
 * @param p_values: Pointer to store the read values
 * @param size: Number of bytes to read
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_RdMulti(
        VL53L7CX_Platform *p_platform,
        uint16_t RegisterAdress,
        uint8_t *p_values,
        uint32_t size)
{
    uint8_t status = 0;
    
    if (!p_platform || !p_values || size == 0) {
        return 255; // Error: invalid parameters
    }
    
    // Prepare register address (16-bit, big-endian)
    uint8_t reg_addr[2] = {(RegisterAdress >> 8) & 0xFF, RegisterAdress & 0xFF};
    
    // Write register address
    int result = i2c_write_blocking(p_platform->i2c_instance, p_platform->address, reg_addr, 2, true);
    if (result != 2) {
        return 255; // Error: failed to write register address
    }
    
    // Read the values
    result = i2c_read_blocking(p_platform->i2c_instance, p_platform->address, p_values, size, false);
    if (result != (int)size) {
        return 255; // Error: failed to read values
    }
    
    return status;
}

/**
 * @brief Reset the VL53L7CX sensor
 * @param p_platform: Pointer to platform structure
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_Reset_Sensor(
        VL53L7CX_Platform *p_platform)
{
    uint8_t status = 0;
    
    if (!p_platform) {
        return 255; // Error: invalid parameters
    }
    
    /* (Optional) Reset sensor by toggling power pins */
    /* For now, we'll just wait for the sensor to stabilize */
    VL53L7CX_WaitMs(p_platform, 100);
    
    return status;
}

/**
 * @brief Swap buffer bytes (for endianness conversion)
 * @param buffer: Pointer to buffer
 * @param size: Size of buffer
 */
void VL53L7CX_SwapBuffer(
        uint8_t 		*buffer,
        uint16_t 	 	 size)
{
    uint32_t i, tmp;
    
    /* Example of possible implementation using <string.h> */
    for(i = 0; i < size; i = i + 4) 
    {
        tmp = (
          buffer[i]<<24)
        |(buffer[i+1]<<16)
        |(buffer[i+2]<<8)
        |(buffer[i+3]);
        
        memcpy(&(buffer[i]), &tmp, 4);
    }
}

/**
 * @brief Wait for specified number of milliseconds
 * @param p_platform: Pointer to platform structure
 * @param TimeMs: Time to wait in milliseconds
 * @return 0 if OK, non-zero if error
 */
uint8_t VL53L7CX_WaitMs(
        VL53L7CX_Platform *p_platform,
        uint32_t TimeMs)
{
    // Use Pico SDK sleep function
    sleep_ms(TimeMs);
    return 0; // Always successful
}
