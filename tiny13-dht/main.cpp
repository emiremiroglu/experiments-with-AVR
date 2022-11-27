#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>

#define	LED         PB0
#define	SERIAL_PIN  PB3
#define SERIAL_BAUD 115200
#define	SENSOR      PB2

#include "./lib/macro.h"
#include "./lib/serial.h"
#include "./lib/sensor.h"

int main(void)
{
	while(1)
	{
    sensor.read();
  }
  return 0;
}
