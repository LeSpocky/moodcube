#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "gamma.h"
#include "hsv2rgb.h"

#define DIR_RD          (1 << DDB0)
#define DIR_GN          (1 << DDB1)
#define DIR_BL          (1 << DDB2)
#define PORT_RD         (1 << PORTB0)
#define PORT_GN         (1 << PORTB1)
#define PORT_BL         (1 << PORTB2)
#define PORT_MASK       (PORT_RD | PORT_GN | PORT_BL)
#define INVERTED_LED    false

#define MASK_ADPS       0x07    /*  ADPS2:ADPS0 in ADCSRA   */
#define MASK_ADTS       0x07    /*  ADTS2:ADTS0 in ADCSRB           */
#define MASK_REFS       0xC0    /*  REFS1,REFS0 in ADMUX    */
#define MASK_MUX        0x3F    /*  MUX5:MUX0 in ADMUX      */

/*  globals */
volatile uint8_t adc_result[3];
volatile uint16_t comp_buf_R, comp_buf_G, comp_buf_B;

/*  declaration */
void init( void );

/*  ISR */
ISR(TIM0_OVF_vect) {
    static uint8_t  pin_level = ( INVERTED_LED ? 0 : PORT_MASK );
    static uint8_t  led_status = 0;
    static uint16_t comp_R, comp_G, comp_B;
    static uint16_t soft_cnt_R = 0x01FF;
    static uint16_t soft_cnt_G = 0x01FF - 170;
    static uint16_t soft_cnt_B = 0x01FF - 340;

    /*  set new pin level first for low jitter and do calculation
     *  afterwards  */
    PORTB = pin_level;

    /*  increment counter and if overflow update compare value from
     *  main loop buffer and set LED on */
    soft_cnt_R++;
    soft_cnt_R &= 0x01FF;
    if ( soft_cnt_R == 0 ) {
        comp_R = comp_buf_R;
        led_status |= PORT_RD;
    }
    soft_cnt_G++;
    soft_cnt_G &= 0x01FF;
    if ( soft_cnt_G == 0 ) {
        comp_G = comp_buf_G;
        led_status |= PORT_GN;
    }
    soft_cnt_B++;
    soft_cnt_B &= 0x01FF;
    if ( soft_cnt_B == 0 ) {
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

ISR(ADC_vect) {
    static uint8_t adc_ch = 0;

    /*  get */
    adc_result[adc_ch] = ADCH;

    /*  set next channel    */
    if ( ++adc_ch == 3 ) adc_ch = 0;
    ADMUX = (ADMUX & ~MASK_MUX) | (adc_ch & MASK_MUX);
}

/*  main    */
int main( void ) {
    uint8_t h, s, v, r, g, b;

    init();
    init8();

    /*  main loop   */
    while ( 1 ) {
        /*  seems like potis are wired inverted */
        h = 255 - adc_result[0];
        s = 255 - adc_result[1];
        v = 255 - adc_result[2];

        rgb( h, s, v, &r, &g, &b );

        comp_buf_R = pgm_read_word( &(gammatbl[r]) );
        comp_buf_G = pgm_read_word( &(gammatbl[g]) );
        comp_buf_B = pgm_read_word( &(gammatbl[b]) );
    }

    return 0;
}

/*  implementation  */
void init( void ) {
    /*  start values for PWM    */
    comp_buf_R = 0;
    comp_buf_G = 0;
    comp_buf_B = 0;

    /*  start results for adc    */
    adc_result[0] = 0;
    adc_result[1] = 0;
    adc_result[2] = 0;

    /*  disable interrupts and ADC  */
    cli();
    ADCSRA &= ~(1 << ADEN);
    PRR &= ~(1 << PRADC);   /*  disable power reduction for ADC after
                                disabling ADC itself    */

    /*  set clock   */
    CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
    CLKPR = 0;              /*  set clock to maximum                */

    /*  set port pins to output and value 0 */
    DDRB = DIR_RD | DIR_GN | DIR_BL;
    PORTB = 0;

    /*  timer init  */
    TIFR0 &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
    TIMSK0 |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

    /*  start timer0 by setting last 3 bits in timer0 control register B
     *  to any clock source */
    TCCR0B = (TCCR0B & 0xF8) | (0x01 & 0x07);

    /*  use VCC as voltage reference (disconnected from PB0/AREF)   */
    ADMUX = (ADMUX & ~MASK_REFS) | 0;   /*  REFS2:REFS0 all cleared */

    /*  put conversion result left bounded in ADCH/ADCL, so we only need
     *  to fetch ADCH for an 8-bit result   */
    ADCSRB |= (1 << ADLAR);

    /*  set which ADC pin is used, start with 0   */
    ADMUX = (ADMUX & ~MASK_MUX) | (0 & MASK_MUX);

    /*  set prescaler in a way the ADC clock gets between 50khZ and
     *  200 kHz. using a prescaler of 128 here yields a value in this
     *  interval for both 8MHz and 16MHz clock. */
    /*  TODO    check this for default 1 MHz or make dependent on cpu
     *          clock speed!    */
    ADCSRA = (ADCSRA & ~MASK_ADPS) | (0x07 & MASK_ADPS);

    /*  use ADC free running mode   */
    ADCSRA |= (1 << ADATE);
    ADCSRB = (ADCSRB & ~MASK_ADTS) | 0; /*  ADTS2:ADTS0 all cleared */

    /*  enable interrupts and ADC   */
    ADCSRA |= (1 << ADEN);
    ADCSRA |= (1 << ADIE);
    sei();

    /*  start ADC conversion    */
    ADCSRA |= (1 << ADSC);
}
