#include <avr/io.h>
#include <util/delay.h>
 
int main() {
	/* Setăm pinul 7 al portului D ca pin de ieșire. */
	DDRD |= (1 << PD7);
  DDRD |= (1 << PD5);
  DDRB |= (1 << PB3);

  DDRB &= ~(1 << PB2);
  PORTB |= (1 << PB2); //AM ACTIVAT REZISTENTA DE PULLUP

  int state = 0, pressed = 0;
 
	while(1) {
    
    if((!(PINB & (1 << PB2))) && pressed == 0){
      state = (state+1)%3;
      pressed = 1;
    }
    else{
      pressed = 0;
    }
    switch (state)    {
    case 0 :
      PORTB |= (1 << PB3);
      PORTD &= ~(1 << PD7);
      break;

    case 1:
      PORTD |= (1 << PD7);
      PORTD &= ~(1 << PD5);
      break;

    case 2:
       PORTD |= (1 << PD5);
       PORTB &= ~(1 << PB3);
      break;
    
    default:
      break;
    }

    _delay_ms(100);
		/*PORTD &= ~(1 << PD7);

    if(!(PINB & (1 << PB2)))
    {
      PORTD &= ~(1 << PD5);
    }
    else
    {
      PORTD |= (1 << PD5);
    }*/
	}

 return 0;
 }
