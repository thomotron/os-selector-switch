#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 9600

static char uart_getc(void) {
    while ((UCSRA & (1<<RXC)) == 0);
    return UDR;
}

static void uart_putc(char c) {
    while ((UCSRA & (1<<UDRE)) == 0);
    UDR = c;
}

static void uart_init(void) {
    UBRRH = (F_CPU/(16L*BAUDRATE)-1) >> 8;
    UBRRL = (F_CPU/(16L*BAUDRATE)-1) & 0xFF;
    UCSRC = 3<<UCSZ0;
    UCSRB = (1<<RXEN) | (1<<TXEN);
}

static int get_switch_state(void) {
    return PINB & 0b00000011;
}

static int wait_for_str(const char *s) {
    const char *p = s;
    char c;

    while (*p != '\0') {
        c = uart_getc();

        if (c == *p) {
            ++p;
        } else {
            p = s;
            if (c == *p) {
                ++p;
            }
        }
    }

    return 0;
}

int main(void) {
    PORTB |= 0b00000011;

    uart_init();

    for (;;) {
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