#ifndef MEMORY_DRIVER_H
#define MEMORY_DRIVER_H

#include <Arduino.h>
#include <SPI.h>

class MemoryDriver {
public:
    static bool begin();
    static bool writeData(const char* data, size_t length, unsigned long address = 0x0000);
    static bool readData(char* buffer, size_t length, unsigned long address = 0x0000);
};

#endif // MEMORY_DRIVER_H
