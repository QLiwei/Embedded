#ifndef __CRC8_H__
#define __CRC8_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Reference Model  */
#define CRC8        {0x00, 0x00, 0x07, false, false}
#define CRC8_ITU    {0x00, 0x55, 0x07, false, false}
#define CRC8_ROHC   {0xFF, 0x00, 0x07, true, true}
#define CRC8_MAXIM  {0x00, 0x00, 0x31, true, true}

typedef enum {
    CRC8_MODEL,
    CRC8_ITU_MODEL,
    CRC8_ROHC_MODEL,
    CRC8_MAXIM_MODEL,
}CRC8_reference_model_e;

/* http://www.ip33.com/crc.html */
/* polynomial discard MSB or LSB because they are always 1 */
typedef struct {
    uint8_t initial_value;
    uint8_t result_xor_value;
    uint8_t polynomial;
    bool input_inversion;
    bool output_inversion;
} crc8_param_t;

uint8_t crc8_calculate(const crc8_param_t *param, uint8_t *input_data, size_t length);
void crc8_calculate_package(const crc8_param_t *param, uint8_t *input_data, size_t length);
void crc8_lookup_table_package(CRC8_reference_model_e model, uint8_t *input_data, size_t length);
bool crc8_package_check(const crc8_param_t *param, uint8_t *input_data, size_t length);
bool crc8_lookup_table_package_check(CRC8_reference_model_e model, uint8_t *input_data, size_t length);

#ifdef __cplusplus
}
#endif

#endif
