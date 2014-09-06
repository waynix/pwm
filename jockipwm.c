#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define LEDDDR DDRE
#define LEDPORT PORTE
#define LEDBIT (1 << PE6)
#define SERVO0_DDR  PORTF
#define SERVO_PORT PORTF
#define SERVO0_PIN 0
#define SERVO1_DDR  PORTF
#define SERVO1_PORT PORTF
#define SERVO1_PIN 1
#define SERVO2_DDR  PORTF
#define SERVO2_PORT PORTF
#define SERVO2_PIN 2
const uint8_t  MAX_PORTCOUNT = 20;
const uint8_t  PORTCOUNT = 5;

uint8_t channels[5];
const uint16_t ONE_MILLISECOND = 0x00B0;


ISR(TIMER3_COMPA_vect)
{
    static int8_t nextServo = 0;
    static int16_t usedTime = 0;
    int16_t nextTimeout = 0;
    if(nextServo != 3)
    {
        SERVO_PORT = 1 << nextServo;
        nextTimeout = 0x00FA + channels[nextServo];
        usedTime += nextTimeout;
        nextServo++;
    }
    else
    {
        SERVO_PORT = 0;
        nextServo = 0;
        nextTimeout = 5000 - usedTime;
        usedTime = 0;
    }
    OCR3A = nextTimeout;
}

ISR(TIMER1_COMPA_vect)
{
    LEDPORT ^= LEDBIT;
    channels[0]+=1;
    channels[1]-=1;
}

/*void serialInit(void)
{
    //USART configuration
//  //clear configuration registers
//  UCSR0A = 0;
//  UCSR0B = 0;
//  UCSR0C = 0;
    //Baud Rate 768000Bit/s (at 16MHZ clock)
    UBRR0H = (unsigned char)0;
    UBRR0L = (unsigned char)25;
    //Double Data Rate
    UCSR0A  = (0 << U2X0)
    //Multiprocessor Communication Mode
            | (0 << MPCM0);
    //State: (1= on 0 =off)
    UCSR0B =  (1 << RXEN0) //Receiver
            | (1 << TXEN0)//Transmitter
    //Interrupts:(1= on 0 =off)
            | (1 << RXCIE0) //Receive
            | (0 << TXCIE0) //Transmit
            | (0 << UDRIE0) //Send buffer free
    //Bit Count:8
            | (0 << UCSZ02);
    UCSR0C  = (1 << UCSZ01)
            | (1 << UCSZ00)
    //CRC Mode: even
            |  (1 << UPM01)
            | (0 << UPM00)
    //Stop Bits: 1bit = 0 2bits = 1
            | (0 << USBS0)
    //USART Mode: asynchron
            | (0 << UMSEL01)
            | (0 << UMSEL00);
}*/

int main(void)
{
    clock_prescale_set(clock_div_1);    // systemtakt teiler
    MCUCR = (1 << JTD);                 //jtag ausschalten
    MCUCR = (1 << JTD);                 //this need to be executed twice 
        
        
    // Timer/Counter 1 für Interrupts aufsetzen
    TCCR1A = 0;
    TCCR1B = 5 << CS10 | 1 << WGM12;
    OCR1A = 410;
    TIMSK1 = 1 << OCIE1A;
    TIFR1 = 1 << OCF1A;

    // Timer0 for PWM select clear timer on compare mode
    TCCR3A = (0 << WGM31) | (0 << WGM30);
    TCCR3B = (0 << WGM33) | (1 << WGM32);
    // The Clocdivider is set to 1/64 this should generate a clock of 250kHz so that that one Timerclock is approximately 1/256(exactly 1/250)ms.
    // Using this it will be possible to use an eight bit register to control fine steps between 1 and 2 ms
    TCCR3B |= (0 << CS32) | (1 << CS31) | (1 << CS30);
    TIMSK3 = (1 << OCIE3A);
    //Set the timer intervall to 20ms for the first cyle
    OCR3A = 5000;
    DDRB = DDRC = DDRD = DDRF = 0xFF;
    PORTB = PORTC = PORTD = PORTF = 0x00;
    LEDDDR = LEDBIT;
    SERVO_PORT = (1 << SERVO0_PIN);
    channels[0] = 0;
    channels[1] = 0xFF;
    sei();

    while (1)
    {
    };
}
