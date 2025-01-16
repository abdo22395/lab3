#include <MemoryDriver.h> // Inkludera MemoryDriver-biblioteket

MemoryDriver memory;

void setup() {
    // Starta seriell kommunikation
    Serial.begin(9600);
    while (!Serial) {
        ; // Vänta på att seriell port ska bli tillgänglig (endast för vissa kort)
    }

    // Initiera 23LCV512 SRAM
    if (!memory.begin()) {
        Serial.println("Failed to initialize 23LCV512!");
    } else {
        Serial.println("23LCV512 initialized successfully.");
    }
}

void loop() {
    // Kolla om data är tillgänglig från Raspberry Pi via UART
    if (Serial.available() > 0) {
        // Läs in data fram till newline
        String command = Serial.readStringUntil('\n');
        Serial.print("Received Command: ");
        Serial.println(command);

        // Hantera kommandon
        if (command.startsWith("WRITE:")) {
            // Extrahera data att skriva
            String data = command.substring(6); // Ta bort "WRITE:"
            char buffer[256];
            data.toCharArray(buffer, sizeof(buffer));

            // Skriv data till 23LCV512 på adress 0x0000
            if (memory.writeData(buffer, strlen(buffer), 0x0000)) {
                Serial.println("Data written to 23LCV512.");
            } else {
                Serial.println("Failed to write data to 23LCV512.");
            }

            // Svara tillbaka till Raspberry Pi
            Serial.println("ACK");
        }
        else if (command.startsWith("READ:")) {
            // Extrahera antal bytes att läsa
            int length = command.substring(5).toInt();
            char buffer[256];
            if (memory.readData(buffer, length, 0x0000)) {
                buffer[length] = '\0'; // Null-terminera
                Serial.print("Data from 23LCV512: ");
                Serial.println(buffer);
            } else {
                Serial.println("Failed to read data from 23LCV512.");
            }

            // Svara tillbaka till Raspberry Pi
            Serial.println("ACK");
        }
        else {
            Serial.println("Unknown Command.");
        }
    }
}
