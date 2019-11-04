// This file is written to correspond with the
// following pinout:
//
//              ATTiny85
//             .---v---.
// B5   RESET -| 1   8 |- Vcc
// B3  SWITCH -| 2   7 |- SCK  B2
// B4         -| 3   6 |- TX   B1
//        GND -| 4   5 |- RX   B0
//             *-------*

#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 9600

// Buffer to store each character read in from serial
char serial_buffer[64];

void init()
{
    // Set DDR register
    // (1 = out, 0 = in)
    DDRB = 0b010000;

    // Set up the USI for receiving UART
    // USICR7:   Enable start condition interrupt
    // USICR6:   Disable counter overflow interrupt
    // USICR5-4: Select 3-wire mode
    // USICR3-1: Use external clock on rising edge
    // USICR0:   Disable clock generation
    USICR = 0b10011000;
}

int main(void) {
    // Initialise everything
    init();

    while (1) {
        wait_for_str("GNU GRUB");

        _delay_ms(2000);

        switch (get_switch_state()) {
            case 2:
                uart_putc('1');
                break;

            case 1:
                uart_putc('2');
                break;

            default:
                break;
        }
    }

    return 0;
}