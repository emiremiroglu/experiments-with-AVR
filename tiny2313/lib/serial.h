#include <util/setbaud.h>

class SERIAL
{
  public:
    SERIAL(){
    	UBRRH = UBRRH_VALUE;
    	UBRRL = UBRRL_VALUE;
    	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
    	UCSRC = (1<<USBS)|(3<<UCSZ0);
    }
    void print(const char *s){
      while(*s){
        tx(*(s++));
      }
    }
  private:
    void tx(char c){
      while (!(UCSRA & (1<<UDRE)));
      UDR = c;
    }
};

SERIAL serial;
