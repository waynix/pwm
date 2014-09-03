#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define LEDDDR DDRE
#define LEDPORT PORTE
#define LEDBIT (1 << PE6)
const uint8_t  MAX_PORTCOUNT = 20;
const uint8_t  PORTCOUNT = 5;

uint8_t channels[5];


ISR(TIMER0_COMPA_vect)
{
    static uint8_t current = 0;
    static uint8_t counter = 0;
    static uint8_t currentEnd = 0;

    counter += 1;
    if(counter == 0)
    {
        current = current + 1;
        if(current < PORTCOUNT)
        {
            currentEnd = channels[current];
        }
        switch(current)
        {
            case 0: //turn channel 1 on
                break;
            case 1: //...
                break;
            case 2: 
                break;
            case 3: //turn channel 4 on
                break;
            case 19://turn channel 0 on
                break;
            default://no port for this cycle defined
                break;
        }
            
    }
    if(currentEnd == counter)
    {
        switch(current)
        {
        case 0://clear channel 0 
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
        }
    }

ISR(TIMER1_COMPA_vect)
    LEDPORT ^= LEDBIT;
}

int main(void)
{
    clock_prescale_set(clock_div_1);    // systemtakt teiler
    MCUCR = (1 << JTD);                 //jtag ausschalten
    MCUCR = (1 << JTD);                 //this need to be executed twice 
        
        
    /* Timer/Counter 1 für Interrupts aufsetzen */
    TCCR1A = 0;
    TCCR1B = 5 << CS10 | 1 << WGM12;
    OCR1A = 8192;
    TIMSK1 = 1 << OCIE1A;
    TIFR1 = 1 << OCF1A;

    /* Timer0 for PWM*/
    TCCR0A = (1 << WGM01) | (0 << WGM00);
    TCCR0B = 

    DDRB = DDRC = DDRD = DDRF = 0xFF;
    PORTB = PORTC = PORTD = PORTF = 0x00;
    LEDDDR = LEDBIT;

    sei();

    while (1);
}
