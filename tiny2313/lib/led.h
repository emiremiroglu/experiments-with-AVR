class LED
{
  public:
    LED() {
      DDRB |= (1 << PB2);
      TCCR0A = (1 << COM0A1) | (1 << WGM00);
      TCCR0B = (1 << CS01);
    }

    void pulse(int max)
    {
      int b = 0;
      while(b < max){
        OCR0A = b;
        b++;
        _delay_ms(10);
      }
    }
};

LED led;
