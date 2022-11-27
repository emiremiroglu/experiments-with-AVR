#include "config.h"

int main(void)
{
  out(PB0);

  while(1)
  {
    // serial.send(".");
    dot.blink();
    _delay_ms(100);
  }

  return 0;
}
