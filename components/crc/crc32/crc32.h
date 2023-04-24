#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CRC32_MODEL = 0,
    CRC32_MPEG2_MODEL,
    CRC32_NONE_MODEL,
}CRC32_reference_model_e;

uint32_t crc32_calculate(CRC32_reference_model_e model, uint8_t *input_data, size_t length);
uint32_t crc32_calculate_mirror_mode(CRC32_reference_model_e model, uint8_t *input_data, size_t length);
void crc32_calculate_package(CRC32_reference_model_e model, uint8_t *input_data, size_t length);
void crc32_lookup_table_package(CRC32_reference_model_e model, uint8_t *input_data, size_t length);
bool crc32_package_check(CRC32_reference_model_e model, uint8_t *input_data, size_t length);
bool crc32_lookup_table_package_check(CRC32_reference_model_e model, uint8_t *input_data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* __CRC32_H__ */
