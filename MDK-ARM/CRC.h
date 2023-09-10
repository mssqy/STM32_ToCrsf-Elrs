#ifndef _CRC_H__
#define _CRC_H__

#include "stdint.h"

void Crc_init(uint8_t poly);
uint8_t Calc(uint8_t *data, uint8_t len);

#endif
