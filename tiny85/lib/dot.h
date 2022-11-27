class DOT
{
  public:

    void blink()
    {
      on(PB0);
      _delay_us(50);
      off(PB0);
    }

    void go(char direction)
    {
      switch(direction) {
        case 'F':
          // Go Forward
        break;
        case 'B':
          // Go Bacward
        break;
      }
    }

    void turn(char direction)
    {
      switch(direction)
      {
        case 'L':
          // Go Forward
        break;
        case 'R':
          // Go Bacward
        break;
      }
    }
};

DOT dot;
