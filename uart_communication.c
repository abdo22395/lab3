#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control definitions
#include <unistd.h>     // UNIX standard function definitions
#include <errno.h>

// Funktion för att konfigurera UART
int configure_uart(int fd) {
    struct termios options;

    // Läs in nuvarande inställningar
    if (tcgetattr(fd, &options) < 0) {
        perror("tcgetattr");
        return -1;
    }

    // Ställ in baud rate
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // 8N1 (8 data bits, no parity, 1 stop bit)
    options.c_cflag &= ~PARENB; // Ingen paritet
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;     // 8 data bits

    // Ingen hårdvara flödeskontroll
    options.c_cflag &= ~CRTSCTS;

    // Aktivera mottagning och sätt som lokal terminal
    options.c_cflag |= (CLOCAL | CREAD);

    // Ingen mjukvara flödeskontroll
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Rå läge
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Rå utgångsläge
    options.c_oflag &= ~OPOST;

    // Timeout och minsta antal tecken
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10; // Timeout i decisekunder

    // Sätt nya inställningar
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int main() {
    int uart_fd;
    char *uart_device = "/dev/ttyACM0"; // Ändra om nödvändigt
    char send_buffer[256];
    char recv_buffer[256];
    int bytes_written, bytes_read;

    // Öppna UART-enheten
    uart_fd = open(uart_device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("open UART");
        return EXIT_FAILURE;
    }

    // Konfigurera UART
    if (configure_uart(uart_fd) != 0) {
        close(uart_fd);
        return EXIT_FAILURE;
    }

    // Gör UART-enheten icke-blockerande
    fcntl(uart_fd, F_SETFL, 0);

    // Exempel på data att skicka (hjärtfrekvensvärde)
    int heart_rate = 75; // Exempelvärde
    snprintf(send_buffer, sizeof(send_buffer), "%d\n", heart_rate);

    // Skicka data till Arduino
    bytes_written = write(uart_fd, send_buffer, strlen(send_buffer));
    if (bytes_written < 0) {
        perror("write");
        close(uart_fd);
        return EXIT_FAILURE;
    }
    printf("Sent %d bytes: %s", bytes_written, send_buffer);

    // Vänta på svar från Arduino
    memset(recv_buffer, 0, sizeof(recv_buffer));
    bytes_read = read(uart_fd, recv_buffer, sizeof(recv_buffer) - 1);
    if (bytes_read < 0) {
        perror("read");
        close(uart_fd);
        return EXIT_FAILURE;
    } else if (bytes_read == 0) {
        printf("No data received.\n");
    } else {
        recv_buffer[bytes_read] = '\0'; // Null-terminera strängen
        printf("Received %d bytes: %s", bytes_read, recv_buffer);
    }

    // Stäng UART-enheten
    close(uart_fd);

    return EXIT_SUCCESS;
}
