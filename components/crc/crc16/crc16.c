/**
 * @file crc16.c
 * @brief Cyclic Redundancy Check (CRC16)
 * @copyright Copyright (c) 2023
 *
 * Change Logs:
 * Data             Author                          Notes
 * 2023-04-23       vector(vector_qiu@163.com)      first version
 *
 */
#include "crc16.h"
#include <assert.h>

static const uint16_t CRC16_IBM_MODEL_TABLE[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
/* Reference Model:CRC16_MAXIM_MODEL */
static const uint16_t CRC16_MAXIM_MODEL_TABLE[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
/* Reference Model:CRC16_USB_MODEL */
static const uint16_t CRC16_USB_MODEL_TABLE[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
/* Reference Model:CRC16_MODBUS_MODEL */
const uint16_t CRC16_MODBUS_MODEL_TABLE[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
/* Reference Model:CRC16_CCITT_MODEL */
static const uint16_t CRC16_CCITT_MODEL_TABLE[256] = {
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB, 0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232, 0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};
/* Reference Model:CRC16_CCITT_FALSE_MODEL */
static const uint16_t CRC16_CCITT_FALSE_MODEL_TABLE[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
/* Reference Model:CRC16_X25_MODEL */
static const uint16_t CRC16_X25_MODEL_TABLE[256] = {
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB, 0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232, 0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};
/* Reference Model:CRC16_XMODEM_MODEL */
static const uint16_t CRC16_XMODEM_MODEL_TABLE[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
/* Reference Model:CRC16_DNP_MODEL */
static const uint16_t CRC16_DNP_MODEL_TABLE[256] = {
    0x0000, 0x365E, 0x6CBC, 0x5AE2, 0xD978, 0xEF26, 0xB5C4, 0x839A, 0xFF89, 0xC9D7, 0x9335, 0xA56B, 0x26F1, 0x10AF, 0x4A4D, 0x7C13,
    0xB26B, 0x8435, 0xDED7, 0xE889, 0x6B13, 0x5D4D, 0x07AF, 0x31F1, 0x4DE2, 0x7BBC, 0x215E, 0x1700, 0x949A, 0xA2C4, 0xF826, 0xCE78,
    0x29AF, 0x1FF1, 0x4513, 0x734D, 0xF0D7, 0xC689, 0x9C6B, 0xAA35, 0xD626, 0xE078, 0xBA9A, 0x8CC4, 0x0F5E, 0x3900, 0x63E2, 0x55BC,
    0x9BC4, 0xAD9A, 0xF778, 0xC126, 0x42BC, 0x74E2, 0x2E00, 0x185E, 0x644D, 0x5213, 0x08F1, 0x3EAF, 0xBD35, 0x8B6B, 0xD189, 0xE7D7,
    0x535E, 0x6500, 0x3FE2, 0x09BC, 0x8A26, 0xBC78, 0xE69A, 0xD0C4, 0xACD7, 0x9A89, 0xC06B, 0xF635, 0x75AF, 0x43F1, 0x1913, 0x2F4D,
    0xE135, 0xD76B, 0x8D89, 0xBBD7, 0x384D, 0x0E13, 0x54F1, 0x62AF, 0x1EBC, 0x28E2, 0x7200, 0x445E, 0xC7C4, 0xF19A, 0xAB78, 0x9D26,
    0x7AF1, 0x4CAF, 0x164D, 0x2013, 0xA389, 0x95D7, 0xCF35, 0xF96B, 0x8578, 0xB326, 0xE9C4, 0xDF9A, 0x5C00, 0x6A5E, 0x30BC, 0x06E2,
    0xC89A, 0xFEC4, 0xA426, 0x9278, 0x11E2, 0x27BC, 0x7D5E, 0x4B00, 0x3713, 0x014D, 0x5BAF, 0x6DF1, 0xEE6B, 0xD835, 0x82D7, 0xB489,
    0xA6BC, 0x90E2, 0xCA00, 0xFC5E, 0x7FC4, 0x499A, 0x1378, 0x2526, 0x5935, 0x6F6B, 0x3589, 0x03D7, 0x804D, 0xB613, 0xECF1, 0xDAAF,
    0x14D7, 0x2289, 0x786B, 0x4E35, 0xCDAF, 0xFBF1, 0xA113, 0x974D, 0xEB5E, 0xDD00, 0x87E2, 0xB1BC, 0x3226, 0x0478, 0x5E9A, 0x68C4,
    0x8F13, 0xB94D, 0xE3AF, 0xD5F1, 0x566B, 0x6035, 0x3AD7, 0x0C89, 0x709A, 0x46C4, 0x1C26, 0x2A78, 0xA9E2, 0x9FBC, 0xC55E, 0xF300,
    0x3D78, 0x0B26, 0x51C4, 0x679A, 0xE400, 0xD25E, 0x88BC, 0xBEE2, 0xC2F1, 0xF4AF, 0xAE4D, 0x9813, 0x1B89, 0x2DD7, 0x7735, 0x416B,
    0xF5E2, 0xC3BC, 0x995E, 0xAF00, 0x2C9A, 0x1AC4, 0x4026, 0x7678, 0x0A6B, 0x3C35, 0x66D7, 0x5089, 0xD313, 0xE54D, 0xBFAF, 0x89F1,
    0x4789, 0x71D7, 0x2B35, 0x1D6B, 0x9EF1, 0xA8AF, 0xF24D, 0xC413, 0xB800, 0x8E5E, 0xD4BC, 0xE2E2, 0x6178, 0x5726, 0x0DC4, 0x3B9A,
    0xDC4D, 0xEA13, 0xB0F1, 0x86AF, 0x0535, 0x336B, 0x6989, 0x5FD7, 0x23C4, 0x159A, 0x4F78, 0x7926, 0xFABC, 0xCCE2, 0x9600, 0xA05E,
    0x6E26, 0x5878, 0x029A, 0x34C4, 0xB75E, 0x8100, 0xDBE2, 0xEDBC, 0x91AF, 0xA7F1, 0xFD13, 0xCB4D, 0x48D7, 0x7E89, 0x246B, 0x1235
};

/* http://www.ip33.com/crc.html */
/* polynomial discard MSB or LSB because they are always 1 */
typedef struct {
    uint16_t initial_value;
    uint16_t result_xor_value;
    uint16_t polynomial;
    bool input_inversion;
    bool output_inversion;
} crc16_param_t;

// #define PRINT_TABLE
#ifndef PRINT_TABLE
static crc16_param_t crc16_param[] = {
    {0x0000, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_IBM_MODEL
    {0x0000, 0xFFFF, 0x8005, true, true},       // Reference Model:CRC16_MAXIM_MODEL
    {0xFFFF, 0xFFFF, 0x8005, true, true},       // Reference Model:CRC16_USB_MODEL
    {0xFFFF, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_MODBUS_MODEL
    {0x0000, 0x0000, 0x1021, true, true},       // Reference Model:CRC16_CCITT_MODEL
    {0xFFFF, 0x0000, 0x1021, false, false},     // Reference Model:CRC16_CCITT_FALSE_MODEL
    {0xFFFF, 0xFFFF, 0x1021, true, true},       // Reference Model:CRC16_X25_MODEL
    {0x0000, 0x0000, 0x1021, false, false},     // Reference Model:CRC16_XMODEM_MODEL
    {0x0000, 0xFFFF, 0x3D65, true, true},       // Reference Model:CRC16_DNP_MODEL
};
#else
/* Generate table usage */
static crc16_param_t crc16_param[] = {
    {0x0000, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_IBM_MODEL
    {0x0000, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_MAXIM_MODEL
    {0x0000, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_USB_MODEL
    {0x0000, 0x0000, 0x8005, true, true},       // Reference Model:CRC16_MODBUS_MODEL
    {0x0000, 0x0000, 0x1021, true, true},       // Reference Model:CRC16_CCITT_MODEL
    {0x0000, 0x0000, 0x1021, false, false},     // Reference Model:CRC16_CCITT_FALSE_MODEL
    {0x0000, 0x0000, 0x1021, true, true},       // Reference Model:CRC16_X25_MODEL
    {0x0000, 0x0000, 0x1021, false, false},     // Reference Model:CRC16_XMODEM_MODEL
    {0x0000, 0x0000, 0x3D65, true, true},       // Reference Model:CRC16_DNP_MODEL
};
#endif

/**
 * @brief uint8_t type data inversion
 *
 * @param data Data to be reversed
 * @return uint8_t The reversed data
 */
static uint8_t u8_data_inversion(uint8_t data) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result <<= 1;
        result |= (data >> i) & 0x01;
    }
    return result;
}

/**
 * @brief uint16_t type data inversion
 *
 * @param data Data to be reversed
 * @return uint16_t The reversed data
 */
static uint16_t u16_data_inversion(uint16_t data) {
    uint16_t result = 0;
    for (uint8_t i = 0; i < 16; i++) {
        result <<= 1;
        result |= (data >> i) & 0x0001;
    }
    return result;
}

/**
 * @brief Calculates the 16 bit wide CRC of an input data of a given length.
 *
 * @param param CRC16 reference model
 * @param input_data Input uint8 t type array data
 * @param length Input uint8 t type array length
 * @return uint16_t CRC16 result
 */
uint16_t crc16_calculate(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16 = crc16_param[model].initial_value;
    uint16_t polynomial = crc16_param[model].polynomial;
    uint8_t data;

    for (size_t i = 0; i < length; i++) {
        data = *input_data++;
        if (crc16_param[model].input_inversion) {
            data = u8_data_inversion(data);
        }
        crc16 ^= (uint16_t)data << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc16 & 0x8000) {
                crc16 <<= 1;
                crc16 ^= polynomial; //  polynomial discard MSB or LSB because they are always 1, don't need to compute
            } else {
                crc16 <<= 1;
            }

        }
    }

    if (crc16_param[model].output_inversion) {
        crc16 = u16_data_inversion(crc16);
    }

    return (crc16 ^ crc16_param[model].result_xor_value);
}

/**
 * @brief The mirror mode computes the 16-bit wide CRC of the input data of a given length.
 *
 * @param param CRC16 reference model
 * @param input_data Input uint8 t type array data
 * @param length Input uint8 t type array length
 * @return uint16_t CRC16 result
 */
uint16_t crc16_calculate_mirror_mode(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16 = crc16_param[model].initial_value;
    uint16_t polynomial = u16_data_inversion(crc16_param[model].polynomial);
    uint8_t data;

    for (size_t i = 0; i < length; i++) {
        data = *input_data++;
        if (!crc16_param[model].input_inversion) {
            data = u8_data_inversion(data);
        }
        crc16 ^= data;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc16 & 0x0001) {
                crc16 >>= 1;
                crc16 ^= polynomial; //  polynomial discard MSB or LSB because they are always 1, don't need to compute
            } else {
                crc16 >>= 1;
            }

        }
    }

    if (!crc16_param[model].output_inversion) {
        crc16 = u16_data_inversion(crc16);
    }

    return (crc16 ^ crc16_param[model].result_xor_value);
}

/**
 * @brief CRC16 calculation method group package
 *
 * @param param CRC16 reference model
 * @param input_data Packets of CRC16 to be computed, last byte of the array holds the CRC16 result
 * @param length Length of the CRC16 packet
 */
void crc16_calculate_package(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16_result = 0;
    if (crc16_param[model].input_inversion == true && crc16_param[model].output_inversion == true) {
        crc16_result = crc16_calculate_mirror_mode(model, input_data, length - 2);
    } else {
        crc16_result = crc16_calculate(model, input_data, length - 2);
    }

    *(input_data + length - 2) = crc16_result & 0xFF;
    *(input_data + length - 1) = (crc16_result >> 8) & 0xFF;
}

/**
 * @brief CRC16 lookup table package
 *
 * @param param CRC16 reference model
 * @param input_data Packets of CRC16 to be computed, last byte of the array holds the CRC16 result
 * @param length Length of the CRC16 packet
 */
void crc16_lookup_table_package(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16_result = 0;
    uint8_t *p = input_data;
    if (p != NULL) {
        switch (model)
        {
        case CRC16_IBM_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_IBM_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_MAXIM_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_MAXIM_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_USB_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_USB_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_MODBUS_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_MODBUS_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_CCITT_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_CCITT_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_CCITT_FALSE_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result << 8) ^ CRC16_CCITT_FALSE_MODEL_TABLE[((crc16_result >> 8) ^ (*p++))];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_X25_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_X25_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_XMODEM_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result << 8) ^ CRC16_XMODEM_MODEL_TABLE[((crc16_result >> 8) ^ (*p++))];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        case CRC16_DNP_MODEL:
            crc16_result = crc16_param[model].initial_value;
            for (size_t i = 0; i < length - 2; i++) {
                crc16_result = (crc16_result >> 8) ^ CRC16_DNP_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
            }
            crc16_result ^= crc16_param[model].result_xor_value;
            break;

        default:
            break;
        }
    }

    *(input_data + length - 2) = crc16_result & 0xFF;
    *(input_data + length - 1) = (crc16_result >> 8) & 0xFF;
}

/**
 * @brief CRC16 calculation method package check
 *
 * @param param CRC16 reference model
 * @param input_data CRC16 packages to check
 * @param length Length of the CRC8 packet
 * @return true CRC16 checks succeed
 * @return false CRC16 checks fails
 */
bool crc16_package_check(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    /* parameter checkout */
    assert(model < CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16_actual_value = ((uint16_t)(*(input_data + length - 1) << 8) | (*(input_data + length - 2)));
    uint16_t crc16_result = 0;
    if (crc16_param[model].input_inversion == true && crc16_param[model].output_inversion == true) {
        crc16_result = crc16_calculate_mirror_mode(model, input_data, length - 2);
    } else {
        crc16_result = crc16_calculate(model, input_data, length - 2);
    }
    if (crc16_result != crc16_actual_value) {
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
bool crc16_lookup_table_package_check(CRC16_reference_model_e model, uint8_t *input_data, size_t length) {
    assert(model <= CRC16_NONE_MODEL);
    assert(input_data != NULL);
    assert(length > 0);

    uint16_t crc16_result = 0;
    uint16_t crc16_actual_value = ((uint16_t)(*(input_data + length - 1) << 8) | (*(input_data + length - 2)));
    uint8_t *p = input_data;
    switch (model)
    {
    case CRC16_IBM_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_IBM_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_MAXIM_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_MAXIM_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_USB_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_USB_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_MODBUS_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_MODBUS_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_CCITT_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_CCITT_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_CCITT_FALSE_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result << 8) ^ CRC16_CCITT_FALSE_MODEL_TABLE[((crc16_result >> 8) ^ (*p++))];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_X25_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_X25_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_XMODEM_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result << 8) ^ CRC16_XMODEM_MODEL_TABLE[((crc16_result >> 8) ^ (*p++))];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    case CRC16_DNP_MODEL:
        crc16_result = crc16_param[model].initial_value;
        for (size_t i = 0; i < length - 2; i++) {
            crc16_result = (crc16_result >> 8) ^ CRC16_DNP_MODEL_TABLE[(crc16_result ^ *p++) & 0xFF];
        }
        crc16_result ^= crc16_param[model].result_xor_value;
        break;

    default:
        break;
    }

    if (crc16_result != crc16_actual_value) {
        return false;
    }
    return true;
}

#define TEST
#ifdef TEST
#include <stdio.h>

static void print_crc16_table(CRC16_reference_model_e model);

int main() {
    uint8_t input_data[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x08};
#if 0
    uint16_t crc16_ibm_model = crc16_calculate(CRC16_IBM_MODEL, input_data, 10);
    uint16_t crc16_maxim_model = crc16_calculate(CRC16_MAXIM_MODEL, input_data, 10);
    uint16_t crc16_usb_model = crc16_calculate(CRC16_USB_MODEL, input_data, 10);
    uint16_t crc16_modbus_model = crc16_calculate(CRC16_MODBUS_MODEL, input_data, 10);
    uint16_t crc16_ccitt_model = crc16_calculate(CRC16_CCITT_MODEL, input_data, 10);
    uint16_t crc16_ccitt_false_model = crc16_calculate(CRC16_CCITT_FALSE_MODEL, input_data, 10);
    uint16_t crc16_x25_model = crc16_calculate(CRC16_X25_MODEL, input_data, 10);
    uint16_t crc16_xmodem_model = crc16_calculate(CRC16_XMODEM_MODEL, input_data, 10);
    uint16_t crc16_dnp_model = crc16_calculate(CRC16_DNP_MODEL, input_data, 10);

    uint16_t mirror_crc16_ibm_model = crc16_calculate_mirror_mode(CRC16_IBM_MODEL, input_data, 10);
    uint16_t mirror_crc16_maxim_model = crc16_calculate_mirror_mode(CRC16_MAXIM_MODEL, input_data, 10);
    uint16_t mirror_crc16_usb_model = crc16_calculate_mirror_mode(CRC16_USB_MODEL, input_data, 10);
    uint16_t mirror_crc16_modbus_model = crc16_calculate_mirror_mode(CRC16_MODBUS_MODEL, input_data, 10);
    uint16_t mirror_crc16_ccitt_model = crc16_calculate_mirror_mode(CRC16_CCITT_MODEL, input_data, 10);
    uint16_t mirror_crc16_ccitt_false_model = crc16_calculate_mirror_mode(CRC16_CCITT_FALSE_MODEL, input_data, 10);
    uint16_t mirror_crc16_x25_model = crc16_calculate_mirror_mode(CRC16_X25_MODEL, input_data, 10);
    uint16_t mirror_crc16_xmodem_model = crc16_calculate_mirror_mode(CRC16_XMODEM_MODEL, input_data, 10);
    uint16_t mirror_crc16_dnp_model = crc16_calculate_mirror_mode(CRC16_DNP_MODEL, input_data, 10);

    printf("---------------------------crc16_calculate result-----------------------\n");
    printf("crc16_ibm_model result = 0x%04x\n", crc16_ibm_model);
    printf("crc16_maxim_model result = 0x%04x\n", crc16_maxim_model);
    printf("crc16_usb_model result = 0x%04x\n", crc16_usb_model);
    printf("crc16_modbus_model result = 0x%04x\n", crc16_modbus_model);
    printf("crc16_ccitt_model result = 0x%04x\n", crc16_ccitt_model);
    printf("crc16_ccitt_false_model result = 0x%04x\n", crc16_ccitt_false_model);
    printf("crc16_x25_model result = 0x%04x\n", crc16_x25_model);
    printf("crc16_xmodem_model result = 0x%04x\n", crc16_xmodem_model);
    printf("crc16_dnp_model result = 0x%04x\n", crc16_dnp_model);


    printf("---------------------crc16_calculate_mirror_mode result------------------\n");
    printf("mirror_crc16_ibm_model result = 0x%04x\n", mirror_crc16_ibm_model);
    printf("mirror_crc16_maxim_model result = 0x%04x\n", mirror_crc16_maxim_model);
    printf("mirror_crc16_usb_model result = 0x%04x\n", mirror_crc16_usb_model);
    printf("mirror_crc16_modbus_model result = 0x%04x\n", mirror_crc16_modbus_model);
    printf("mirror_crc16_ccitt_model result = 0x%04x\n", mirror_crc16_ccitt_model);
    printf("mirror_crc16_ccitt_false_model result = 0x%04x\n", mirror_crc16_ccitt_false_model);
    printf("mirror_crc16_x25_model result = 0x%04x\n", mirror_crc16_x25_model);
    printf("mirror_crc16_xmodem_model result = 0x%04x\n", mirror_crc16_xmodem_model);
    printf("mirror_crc16_dnp_model result = 0x%04x\n", mirror_crc16_dnp_model);
#endif

#if 0
    print_crc16_table(CRC16_IBM_MODEL);
    print_crc16_table(CRC16_MAXIM_MODEL);
    print_crc16_table(CRC16_USB_MODEL);
    print_crc16_table(CRC16_MODBUS_MODEL);
    print_crc16_table(CRC16_CCITT_MODEL);
    print_crc16_table(CRC16_CCITT_FALSE_MODEL);
    print_crc16_table(CRC16_X25_MODEL);
    print_crc16_table(CRC16_XMODEM_MODEL);
    print_crc16_table(CRC16_DNP_MODEL);
#endif
#if 0
    uint16_t crc16_result = 0;
    bool crc16_check_result = false;
    printf("data:[");
    for (int i; i < sizeof(input_data)/sizeof(input_data[0]); i++) {
        printf("0x%02x, ", input_data[i]);
    }
    printf("]\n");

    crc16_calculate_package(CRC16_IBM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_IBM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_IBM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_IBM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_IBM_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_MAXIM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_MAXIM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_MAXIM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_MAXIM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_MAXIM_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_USB_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_USB_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_USB_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_USB_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_USB_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_MODBUS_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_MODBUS_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_MODBUS_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_MODBUS_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_MODBUS_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_CCITT_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_CCITT_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_CCITT_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_CCITT_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_CCITT_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_CCITT_FALSE_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_CCITT_FALSE_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_CCITT_FALSE_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_CCITT_FALSE_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_CCITT_FALSE_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_X25_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_X25_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_X25_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_X25_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_X25_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_XMODEM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_XMODEM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_XMODEM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_XMODEM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_XMODEM_MODEL check fail.\n");
    }

    crc16_calculate_package(CRC16_DNP_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_calculate_package CRC16_DNP_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_package_check(CRC16_DNP_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_DNP_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_DNP_MODEL check fail.\n");
    }
#endif

#if 1
    uint16_t crc16_result = 0;
    bool crc16_check_result = false;

    crc16_lookup_table_package(CRC16_IBM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_IBM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_IBM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_IBM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_IBM_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_MAXIM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_MAXIM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_MAXIM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_MAXIM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_MAXIM_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_USB_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_USB_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_USB_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_USB_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_USB_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_MODBUS_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_MODBUS_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_MODBUS_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_MODBUS_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_MODBUS_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_CCITT_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_CCITT_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_CCITT_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_CCITT_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_CCITT_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_CCITT_FALSE_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_CCITT_FALSE_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_CCITT_FALSE_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_CCITT_FALSE_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_CCITT_FALSE_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_X25_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_X25_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_X25_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_X25_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_X25_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_XMODEM_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_XMODEM_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_XMODEM_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_XMODEM_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_XMODEM_MODEL check fail.\n");
    }

    crc16_lookup_table_package(CRC16_DNP_MODEL, input_data, 12);
    crc16_result = input_data[11] << 8 | input_data[10];
    printf("crc16_lookup_table_package CRC16_DNP_MODEL result = 0x%04x\n", crc16_result);
    crc16_check_result = crc16_lookup_table_package_check(CRC16_DNP_MODEL, input_data, 12);
    if (crc16_check_result) {
        printf("crc16_calculate_package CRC16_DNP_MODEL check succeed.\n");
    } else {
        printf("crc16_calculate_package CRC16_DNP_MODEL check fail.\n");
    }
#endif
    return 0;
}

static void print_crc16_table(CRC16_reference_model_e model) {
    uint16_t crc16_result = 0;
    uint8_t i = 0;
    switch (model)
    {
    case CRC16_IBM_MODEL:
        printf("/* Reference Model:CRC16_IBM_MODEL */\n");
        printf("static const uint16_t CRC16_IBM_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_MAXIM_MODEL:
        printf("/* Reference Model:CRC16_MAXIM_MODEL */\n");
        printf("static const uint16_t CRC16_MAXIM_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_USB_MODEL:
        printf("/* Reference Model:CRC16_USB_MODEL */\n");
        printf("static const uint16_t CRC16_USB_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_MODBUS_MODEL:
        printf("/* Reference Model:CRC16_MODBUS_MODEL */\n");
        printf("const uint16_t CRC16_MODBUS_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_CCITT_MODEL:
        printf("/* Reference Model:CRC16_CCITT_MODEL */\n");
        printf("static const uint16_t CRC16_CCITT_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_CCITT_FALSE_MODEL:
        printf("/* Reference Model:CRC16_CCITT_FALSE_MODEL */\n");
        printf("static const uint16_t CRC16_CCITT_FALSE_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_X25_MODEL:
        printf("/* Reference Model:CRC16_X25_MODEL */\n");
        printf("static const uint16_t CRC16_X25_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_XMODEM_MODEL:
        printf("/* Reference Model:CRC16_XMODEM_MODEL */\n");
        printf("static const uint16_t CRC16_XMODEM_MODEL_TABLE[256] = {\n");
        break;

    case CRC16_DNP_MODEL:
        printf("/* Reference Model:CRC16_DNP_MODEL */\n");
        printf("static const uint16_t CRC16_DNP_MODEL_TABLE[256] = {\n");
        break;

    default:
        break;
    }
    for (int j = 0; j <= 0xFF; j++, i++) {
        if (crc16_param[model].input_inversion == true && crc16_param[model].output_inversion == true) {
            crc16_result = crc16_calculate_mirror_mode(model, &i, 1);
        } else {
            crc16_result = crc16_calculate(model, &i, 1);
        }
        if (i % 16 == 0) {
            printf("    ");
        } else {
            printf(" ");
        }
        printf("0x%04X", crc16_result);
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
