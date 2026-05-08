#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 16000000UL

#include <xc.h>

#include "i2c.h"
#include "i2c_lcd.h"
#include "caballo.h"

void main(void)
{
    OSCCON = 0x70;

    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;

    I2C_init();

    __delay_ms(100);

    lcd_init();

    lcd_clear();

    lcd_set_cursor(0,0);
    lcd_write_string("Caballo I2C");

    __delay_ms(2000);

    while(1)
    {
        CabTrot();
    }
}