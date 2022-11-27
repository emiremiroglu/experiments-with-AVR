#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include "./lib/pins.h"

int main(void){
	output(0);

	while(1){
		off(0);
		_delay_ms(500);
		on(0);
		_delay_ms(500);
	}
  return 0;
}
