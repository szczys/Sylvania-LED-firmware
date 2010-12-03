 #define F_CPU 1200000

#include <avr/io.h>
#include <util/delay.h>

#define red (1<<2)
#define green (1<<3)
#define blue (1<<4)
#define magenta (red | blue)
#define cyan (blue | green)
#define yellow (green | red)
#define white (red | blue | green)

void delay_ms(int cnt)
{
  while(cnt--)
  {
    _delay_ms(1);
  }
}
int main(void)
{
  delay_ms(500);
  DDRB |= (red | green | blue);
  PORTB |= (red | green | blue);
  unsigned char tracker = 0;

  while(1)
  {
    PORTB |= (red | green | blue);
    
    switch (tracker)
    {
      case 0:
        PORTB &= ~red;
        break;
      case 1:
        PORTB &= ~magenta;
        break;
      case 2:
        PORTB &= ~blue;
        break;
      case 3:
        PORTB &= ~cyan;
        break;
      case 4:
        PORTB &= ~green;
        break;
      case 5:
        PORTB &= ~yellow;
        break;
      case 6:
        PORTB &= ~white;
        break;
    }    
    if (++tracker > 6) tracker = 0;

    delay_ms(500);

  }
}
