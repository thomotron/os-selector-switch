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
#include <string.h>

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
        if (!compareString(serial_buffer, "GNU GRUB")) continue;

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

int compareString(char* str, char* template)
{
    int matches = 0;
    unsigned short str_start = str;
    unsigned short template_start = template;

    while (*str && *template)
    {
        matches += *str == *template ? 1 : 0;
    }

    switch ()
}

unsigned char ReverseByte (unsigned char x) {
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

// Initialise USI for UART reception.
void InitialiseUSI (void) {
    DDRB &= ~(1<<DataIn);           // Define DI as input
    USICR = 0;                      // Disable USI.
    GIFR = 1<<PCIF;                 // Clear pin change interrupt flag.
    GIMSK |= 1<<PCIE;               // Enable pin change interrupts
    PCMSK |= 1<<PCINT0;             // Enable pin change on pin 0
}

// Pin change interrupt detects start of UART reception.
ISR (PCINT0_vect) {
        if (PINB & 1<<DataIn) return;   // Ignore if DI is high
        GIMSK &= ~(1<<PCIE);            // Disable pin change interrupts
        TCCR0A = 2<<WGM00;              // Timer in CTC mode
        TCCR0B = 2<<CS00;               // Set prescaler to /8
        OCR0A = 103;                    // Shift every (103+1)*8 cycles
        TCNT0 = 206;                    // Start counting from (256-52+2)
        // Enable USI OVF interrupt, and select Timer0 compare match as USI Clock source:
        USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
        USISR = 1<<USIOIF | 8;          // Clear USI OVF flag, and set counter
}

// USI overflow interrupt indicates we've received a byte
ISR (USI_OVF_vect) {
        USICR = 0;                      // Disable USI
        int temp = USIDR;
        Display(ReverseByte(temp));
        GIFR = 1<<PCIF;                 // Clear pin change interrupt flag.
        GIMSK |= 1<<PCIE;               // Enable pin change interrupts again
}

// Reads a single char from the USI register
char readChar()
{

}