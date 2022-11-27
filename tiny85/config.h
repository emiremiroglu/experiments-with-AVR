#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#define	LED_PIN PB0
#define	IR_PIN PB1
#define	SERIAL_PIN PB3

#include "./lib/macro.h"
#include "./lib/serial.h"
#include "./lib/dot.h"
// #include "./lib/ir.h"
