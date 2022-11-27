#define USECPERTICK 50
#define CLKFUDGE 5
#define CLKMAX 256
#define PRESCALE 8
#define SYSCLOCK 16000000
#define CLKSPERUSEC (SYSCLOCK/PRESCALE/1000000)
#define MAXBUF 8

#define MARK  0
#define SPACE 1
#define NBITS 32

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define INIT_TIMER_COUNT2 (CLKMAX - USECPERTICK*CLKSPERUSEC + CLKFUDGE)
#define RESET_TIMER2 TCNT2 = INIT_TIMER_COUNT2

#define STARTNOM      9000
#define SPACENOM      4500
#define BITMARKNOM    620
#define ONESPACENOM   1600
#define ZEROSPACENOM  480
#define RPTSPACENOM   2180

#define TOLERANCE 20
#define LTOL (1.0 - TOLERANCE/100.)
#define UTOL (1.0 + TOLERANCE/100.)


#define STARTMIN      (int)((STARTNOM/USECPERTICK)*LTOL)
#define STARTMAX      (int)((STARTNOM/USECPERTICK)*UTOL)
#define SPACEMIN      (int)((SPACENOM/USECPERTICK)*LTOL)
#define SPACEMAX      (int)((SPACENOM/USECPERTICK)*UTOL)
#define BITMARKMIN    (int)((BITMARKNOM/USECPERTICK)*LTOL-2)
#define BITMARKMAX    (int)((BITMARKNOM/USECPERTICK)*UTOL+2)
#define ONESPACEMIN   (int)((ONESPACENOM/USECPERTICK)*LTOL)
#define ONESPACEMAX   (int)((ONESPACENOM/USECPERTICK)*UTOL)
#define ZEROSPACEMIN  (int)((ZEROSPACENOM/USECPERTICK)*LTOL-2)
#define ZEROSPACEMAX  (int)((ZEROSPACENOM/USECPERTICK)*UTOL+2)
#define RPTSPACEMIN   (int)((RPTSPACENOM/USECPERTICK)*LTOL)
#define RPTSPACEMAX   (int)((RPTSPACENOM/USECPERTICK)*UTOL)


#define IDLE     1
#define STARTH   2
#define STARTL   3
#define BIT      4
#define ONE      5
#define ZERO     6
#define STOP     7
#define BITMARK  8
#define RPTMARK  9


static volatile struct {
  uint8_t rcvstate;
  uint8_t bitcounter;
  uint8_t irdata;
  uint8_t fptr;
  uint8_t rptr;
  uint8_t irpin;
  unsigned int timer;
  unsigned long irmask;
  unsigned long ircode;
  unsigned long irbuf[MAXBUF];
} irparams;

class IR
{
  public:

    void begin()
    {

      TCCR2A = 0;

      cbi(TCCR2B,CS20);
      cbi(TCCR2B,CS22);
      sbi(TCCR2B,CS21);
      sbi(TIMSK2,TOIE2);

      RESET_TIMER2;

      sei();

      irparams.rcvstate = IDLE;
      irparams.bitcounter = 0;
      irparams.ircode = 0;
      irparams.fptr = 0;
      irparams.rptr = 0;
    }

    int available()
    {
      int n;
      n = irparams.fptr - irparams.rptr;
      if (n < 0)
        n += MAXBUF;
      return(n);
    }

    unsigned long receive()
    {
      unsigned long ircode;
      if(irparams.fptr != irparams.rptr)
      {
        ircode = irparams.irbuf[irparams.rptr];
        irparams.rptr = (irparams.rptr + 1) % MAXBUF;
        return((unsigned long)ircode);
      }
      else {
        return((unsigned long)-1);
      }

    }

    void flush()
    {
      irparams.rptr = irparams.fptr;
    }
};


ISR(TIMER2_OVF_vect)
{
  RESET_TIMER2;

  if(read(B,0)) {
    irparams.irdata = 0x1;
  } else {
    irparams.irdata = 0x0;
  }

  switch(irparams.rcvstate) {
    case IDLE:
      if (irparams.irdata == MARK) {
          irparams.rcvstate = STARTH;
          irparams.timer = 0;
      }
      break;
    case STARTH:

      if (irparams.irdata == SPACE) {
        if ((irparams.timer >= STARTMIN) && (irparams.timer <= STARTMAX)) {
          irparams.rcvstate = STARTL;
          irparams.timer = 0;
        }
        else
          irparams.rcvstate = IDLE;
      }
      else
        irparams.timer++;
      break;
    case STARTL:

      if (irparams.irdata == MARK) {
        if ((irparams.timer >= SPACEMIN) && (irparams.timer <= SPACEMAX)) {
          irparams.rcvstate = BITMARK;
          irparams.timer = 0;
          irparams.bitcounter = 0;
          irparams.irmask = (unsigned long)0x1;
          irparams.ircode = 0;
        }
        else if ((irparams.timer >= RPTSPACEMIN) && (irparams.timer <= RPTSPACEMAX)) {
          irparams.rcvstate = RPTMARK;
          irparams.timer = 0;
        }
        else
          irparams.rcvstate = IDLE;
      }
      else {
        irparams.timer++;
        if (irparams.timer >= SPACEMAX)
          irparams.rcvstate = IDLE;
      }
      break;
    case RPTMARK:
      irparams.timer++;
      if (irparams.irdata == SPACE) {
        if ((irparams.timer >= BITMARKMIN) && (irparams.timer <= BITMARKMAX))
          irparams.rcvstate = IDLE;
        else
          irparams.rcvstate = IDLE;
      }
      break;
    case BITMARK:
      irparams.timer++;
      if (irparams.irdata == SPACE) {
        if ((irparams.timer < BITMARKMIN) || (irparams.timer > BITMARKMAX))
          irparams.rcvstate = IDLE;
        else {
          irparams.rcvstate = BIT;
          irparams.timer = 0;
        }
      }
      break;
    case BIT:
      irparams.timer++;
      if (irparams.irdata == MARK) {
        if ((irparams.timer >= ONESPACEMIN) && (irparams.timer <= ONESPACEMAX)) {
          irparams.rcvstate = ONE;
          irparams.timer = 0;
        }
        else if ((irparams.timer >= ZEROSPACEMIN) && (irparams.timer <= ZEROSPACEMAX)) {
          irparams.rcvstate = ZERO;
          irparams.timer = 0;
        }
        else
          irparams.rcvstate = IDLE;
      }
      else {
        if (irparams.timer > ONESPACEMAX)
          irparams.rcvstate = IDLE;
      }
      break;
    case ONE:
      irparams.ircode |= irparams.irmask;
      irparams.irmask <<= 1;
      irparams.bitcounter++;
      if (irparams.bitcounter < NBITS)
        irparams.rcvstate = BITMARK;
      else
        irparams.rcvstate = STOP;
      break;
    case ZERO:
      irparams.irmask <<= 1;
      irparams.bitcounter++;
      if (irparams.bitcounter < NBITS)
        irparams.rcvstate = BITMARK;
      else
        irparams.rcvstate = STOP;
      break;
    case STOP:
      irparams.timer++;
      if (irparams.irdata == SPACE) {
        if ((irparams.timer >= BITMARKMIN) && (irparams.timer <= BITMARKMAX)) {
          irparams.irbuf[irparams.fptr] = irparams.ircode;
          irparams.fptr = (irparams.fptr + 1) % MAXBUF;
        }
        irparams.rcvstate = IDLE;
      }
      break;
  }

}
// unsigned long code;
// ir.begin();
// while(ir.available())
// {
//   code = ir.receive();
//
//   switch(code)
//   {
//     // CENTER
//     case 1868400622:
//     break;
//
//     // UP
//     case 1863026670:
//     break;
//
//     // DOWN
//     case 1863157742:
//     break;
//
//     // LEFT
//     case 1862830062:
//     break;
//
//     // RIGHT
//     case 1862764526:
//     break;
//
//     // PLAY
//     case 1868466158:
//     break;
//   }
//
// }
