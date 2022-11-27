#define in(port, pin) DDR ## port &= ~(1<<pin)
#define out(port, pin) DDR ## port |= (1<<pin)
#define on(port, pin) PORT ## port |= (1<<pin)
#define off(port, pin) PORT ## port &= ~(1<<pin)
#define flip(port, pin) PORT ## port ^= (1<<pin)
#define read(port, pin) (PIN ## port & (1<<pin))

/*
  A0 XTAL
  A1 XTAL

  A2 RESET

  B0 IO
  B1 IO
  B2 PWM
  B3 PWM
  B4 PWM

  D0 RX
  D1 TX

  D2 INT0
  D3 INT1
  D4 IO
  D5 IO
  D6 IO

*/
