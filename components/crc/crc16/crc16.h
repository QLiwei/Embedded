#ifndef __CRC16_H__
#define __CRC16_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CRC16_IBM_MODEL = 0,
    CRC16_MAXIM_MODEL,
    CRC16_USB_MODEL,
    CRC16_MODBUS_MODEL,
    CRC16_CCITT_MODEL,
    CRC16_CCITT_FALSE_MODEL,
    CRC16_X25_MODEL,
    CRC16_XMODEM_MODEL,
    CRC16_DNP_MODEL,
    CRC16_NONE_MODEL,
}CRC16_reference_model_e;

uint16_t crc16_calculate(CRC16_reference_model_e model, uint8_t *input_data, size_t length);
uint16_t crc16_calculate_mirror_mode(CRC16_reference_model_e model, uint8_t *input_data, size_t length);
void crc16_calculate_package(CRC16_reference_model_e model, uint8_t *input_data, size_t length);
void crc16_lookup_table_package(CRC16_reference_model_e model, uint8_t *input_data, size_t length);
bool crc16_package_check(CRC16_reference_model_e model, uint8_t *input_data, size_t length);
bool crc16_lookup_table_package_check(CRC16_reference_model_e model, uint8_t *input_data, size_t length);

#ifdef __cplusplus
}
#endif

#endif
