class PWM
{
  public:
    void init(void)
    {
      DDRB |= _BV(PB0);
      TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
      TCCR0B = _BV(CS00);
    }
    void write(int value)
    {
      OCR0A = value;
    }
};

PWM pwm;
