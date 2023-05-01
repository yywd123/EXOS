#pragma once

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xa0
#define PIC2_DATA 0xa1

void maskIRQ(uint8_t irq, bool enable);
void sendEOI(uint8_t irq);