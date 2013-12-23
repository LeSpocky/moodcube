#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DIR_RD          (1 << DDB0)
#define DIR_GN          (1 << DDB1)
#define DIR_BL          (1 << DDB2)
#define PORT_RD         (1 << PORTB0)
#define PORT_GN         (1 << PORTB1)
#define PORT_BL         (1 << PORTB2)
#define PORT_MASK       (PORT_RD | PORT_GN | PORT_BL)
#define INVERTED_LED    false

/*  constants   */
const uint8_t pwmtable[32] = {
    0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11, 13, 16, 19, 23, 27,
    32, 38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215, 255
};

/*  globals */
volatile uint8_t adc_result;
volatile uint8_t comp_buf_R, comp_buf_G, comp_buf_B;

/*  declaration */
void init( void );

/*  ISR */
ISR(TIM0_OVF_vect) {
    static uint8_t  pin_level = ( INVERTED_LED ? 0 : PORT_MASK );
    static uint8_t  led_status = 0;
    static uint8_t  comp_R, comp_G, comp_B;
    static uint8_t  soft_cnt_R = 0xFF;
    static uint8_t  soft_cnt_G = 0xFF - 85;
    static uint8_t  soft_cnt_B = 0xFF - 170;

    /*  set new pin level first for low jitter and do calculation
     *  afterwards  */
    PORTB = pin_level;

    /*  increment counter and if overflow update compare value from
     *  main loop buffer and set LED on */
    if ( ++soft_cnt_R == 0 ) {
        comp_R = comp_buf_R;
        led_status |= PORT_RD;
    }
    if ( ++soft_cnt_G == 0 ) {
        comp_G = comp_buf_G;
        led_status |= PORT_GN;
    }
    if ( ++soft_cnt_B == 0 ) {
        comp_B = comp_buf_B;
        led_status |= PORT_BL;
    }

    /*  on compare match set LED off (written to port on next
     *  interrupt)  */
    if ( comp_R == soft_cnt_R ) {
        led_status &= ~PORT_RD;
    }
    if ( comp_G == soft_cnt_G ) {
        led_status &= ~PORT_GN;
    }
    if ( comp_B == soft_cnt_B ) {
        led_status &= ~PORT_BL;
    }

    pin_level = ( INVERTED_LED ? ~led_status : led_status ) & PORT_MASK;
}

/*  main    */
int main (void) {
    int i;

    init();

    /*  main loop   */
    while ( 1 ) {
        for ( i = 0; i < 32; i++ ) {
            comp_buf_R = pwmtable[i];
            comp_buf_G = pwmtable[i];
            comp_buf_B = pwmtable[i];
            _delay_ms(50);
        }
        _delay_ms(1000);
        for ( i = 31; i >= 0; i-- ) {
            comp_buf_R = pwmtable[i];
            comp_buf_G = pwmtable[i];
            comp_buf_B = pwmtable[i];
            _delay_ms(50);
        }
        _delay_ms(1000);
    }

    return 0;
}

/*  implementation  */
void init( void ) {
    /*  start values for PWM    */
    comp_buf_R = 10;
    comp_buf_G = 80;
    comp_buf_B = 170;

    /*  disable interrupts and ADC  */
    cli();
//    ADCSRA &= ~(1 << ADEN);
//    PRR &= ~(1 << PRADC);   /*  disable power reduction for ADC after
//                                disabling ADC itself    */

    /*  set clock   */
    CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
    CLKPR = 0;              /*  set clock to maximum                */

    /*  timer init  */
    TIFR0 &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
    TIMSK0 |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

    /*  start timer0 by setting last 3 bits in timer0 control register B
     *  to any clock source */
    TCCR0B = (TCCR0B & 0xF8) | (0x01 & 0x07);

    /*  enable interrupts and ADC   */
//    ADCSRA |= (1 << ADEN);
//    ADCSRA |= (1 << ADIE);
    sei();

    /*  set port pins to output and value 0 */
    DDRB = DIR_RD | DIR_GN | DIR_BL;
    PORTB = 0;
}
