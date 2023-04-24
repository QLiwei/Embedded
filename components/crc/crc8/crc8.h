#ifndef __CRC8_H__
#define __CRC8_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CRC8_MODEL = 0,
    CRC8_ITU_MODEL,
    CRC8_ROHC_MODEL,
    CRC8_MAXIM_MODEL,
    CRC8_NONE_MODEL,
}CRC8_reference_model_e;

uint8_t crc8_calculate(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
uint8_t crc8_calculate_mirror_mode(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
void crc8_calculate_package(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
void crc8_lookup_table_package(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
bool crc8_package_check(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
bool crc8_lookup_table_package_check(CRC8_reference_model_e model, uint8_t *input_data, size_t length);

#ifdef __cplusplus
}
#endif

#endif
