#define	TXDELAY (int)(((F_CPU/115200)-7 +1.5)/3)

class SERIAL
{
  public:
    SERIAL()
    {
      out(SERIAL_PIN);
    }

    void send(const char *s)
    {
    	while(*s) {
    		tx(*(s++));
    	}
    }

  private:
    static void tx(char c)
    {
      uint8_t sreg;
      sreg = SREG;
      cli();

      __asm volatile(
        " cbi %[uart_port], %[uart_pin] \n\t"
        " in r0, %[uart_port] \n\t"
        " ldi r30, 3 \n\t"
        " ldi r28, %[txdelay] \n\t"
        "TxLoop: \n\t"
        " mov r29, r28 \n\t"
        "TxDelay: \n\t"
        " dec r29 \n\t"
        " brne TxDelay \n\t"
        " bst %[ch], 0 \n\t"
        " bld r0, %[uart_pin] \n\t"
        " lsr r30 \n\t"
        " ror %[ch] \n\t"
        " out %[uart_port], r0 \n\t"
        " brne TxLoop \n\t"
        :
        : [uart_port] "I" (_SFR_IO_ADDR(PORTB)),
        [uart_pin] "I" (SERIAL_PIN),
        [txdelay] "I" (TXDELAY),
        [ch] "r" (c)
        : "r0","28","r29","r30"
      );
      SREG = sreg;
    }
};

SERIAL serial;
