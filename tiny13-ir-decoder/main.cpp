#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#define	LED         PB0
// #define	IR_PIN      PB1
// #define	SERIAL_PIN  PB3
// #define SERIAL_BAUD 115200

#include "./lib/pins.h"
// #include "./lib/pwm.h"
// #include "./lib/serial.h"
// #include "./lib/ir.h"
// #include "./lib/led.h"

int main(void)
{
  int val = 0;

  pwm.init();

	while(1)
  {
    _delay_ms(1000);






    // PWM
    // if(val < 90) {
    //   val++;
    // }
    // pwm.write(val);
    // _delay_ms(200);
	}
  return 0;
}
