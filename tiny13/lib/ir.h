#define	WAIT		(0)
#define	START		(1)
#define	FINISH	(2)
#define	PROCESS (3)

#define	IR_START	(0)
#define	IR_DATA		(1)
#define	IR_FINISH	(2)
#define	IR_HOOK		(3)

volatile uint16_t timeout = 0;
volatile uint16_t count = 0;
volatile uint32_t raw = 0;

uint8_t event = 0;
uint8_t proto = 0;
uint8_t index = 0;
uint32_t data = 0;

class IR
{
	public:
		void init()
		{
			DDRB &= ~_BV(IR_PIN);
			PORTB &= ~_BV(IR_PIN);
			TCCR0A |= _BV(WGM01);
			TCCR0B |= _BV(CS00);
			TIMSK0 |= _BV(OCIE0A);
			OCR0A = 122;
			GIMSK |= _BV(INT0);
			MCUCR &= ~_BV(ISC01);
			MCUCR |= _BV(ISC00);
			sei();
		}

		int8_t read(uint8_t *command)
		{
			if(!raw) return 0x0;
			*command = raw >> 16;
			raw = 0;
			return 0x1;
		}

		static int8_t nec(uint16_t counter, uint8_t value)
		{
			int8_t bit = 0x1;
			switch(proto)
			{
				case IR_START:
					if (value == 0x1) {
						if(counter > 330 && counter < 360) {
							proto = IR_DATA;
							data = index = 0;
							bit = 0x0;
						} else if (counter > 155 && counter < 185) {
							proto = IR_FINISH;
							bit = 0x0;
						}
					}
				break;
				case IR_DATA:
					if (index < 32) {
						if (value == 0x1) {
							data |= ((uint32_t)((counter < 90) ? 0 : 1) << index++);
							if (index == 32) {
								proto = IR_HOOK;
							}
						}
						bit = 0x0;
					}
				break;
				case IR_HOOK:
					if(value == 0x0) {
						proto = IR_FINISH;
						bit = 0x0;
					}
				break;
				case IR_FINISH:
					raw = data;
				break;
			}
			return bit;
		}

		static void process()
		{
			uint8_t value;
			uint16_t counter;

			counter = count;
			count = 0;

			value = (PINB & (1 << IR_PIN)) > 0 ? 0x0 : 0x1;

			switch(event)
			{
				case WAIT:
					if (value == 0x1) {
						event = START;
					}
				break;
				case START:
					if (value == 0x0) {
						if (counter > 655 && counter < 815) {
							event = PROCESS;
							proto = IR_START;
							timeout = 7400;
						}
						else {
							event = FINISH;
						}
					}
					else {
						event = FINISH;
					}
				break;
				case PROCESS:
					if (nec(counter, value)) {
						event = FINISH;
					}
				break;

				case FINISH:
					event = WAIT;
					timeout = 0;
				break;
			}
		}
};

IR ir;

ISR(INT0_vect)
{
	ir.process();
}

ISR(TIM0_COMPA_vect)
{
	if ((count++ > 10000) ||
	(timeout && --timeout == 0))
		event = WAIT;
}
