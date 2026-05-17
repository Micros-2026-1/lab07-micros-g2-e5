#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 48000000UL

#include <xc.h>

#include "i2c.h"
#include "i2c_lcd.h"


// -------- FUNCION SCROLL SOLO FILA 2 --------
void scrollLinea2(const char *texto)
{
    unsigned int len = 0;

    while(texto[len] != '\0')
        len++;

    while(1)
    {
        for(unsigned int inicio = 0; inicio < len + 16; inicio++)
        {
            // Cursor en segunda fila
            lcd_set_cursor(1,0);

            for(unsigned char k = 0; k < 16; k++)
            {
                unsigned int pos = inicio + k;

                if(pos < len)
                    lcd_write_char(texto[pos]);
                else
                    lcd_write_char(' ');
            }

            __delay_ms(200);
        }
    }
}



// -------- MAIN --------
void main(void)
{
    // Oscilador interno
    OSCCON = 0x70;

    // Desactivar analogicos
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;

    // Inicializar I2C
    I2C_init();

    __delay_ms(100);

    // Inicializar LCD
    lcd_init();

    lcd_clear();

    // -------- TEXTO FIJO FILA 1 --------
    lcd_set_cursor(0,0);
    lcd_write_string("ORNITORRINCO");

    // -------- SCROLL FILA 2 --------
    scrollLinea2("¿Un ornitorrinco?...... !!PERRY EL ORNITORRINCO!!   ");
}