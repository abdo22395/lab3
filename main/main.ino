#include "memory_driver.h"

#define BAUD_RATE 9600

void setup() {
    // Starta seriell kommunikation
    Serial.begin(BAUD_RATE);
    while (!Serial) {
        ; // Vänta på att seriell port ska bli tillgänglig
    }

    // Initiera 23LCV512
    if (!MemoryDriver::begin()) {
        Serial.println("Failed to initialize 23LCV512!");
    } else {
        Serial.println("23LCV512 initialized successfully.");
    }
}

void loop() {
    // Kolla om data är tillgänglig från Raspberry Pi
    if (Serial.available() > 0) {
        // Läs in data fram till newline
        String data = Serial.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(data);

        // Konvertera String till char array
        char buffer[256];
        data.toCharArray(buffer, sizeof(buffer));

        // Skriv data till 23LCV512 på adress 0x0000
        if (MemoryDriver::writeData(buffer, strlen(buffer), 0x0000)) {
            Serial.println("Data written to 23LCV512.");
        } else {
            Serial.println("Failed to write data to 23LCV512.");
        }

        // Svara tillbaka till Raspberry Pi
        Serial.println("ACK");
    }
}
