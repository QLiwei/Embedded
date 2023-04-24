/**
 * @file crc8.c
 * @brief Cyclic Redundancy Check (CRC8)
 * @copyright Copyright (c) 2023
 *
 * Change Logs:
 * Data             Author                          Notes
 * 2023-04-19       vector(vector_qiu@163.com)      first version
 *
 */
#include "crc8.h"
#include <assert.h>

/* 低空间占用可以使用4BIT table查表 */
/* Reference Model:CRC8 */
static const uint8_t CRC8_8BIT_TABLE[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};
/* Reference Model:CRC8_ITU_MODEL */
static const uint8_t CRC8_8BIT_ITU_TABLE[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};
/* Reference Model:CRC8_ROHC_MODEL */
static const uint8_t CRC8_8BIT_ROHC_TABLE[256] = {
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
    0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69, 0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
    0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
    0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
    0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
    0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
    0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
    0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
    0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
    0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
    0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
    0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
    0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
    0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
    0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
    0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};
/* Reference Model:CRC8_MAXIM_MODEL */
static const uint8_t CRC8_8BIT_MAXIM_TABLE[256] = {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35
};

/* http://www.ip33.com/crc.html */
/* polynomial discard MSB or LSB because they are always 1 */
typedef struct {
    uint8_t initial_value;
    uint8_t result_xor_value;
    uint8_t polynomial;
    bool input_inversion;
    bool output_inversion;
} crc8_param_t;

// #define PRINT_TABLE
#ifndef PRINT_TABLE
static crc8_param_t crc8_param[] = {
    {0x00, 0x00, 0x07, false, false},       // Reference Model:CRC8
    {0x00, 0x55, 0x07, false, false},       // Reference Model:CRC8_ITU
    {0xFF, 0x00, 0x07, true, true},         // Reference Model:CRC8_ROHC
    {0x00, 0x00, 0x31, true, true},         // Reference Model:CRC8_MAXIM
};
#else
/* Generate table usage */
static crc8_param_t crc8_param[] = {
    {0x00, 0x00, 0x07, false, false},       // Reference Model:CRC8
    {0x00, 0x00, 0x07, false, false},       // Reference Model:CRC8_ITU
    {0x00, 0x00, 0x07, true, true},         // Reference Model:CRC8_ROHC
    {0x00, 0x00, 0x31, true, true},         // Reference Model:CRC8_MAXIM
};
#endif
/**
 * @brief uint8_t type data inversion
 *
 * @param data Data to be reversed
 * @return uint8_t The reversed data
 */
static uint8_t data_inversion(uint8_t data) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result <<= 1;
        result |= (data >> i) & 0x01;
    }
    return result;
}

/**
 * @brief Calculates the 8 bit wide CRC of an input data of a given length.
 *
 * @param param CRC8 reference model
 * @param input_data Input uint8 t type array data
 * @param length Input uint8 t type array length
 * @return uint8_t CRC8 result
 */
uint8_t crc8_calculate(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8 = crc8_param[model].initial_value;
    uint8_t polynomial = crc8_param[model].polynomial;
    uint8_t data;

    for (size_t i = 0; i < length; i++) {
        data = *input_data++;
        if (crc8_param[model].input_inversion) {
            data = data_inversion(data);
        }
        crc8 ^= data;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc8 & 0x80) {
                crc8 <<= 1;
                crc8 ^= polynomial; //  polynomial discard MSB or LSB because they are always 1, don't need to compute
            } else {
                crc8 <<= 1;
            }

        }
    }

    if (crc8_param[model].output_inversion) {
        crc8 = data_inversion(crc8);
    }

    return (crc8 ^ crc8_param[model].result_xor_value);
}

/**
 * @brief The mirror mode computes the 8-bit wide CRC of the input data of a given length.
 *
 * @param param CRC8 reference model
 * @param input_data Input uint8 t type array data
 * @param length Input uint8 t type array length
 * @return uint8_t CRC8 result
 */
uint8_t crc8_calculate_mirror_mode(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8 = crc8_param[model].initial_value;
    uint8_t polynomial = data_inversion(crc8_param[model].polynomial);
    uint8_t data;

    for (size_t i = 0; i < length; i++) {
        data = *input_data++;
        if (!crc8_param[model].input_inversion) {
            data = data_inversion(data);
        }
        crc8 ^= data;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc8 & 0x0001) {
                crc8 >>= 1;
                crc8 ^= polynomial; //  polynomial discard MSB or LSB because they are always 1, don't need to compute
            } else {
                crc8 >>= 1;
            }

        }
    }

    if (!crc8_param[model].output_inversion) {
        crc8 = data_inversion(crc8);
    }

    return (crc8 ^ crc8_param[model].result_xor_value);
}

/**
 * @brief CRC8 calculation method group package
 *
 * @param param CRC8 reference model
 * @param input_data Packets of CRC8 to be computed, last byte of the array holds the CRC8 result
 * @param length Length of the CRC8 packet
 */
void crc8_calculate_package(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8_result = 0;
    crc8_result = crc8_calculate(model, input_data, length - 1);
    *(input_data + length - 1) = crc8_result & 0xFF;
}

/**
 * @brief CRC8 lookup table package
 *
 * @param param CRC8 reference model
 * @param input_data Packets of CRC8 to be computed, last byte of the array holds the CRC8 result
 * @param length Length of the CRC8 packet
 */
void crc8_lookup_table_package(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8_result = 0;
    uint8_t *p = input_data;
    if (p != NULL) {
        switch (model)
        {
        case CRC8_MODEL:
            crc8_result = crc8_param[model].initial_value;
            for (size_t i = 0; i < length - 1; i++) {
                crc8_result = CRC8_8BIT_TABLE[(crc8_result ^ (*p++)) & 0xFF];
            }
            crc8_result ^= crc8_param[model].result_xor_value;
            break;

        case CRC8_ITU_MODEL:
            crc8_result = crc8_param[model].initial_value;
            for (size_t i = 0; i < length - 1; i++) {
                crc8_result = CRC8_8BIT_ITU_TABLE[(crc8_result ^ (*p++)) & 0xFF];
            }
            crc8_result ^= crc8_param[model].result_xor_value;
            break;

        case CRC8_ROHC_MODEL:
            crc8_result = crc8_param[model].initial_value;
            for (size_t i = 0; i < length - 1; i++) {
                crc8_result = CRC8_8BIT_ROHC_TABLE[(crc8_result ^ (*p++)) & 0xFF];
            }
            crc8_result ^= crc8_param[model].result_xor_value;
            break;

        case CRC8_MAXIM_MODEL:
            crc8_result = crc8_param[model].initial_value;
            for (size_t i = 0; i < length - 1; i++) {
                crc8_result = CRC8_8BIT_MAXIM_TABLE[(crc8_result ^ (*p++)) & 0xFF];
            }
            crc8_result ^= crc8_param[model].result_xor_value;
            break;

        default:
            break;
        }
    }

    *(input_data + length - 1) = crc8_result & 0xFF;
}

/**
 * @brief CRC8 calculation method package check
 *
 * @param param CRC8 reference model
 * @param input_data CRC8 packages to check
 * @param length Length of the CRC8 packet
 * @return true CRC8 checks succeed
 * @return false CRC8 checks fails
 */
bool crc8_package_check(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8_result = 0;
    crc8_result = crc8_calculate(model, input_data, length - 1);
    if (crc8_result != *(input_data + length - 1)) {
        return false;
    }
    return true;
}

/**
 * @brief CRC8 lookup table packages check
 *
 * @param param CRC8 reference model
 * @param input_data CRC8 packages to check
 * @param length Length of the CRC8 packet
 * @return true CRC8 checks succeed
 * @return false CRC8 checks fails
 */
bool crc8_lookup_table_package_check(CRC8_reference_model_e model, uint8_t *input_data, size_t length) {
    assert(model < CRC8_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint8_t crc8_result = 0;
    uint8_t *p = input_data;
    switch (model)
    {
    case CRC8_MODEL:
        crc8_result = crc8_param[model].initial_value;
        for (size_t i = 0; i < length - 1; i++) {
            crc8_result = CRC8_8BIT_TABLE[(crc8_result ^ (*p++)) & 0xFF];
        }
        crc8_result ^= crc8_param[model].result_xor_value;
        break;

    case CRC8_ITU_MODEL:
        crc8_result = crc8_param[model].initial_value;
        for (size_t i = 0; i < length - 1; i++) {
            crc8_result = CRC8_8BIT_ITU_TABLE[(crc8_result ^ (*p++)) & 0xFF];
        }
        crc8_result ^= crc8_param[model].result_xor_value;
        break;

    case CRC8_ROHC_MODEL:
        crc8_result = crc8_param[model].initial_value;
        for (size_t i = 0; i < length - 1; i++) {
            crc8_result = CRC8_8BIT_ROHC_TABLE[(crc8_result ^ (*p++)) & 0xFF];
        }
        crc8_result ^= crc8_param[model].result_xor_value;
        break;

    case CRC8_MAXIM_MODEL:
        crc8_result = crc8_param[model].initial_value;
        for (size_t i = 0; i < length - 1; i++) {
            crc8_result = CRC8_8BIT_MAXIM_TABLE[(crc8_result ^ (*p++)) & 0xFF];
        }
        crc8_result ^= crc8_param[model].result_xor_value;
        break;

    default:
        break;
    }

    if (crc8_result != *(input_data + length - 1)) {
        return false;
    }
    return true;
}

#define TEST
#ifdef TEST
#include <stdio.h>
static void print_crc8_table(CRC8_reference_model_e model);

int main() {
/* test: crc8_calculate  */
#if 0
    uint8_t crc8_result;
    uint8_t input_data0[8] = {0x03, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; // crc8 result: 0x3e

    crc8_result  = crc8_calculate(CRC8_MODEL, input_data0, 1);
    printf("CRC8_MODEL crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate_mirror_mode(CRC8_MODEL, input_data0, 1);
    printf("CRC8_MODEL mirror mode crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate(CRC8_ITU_MODEL, input_data0, 1);
    printf("CRC8_ITU_MODEL crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate_mirror_mode(CRC8_ITU_MODEL, input_data0, 1);
    printf("CRC8_ITU_MODEL mirror mode crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate(CRC8_ROHC_MODEL, input_data0, 1);
    printf("CRC8_ROHC_MODEL crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate_mirror_mode(CRC8_ROHC_MODEL, input_data0, 1);
    printf("CRC8_ROHC_MODEL mirror mode crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate(CRC8_MAXIM_MODEL, input_data0, 1);
    printf("CRC8_MAXIM_MODEL crc8_result: 0x%x\n", crc8_result);

    crc8_result  = crc8_calculate_mirror_mode(CRC8_MAXIM_MODEL, input_data0, 1);
    printf("CRC8_MAXIM_MODEL mirror mode crc8_result: 0x%x\n", crc8_result);
#endif

#if 0 /* printf crc8 8bit table */
    print_crc8_table(CRC8_MODEL);
    print_crc8_table(CRC8_ITU_MODEL);
    print_crc8_table(CRC8_ROHC_MODEL);
    print_crc8_table(CRC8_MAXIM_MODEL);
#endif

/* test crc8_calculate_package crc8_package_check */
#if 0
    printf("---------CRC8 calculation method-----------\n");
    bool check_result;
    uint8_t input_data[9] = {0x01, 0x02, 0x03, 0x05, 0x08, 0x07, 0x06, 0x05};

    crc8_calculate_package(CRC8_MODEL, input_data, 9);
    printf("CRC8_MODE result: 0x%02x \n", input_data[8]);
    check_result = crc8_package_check(CRC8_MODEL, input_data, 9);
    if (check_result) {
        printf("CRC8_MODEL check succeed!\n");
    } else {
        printf("CRC8_MODEL check fails!\n");
    }

    crc8_calculate_package(CRC8_ITU_MODEL, input_data, 9);
    printf("CRC8_ITU_MODEL result: 0x%02x \n", input_data[8]);
    check_result = crc8_package_check(CRC8_ITU_MODEL, input_data, 9);
    if (check_result) {
        printf("CRC8_ITU_MODEL check succeed!\n");
    } else {
        printf("CRC8_ITU_MODEL check fails!\n");
    }

    crc8_calculate_package(CRC8_ROHC_MODEL, input_data, 9);
    printf("CRC8_ROHC_MODEL result: 0x%02x \n", input_data[8]);
    check_result = crc8_package_check(CRC8_ROHC_MODEL, input_data, 9);
    if (check_result) {
        printf("CRC8_ROHC_MODEL check succeed!\n");
    } else {
        printf("CRC8_ROHC_MODEL check fails!\n");
    }

    crc8_calculate_package(CRC8_MAXIM_MODEL, input_data, 9);
    printf("CRC8_MAXIM_MODEL result: 0x%02x \n", input_data[8]);
    check_result = crc8_package_check(CRC8_MAXIM_MODEL, input_data, 9);
    if (check_result) {
        printf("CRC8_MAXIM_MODEL check succeed!\n");
    } else {
        printf("CRC8_MAXIM_MODEL check fails!\n");
    }
#endif

#if 0
    printf("---------CRC8 lookup table-----------\n");
    bool check_result2;
    uint8_t input_data2[9] = {0x01, 0x02, 0x03, 0x05, 0x08, 0x07, 0x06, 0x05};

    crc8_lookup_table_package(CRC8_MODEL, input_data2, 9);
    printf("CRC8_MODEL result: 0x%02x \n", input_data2[8]);
    check_result2 = crc8_lookup_table_package_check(CRC8_MODEL, input_data2, 9);
    if (check_result2) {
        printf("CRC8_MODEL check succeed!\n");
    } else {
        printf("CRC8_MODEL check fails!\n");
    }

    crc8_lookup_table_package(CRC8_ITU_MODEL, input_data2, 9);
    printf("CRC8_ITU_MODEL result: 0x%02x \n", input_data2[8]);
    check_result2 = crc8_lookup_table_package_check(CRC8_ITU_MODEL, input_data2, 9);
    if (check_result2) {
        printf("CRC8_ITU_MODEL check succeed!\n");
    } else {
        printf("CRC8_ITU_MODEL check fails!\n");
    }

    crc8_lookup_table_package(CRC8_ROHC_MODEL, input_data2, 9);
    printf("CRC8_ROHC_MODEL result: 0x%02x \n", input_data2[8]);
    check_result2 = crc8_lookup_table_package_check(CRC8_ROHC_MODEL, input_data2, 9);
    if (check_result2) {
        printf("CRC8_ROHC_MODEL check succeed!\n");
    } else {
        printf("CRC8_ROHC_MODEL check fails!\n");
    }

    crc8_lookup_table_package(CRC8_MAXIM_MODEL, input_data2, 9);
    printf("CRC8_MAXIM_MODEL result: 0x%02x \n", input_data2[8]);
    check_result2 = crc8_lookup_table_package_check(CRC8_MAXIM_MODEL, input_data2, 9);
    if (check_result2) {
        printf("CRC8_MAXIM_MODEL check succeed!\n");
    } else {
        printf("CRC8_MAXIM_MODEL check fails!\n");
    }
#endif
    return 0;
}

static void print_crc8_table(CRC8_reference_model_e model) {
    uint8_t crc8_result = 0;
    uint8_t i = 0;
    switch (model)
    {
    case CRC8_MODEL:
        printf("/* Reference Model:CRC8 */\n");
        printf("static const uint8_t CRC8_8BIT_TABLE[256] = {\n");
        break;

    case CRC8_ITU_MODEL:
        printf("/* Reference Model:CRC8_ITU_MODEL */\n");
        printf("static const uint8_t CRC8_8BIT_ITU_TABLE[256] = {\n");
        break;

    case CRC8_ROHC_MODEL:
        printf("/* Reference Model:CRC8_ROHC_MODEL */\n");
        printf("static const uint8_t CRC8_8BIT_ROHC_TABLE[256] = {\n");
        break;

    case CRC8_MAXIM_MODEL:
        printf("/* Reference Model:CRC8_MAXIM_MODEL */\n");
        printf("static const uint8_t CRC8_8BIT_MAXIM_TABLE[256] = {\n");
        break;

    default:
        break;
    }
    for (int j = 0; j <= 0xFF; j++, i++) {
        crc8_result = crc8_calculate(model, &i, 1);
        if (i % 16 == 0) {
            printf("    ");
        } else {
            printf(" ");
        }
        printf("0x%02X", crc8_result);
        if (i != 0xFF) {
            printf(",");
        }
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("};\n");
}
#endif
