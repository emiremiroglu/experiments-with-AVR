class PWM
{
  public:

    void start(void)
    {
      DDRB |= _BV(PB1);
      TCCR1A |= _BV(COM1A1) | _BV(WGM10);
      TCCR1B |= _BV(CS10) | _BV(WGM12);
    }

    void write(int value)
    {
      OCR1A = value;
    }
    
};
