class DOT
{
  public:
    
    void blink()
    {
      on(B,5);
      _delay_ms(20);
      off(B,5);
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
