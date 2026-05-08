#include <xc.h>

#include "i2c.h"
#include "i2c_lcd.h"
#include "caballo.h"

// ================================================================
// CGRAM
// ================================================================

void Personalizado(unsigned char location, const unsigned char charmap[])
{
    location &= 0x07;

    // Direccion CGRAM
    lcd_cmd(0x40 | (location << 3));

    __delay_ms(2);

    for(int i = 0; i < 8; i++)
    {
        lcd_write_char(charmap[i] & 0x1F);
        __delay_ms(2);
    }

    // Volver DDRAM
    lcd_cmd(0x80);

    __delay_ms(2);
}

void CargarFrame(const unsigned char frame[8][8])
{
    for(int i = 0; i < 8; i++)
    {
        Personalizado(i, frame[i]);
    }
}

// ================================================================
// FRAMES
// ================================================================

const unsigned char cab1[8][8] = {

    {0b00000,0b00000,0b00000,0b00111,0b01111,0b01110,0b11100,0b11000},
    {0b00000,0b00000,0b00000,0b01111,0b11111,0b11111,0b11111,0b11111},
    {0b00000,0b00000,0b00001,0b00011,0b11111,0b11111,0b11111,0b11111},
    {0b10100,0b11100,0b11010,0b11110,0b11111,0b10011,0b10000,0b10000},
    {0b10011,0b10011,0b00010,0b00010,0b00100,0b00000,0b00000,0b00000},
    {0b11111,0b00111,0b01110,0b00110,0b00001,0b00000,0b00000,0b00000},
    {0b11111,0b00011,0b00011,0b00001,0b00001,0b00000,0b00000,0b00000},
    {0b10000,0b11000,0b01100,0b00010,0b00010,0b10001,0b00000,0b00000}
};

const unsigned char cab2[8][8] = {

    {0b00000,0b00000,0b00000,0b00000,0b00011,0b00111,0b01110,0b01110},
    {0b00000,0b00000,0b00000,0b00000,0b01111,0b11111,0b11111,0b11111},
    {0b00000,0b00000,0b00000,0b00011,0b00111,0b11111,0b11111,0b11111},
    {0b00000,0b00000,0b00101,0b11111,0b11101,0b11111,0b10110,0b00110},
    {0b01100,0b11000,0b10000,0b00000,0b00001,0b00001,0b00001,0b00000},
    {0b11111,0b11111,0b11110,0b10111,0b00000,0b00000,0b10000,0b00000},
    {0b11111,0b11111,0b00011,0b00010,0b10100,0b00100,0b00010,0b00000},
    {0b11100,0b11100,0b00100,0b00100,0b01000,0b00000,0b00000,0b00000}
};

const unsigned char cab3[8][8] = {

    {0b00000,0b00000,0b00000,0b00000,0b00000,0b00111,0b01111,0b11110},
    {0b00000,0b00000,0b00000,0b00000,0b01110,0b11111,0b11111,0b11111},
    {0b00000,0b00000,0b00000,0b00001,0b00111,0b11111,0b11111,0b11111},
    {0b00000,0b00101,0b11111,0b11101,0b11111,0b11011,0b10011,0b10000},
    {0b10011,0b00011,0b00110,0b01100,0b10000,0b10000,0b00000,0b00000},
    {0b11111,0b10111,0b00110,0b01100,0b10000,0b10000,0b00000,0b00000},
    {0b11111,0b11111,0b00001,0b00001,0b00001,0b00001,0b00000,0b00000},
    {0b10000,0b11000,0b11100,0b01000,0b01000,0b00000,0b10000,0b00000}
};

// ================================================================
// MOSTRAR CABALLO
// ================================================================

static void MostrarEnPos(unsigned char col)
{
    lcd_set_cursor(0, col);

    lcd_write_char(0);
    lcd_write_char(1);
    lcd_write_char(2);
    lcd_write_char(3);

    lcd_set_cursor(1, col);

    lcd_write_char(4);
    lcd_write_char(5);
    lcd_write_char(6);
    lcd_write_char(7);
}

// ================================================================
// BORRAR COLUMNA
// ================================================================

static void BorrarColumna(unsigned char col)
{
    lcd_set_cursor(0, col);

    lcd_write_char(' ');

    lcd_set_cursor(1, col);

    lcd_write_char(' ');

    __delay_ms(5);
}

// ================================================================
// ANIMACION PRINCIPAL
// ================================================================

void CabTrot(void)
{
    lcd_clear();

    const unsigned char (*frames[3])[8] = {
        cab1,
        cab2,
        cab3
    };

    for(unsigned char pos = 0; pos <= 12; pos++)
    {
        // Borrar rastro anterior
        if(pos >= 1)
        {
            BorrarColumna(pos - 1);

            __delay_ms(10);
        }

        // Animar patas
        for(int f = 0; f < 3; f++)
        {
            CargarFrame(frames[f]);

            MostrarEnPos(pos);

            __delay_ms(50);
        }
    }

    __delay_ms(300);

    lcd_clear();

    __delay_ms(100);
}