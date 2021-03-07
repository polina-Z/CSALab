#include <iostream>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    uint16_t temp = *reg ^ (*reg >> 2) ^ (*reg >> 3) ^ (*reg >> 5);
    temp &= 1;
    *reg >>= 1;
    *reg &= ~((1 ^ temp) << 15);
    *reg |= temp << 15;   
}

