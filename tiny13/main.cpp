#include <avr/interrupt.h>
#include <avr/io.h>

#define	IR_PIN PB0
// #define SERIAL_BAUD 115200
// #define	SERIAL_PIN PB2

#include "./lib/pins.h"
// #include "./lib/serial.h"
#include "./lib/ir.h"
// #include "./lib/led.h"

int main(void){

  input(IR_PIN);

  uint8_t code;
	ir.init();

	while (1){
		if(ir.read(&code)) {
      switch(code){
        case 2:
          // MENU
        break;

        case 4:
          // CENTER
        break;

        case 11:
          // UP
        break;

        case 13:
          // DOWN
        break;

        case 8:
          // LEFT
        break;

        case 7:
          // RIGHT
        break;


      }
    }
	}
  return 0;
}
