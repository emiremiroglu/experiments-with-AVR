#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include "./lib/macro.h"

int main(void)
{
  output(B,5);

  while(1)
  {
    toggle(B,5);
    _delay_ms(1000);
  }
}
