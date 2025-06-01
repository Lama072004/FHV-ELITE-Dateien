#include "sensor_read_write.h"
#include "sensor_init.h"
#include <stdint.h>

#define REG_FIFO_DATA      0x07

void max30102_read_fifo(uint32_t *red, uint32_t *ir) {
    uint8_t raw[6];
    read_register(REG_FIFO_DATA, raw, 6);
    *red = ((raw[0] << 16) | (raw[1] << 8) | raw[2]) & 0x03FFFF;
    *ir  = ((raw[3] << 16) | (raw[4] << 8) | raw[5]) & 0x03FFFF;
}
