 #define F_CPU 1200000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define black 0
#define red (1<<2)
#define green (1<<3)
#define blue (1<<4)
#define magenta (red | blue)
#define cyan (blue | green)
#define yellow (green | red)
#define white (red | blue | green)

volatile unsigned char curr_color = red;

void delay_ms(unsigned int cnt)
{
  while(cnt--)
  {
    _delay_ms(1);
  }
}

void delay_min(unsigned char cnt)
{
  while(cnt--)
  {
    delay_ms(60000);
  }
}
void init_timers(void)
{
  cli();
  //Timer0 for buttons
  OCR0A = 0;				// 0% duty cycle
  TCCR0A |= 1<<WGM01 | 1<<WGM00;	// Fast PWM
  TCCR0B |= 1<<CS01 | 1<<CS00;		//start timer, 1/64 prescale
  TIMSK0 |= 1<<OCIE0A | 1<<TOIE0;	//enable compA and overflow interrupt
  sei();
}

int main(void)
{

  //Setup jumper inputs
  DDRB &= ~(1<<PB1);	//PB1 as input
  PORTB |= (1<<PB1);	//Enable pull-up on PB1
  unsigned char tracker = 0;

  //Setup LED pins
  PORTB |= white;
  DDRB |= white;

  delay_ms(500);

  if (~PINB & (1<<PB1))
  {
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

  init_timers();


  while(1)
  {
    //fade in red
    curr_color = red;
    tracker = 0;
    while (++tracker < 255)
    {
      OCR0A = tracker;
      delay_ms(20);
    }
    //Red for 2 seconds
    delay_min(20);

    //Step down red
    tracker = 255;
    OCR0A = tracker;
    while (--tracker > 0)
    {
      OCR0A = tracker;
      delay_ms(20);
    }

    //Go dark for a while
    curr_color = black;
    delay_ms(30000);

    //fade in green
    curr_color = green;
    while (++tracker < 255)
    {
      OCR0A = tracker;
      delay_ms(20);
    }
    //Red for 2 seconds
    delay_min(20);

    //Step down green
    tracker = 255;
    while (--tracker > 0)
    {
      OCR0A = tracker;
      delay_ms(20);
    }

    //Go dark for a while
    curr_color = black;
    delay_ms(30000);

  }
}

ISR(TIM0_OVF_vect)
{
  //PWM will be at BOTTOM, Turn on LED by setting PIN low
  PORTB &= ~curr_color;
}

ISR(TIM0_COMPA_vect)
{
  //PWM just matched duty cycle crossing, Turn off LEDs by setting pins high
  PORTB |= curr_color;
}
