#include "memory_driver.h"

#define SRAM_CS_PIN 10 // Chip Select Pin for 23LCV512

bool MemoryDriver::begin() {
    pinMode(SRAM_CS_PIN, OUTPUT);
    digitalWrite(SRAM_CS_PIN, HIGH); // Avaktivera SRAM
    SPI.begin();
    // Eventuellt ytterligare initialisering
    return true;
}

bool MemoryDriver::writeData(const char* data, size_t length, unsigned long address) {
    digitalWrite(SRAM_CS_PIN, LOW); // Aktivera SRAM

    // Skriv till specifik adress
    SPI.transfer(0x02); // WRITE instruction
    SPI.transfer((address >> 8) & 0xFF); // Address byte 1
    SPI.transfer(address & 0xFF);        // Address byte 2

    for (size_t i = 0; i < length; i++) {
        SPI.transfer(data[i]);
    }

    digitalWrite(SRAM_CS_PIN, HIGH); // Avaktivera SRAM
    return true;
}

bool MemoryDriver::readData(char* buffer, size_t length, unsigned long address) {
    digitalWrite(SRAM_CS_PIN, LOW); // Aktivera SRAM

    // L�s fr�n specifik adress
    SPI.transfer(0x03); // READ instruction
    SPI.transfer((address >> 8) & 0xFF); // Address byte 1
    SPI.transfer(address & 0xFF);        // Address byte 2

    for (size_t i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00); // L�s data
    }

    digitalWrite(SRAM_CS_PIN, HIGH); // Avaktivera SRAM
    return true;
}
