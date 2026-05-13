[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/MCJunYEq)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=23739449&assignment_repo_type=AssignmentRepo)
# Lab06: Comunicación UART con PIC18F45K22

## Integrantes

* [Liliana Carreño](https://github.com/Liliana-Carreno)
* [Salome Ramirez](https://github.com/salomeramirezpi-eng)
* [Gabriel Ortega](https://github.com/gabrieldaortegaro-arch)

### 1.1 Introducción

<p align="justify" style="text-indent:40px;">
En este laboratorio se implementó la configuración del módulo MSSP para establecer comunicación con una pantalla LCD 16×2 mediante un adaptador I²C que reduce la cantidad de pines necesarios. Durante el desarrollo se implementaron módulos en archivos .h y .c para organizar el código de comunicación I²C y control de la LCD.
</p>

<p align="justify" style="text-indent:40px;">
El primer programa permitió visualizar un texto estático en la primera línea de la pantalla y un mensaje desplazable en la segunda línea, demostrando el manejo dinámico de cadenas de caracteres. Posteriormente, se desarrolló una animación de un caballo en movimiento utilizando caracteres personalizados y módulos adicionales para controlar la secuencia de animación. Con esto se reforzó el uso de comunicación serial I²C, modularización del código y manipulación gráfica básica en pantallas LCD.
</p>


### 1.2 Objetivos

* Configurar el módulo MSSP del PIC18F45K22 en modo maestro para establecer comunicación mediante protocolo I²C.

* Implementar funciones modulares en archivos .h y .c para controlar una pantalla LCD 16×2 utilizando el adaptador PCF8574.

* Desarrollar aplicaciones visuales en la LCD, incluyendo texto estático, desplazamiento de cadenas y animaciones mediante caracteres personalizados.


## Documentación

### 2.1 Descripción del Laboratorio.

<p align="justify" style="text-indent:40px;">
El desarrollo del laboratorio se organizó de manera modular mediante con archivos i2c.h, i2c.c, i2c_lcd.h e i2c_lcd.c (Codigos que comparten ambos proyectos), que implementarban las funciones necesarias para inicializar, enviar y mostrar caracteres en la LCD.
</p>
<p align="justify" style="text-indent:40px;">
Primero se mostró un texto estático en la primera línea de la pantalla (ORNITORRINCO) mientras que en la segunda línea se realizó el desplazamiento (). Posteriormente, se desarrolló una animación de un caballo en movimiento utilizando caracteres personalizados donde fueron necesarios unos módulos adicionales previamente usados en laboratorios para controlar los frames de animación y el desplazamiento horizontal.
</p>
<p align="justify" style="text-indent:40px;">
Con este laboratorio se reforzaron conceptos relacionados con protocolos de comunicación serial, modularización de código, manejo de pantallas LCD y creación de interfaces visuales básicas en sistemas embebidos. Porque claramente la humanidad decidió que hacer caminar un caballo pixelado sobre 16 caracteres era un paso indispensable hacia el progreso tecnológico.
</p>

### 2.2 Explicacion del codigo implementado.

# Descripción del módulo `i2c.h`

```c
#ifndef I2C_H
#define I2C_H

#include <xc.h>

#define ANSEL_SCL ANSELCbits.ANSC3
#define ANSEL_SDA ANSELCbits.ANSC4

#define TRIS_SCL TRISCbits.TRISC3
#define TRIS_SDA TRISCbits.TRISC4

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(unsigned char data);

#endif
```

En este bloque se define el archivo de cabecera encargado de la comunicación I²C.  
Se incluye la librería `<xc.h>` para acceder a los registros internos del PIC. Además, se crean macros para configurar los pines RC3 y RC4 como líneas SCL y SDA, tanto en modo digital como en dirección de entrada/salida.  

Finalmente, se declaran los prototipos de las funciones principales del módulo I²C, permitiendo inicializar el protocolo, generar condiciones de inicio y parada, y enviar datos al dispositivo esclavo. Básicamente, aquí se deja listo el “manual de instrucciones” para que el resto del programa no termine escribiendo jeroglíficos eléctricos.

---

# Descripción de la función `I2C_init()`

```c
void I2C_init(void)
{
    TRIS_SCL = 1;
    TRIS_SDA = 1;
    
    ANSEL_SCL = 0;
    ANSEL_SDA = 0; 
    
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
    SSPADD = 119;
    SSPCON1bits.SSPEN = 1;
}
```

En esta función se realiza la configuración inicial del módulo MSSP del PIC para trabajar en modo I²C maestro.  

Primero, los pines SCL y SDA se configuran como entradas, ya que el protocolo I²C maneja líneas compartidas con resistencias pull-up. Luego, se deshabilita la función analógica de los pines RC3 y RC4 para que funcionen únicamente como señales digitales.  

Posteriormente, se configuran los registros `SSPSTAT`, `SSPCON1`, `SSPCON2` y `SSPADD`, definiendo el modo maestro y la velocidad de comunicación. Finalmente, se habilita el módulo MSSP mediante el bit `SSPEN`. Traducción humana: el PIC deja de improvisar y empieza a hablar I²C de manera civilizada.

---

# Descripción de la función `I2C_start()`

```c
void I2C_start(void)
{
    SSPCON2bits.SEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
```

Esta función genera la condición de inicio (START) del protocolo I²C.  

Al activar el bit `SEN`, el microcontrolador indica el comienzo de la transmisión de datos sobre el bus I²C. Luego, el programa espera hasta que la bandera `SSPIF` indique que la operación fue completada correctamente. Finalmente, la bandera se limpia para futuras operaciones.

---

# Descripción de la función `I2C_stop()`

```c
void I2C_stop(void)
{
    SSPCON2bits.PEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
```

En esta función se genera la condición de parada (STOP) del protocolo I²C.  

El bit `PEN` permite finalizar la transmisión de datos en el bus. Después, el programa espera a que el hardware confirme que la operación terminó correctamente y limpia la bandera de interrupción correspondiente.

---

# Descripción de la función `I2C_write()`

```c
void I2C_write(unsigned char data)
{
    SSPBUF = data;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
```

Esta función permite enviar un byte de información mediante el bus I²C.  

El dato recibido como parámetro se almacena en el registro `SSPBUF`, encargado de transmitir la información serialmente. El programa espera hasta que la transmisión finalice y posteriormente limpia la bandera `SSPIF`.  

Con esta función se pueden enviar comandos, direcciones o caracteres hacia dispositivos esclavos, como la LCD con adaptador PCF8574. Pequeño detalle elegante: toda la comunicación visual del laboratorio depende de este byte viajando disciplinadamente por dos cables.

---

# Descripción del módulo `i2c_lcd.h`

```c
#ifndef LCD_I2C_H
#define LCD_I2C_H

#define _XTAL_FREQ 48000000UL
#define ADDRESS_LCD 0x4E

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_write_char(char c);
void lcd_write_string(const char *str);
void lcd_clear(void);
void lcd_create_char(unsigned char location, unsigned char *charmap);

#endif
```

En este bloque se define el archivo de cabecera para el control de la pantalla LCD mediante I²C.  

Se establece la frecuencia de trabajo del microcontrolador con `_XTAL_FREQ`, necesaria para utilizar funciones de retardos. Además, se define la dirección I²C de la LCD (`0x4E`) correspondiente al módulo PCF8574.  

También se declaran las funciones encargadas de inicializar la pantalla, enviar comandos, posicionar el cursor, escribir caracteres, mostrar cadenas completas, limpiar la pantalla y crear caracteres personalizados.

---

# Descripción de la función `lcd_init()`

```c
void lcd_init(void)
{
    __delay_ms(20);
    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    __delay_ms(3);
}
```

Esta función inicializa la pantalla LCD en modo de comunicación de 4 bits.  

Inicialmente se realiza un pequeño retardo para asegurar la estabilización de la pantalla. Luego, se envían varios comandos de configuración que permiten definir el modo de operación, activar el display, configurar el cursor y limpiar la pantalla.  

El último retardo garantiza que la LCD termine correctamente el proceso de inicialización antes de recibir nuevos datos. Porque las LCD antiguas tienen el temperamento de una impresora de los 90: si no esperas el tiempo exacto, simplemente dejan de cooperar.

---

# Descripción de la función `lcd_cmd()`

```c
void lcd_cmd(unsigned char cmd)
{
    char data_u, data_l;
    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xF0);

    I2C_start();
    I2C_write(ADDRESS_LCD);
    I2C_write(data_u | 0x0C);
    I2C_write(data_u | 0x08);
    I2C_write(data_l | 0x0C);
    I2C_write(data_l | 0x08);
    I2C_stop();
}
```

Esta función envía comandos de control a la pantalla LCD mediante el protocolo I²C.  

El comando se divide en dos partes de 4 bits: parte alta (`data_u`) y parte baja (`data_l`), ya que la LCD trabaja en modo de 4 bits. Posteriormente, se inicia la comunicación I²C y se envían los datos junto con señales de control para habilitar la escritura en la pantalla.  

Finalmente, se cierra la comunicación con la condición STOP.

---

# Descripción de la función `lcd_write_char()`

```c
void lcd_write_char(char c)
{
    char data_u, data_l;
    data_u = (c & 0xF0);
    data_l = ((c << 4) & 0xF0);

    I2C_start();
    I2C_write(ADDRESS_LCD);
    I2C_write(data_u | 0x0D);
    I2C_write(data_u | 0x09);
    I2C_write(data_l | 0x0D);
    I2C_write(data_l | 0x09);
    I2C_stop();
}
```

Esta función permite escribir un carácter individual en la pantalla LCD.  

El carácter también se divide en dos partes de 4 bits para adaptarse al modo de operación de la LCD. Luego, los datos son enviados mediante I²C junto con los bits de control necesarios para indicar que la información corresponde a datos y no a comandos.


```c
void lcd_set_cursor(unsigned char row, unsigned char col)
{
    if (row == 0) lcd_cmd(0x80 + col);
    else lcd_cmd(0xC0 + col);
}
```

Esta función posiciona el cursor de la pantalla LCD en una fila y columna específicas.  

Si la fila seleccionada es la primera, se utiliza la dirección base `0x80`; de lo contrario, se utiliza `0xC0` para la segunda fila. Luego, se envía el comando correspondiente a la LCD.


```c
void lcd_write_string(const char *str)
{
    while(*str != '\0')
    {
        lcd_write_char(*str++);
    }
}
```

Esta función permite mostrar una cadena completa de caracteres en la pantalla LCD.  

El programa recorre el string carácter por carácter hasta encontrar el terminador nulo (`'\0'`). Cada carácter es enviado individualmente utilizando la función `lcd_write_char()`.


```c
void lcd_clear(void)
{
    lcd_cmd(0x01);
    __delay_ms(2);
}
```

Esta función limpia completamente el contenido mostrado en la pantalla LCD.  

El comando `0x01` borra todos los caracteres y devuelve el cursor a la posición inicial. Luego, se realiza un pequeño retardo para asegurar que la LCD complete correctamente la operación.


```c
void lcd_create_char(unsigned char location, unsigned char *charmap)
{
    location &= 0x07;

    lcd_cmd(0x40 | (location << 3));

    for(unsigned char i = 0; i < 8; i++)
    {
        lcd_write_char(charmap[i]);
    }

    lcd_cmd(0x80);
}
```

Esta función permite crear caracteres personalizados en la memoria CGRAM de la pantalla LCD.  

Primero, se limita la ubicación del carácter entre 0 y 7, ya que la LCD solo permite almacenar ocho caracteres personalizados simultáneamente. Luego, se selecciona la dirección de memoria correspondiente y se envían los 8 bytes que definen el diseño del carácter fila por fila.  

Finalmente, el cursor regresa a la memoria principal de la pantalla. Gracias a esta función fue posible construir animaciones y gráficos personalizados, porque aparentemente ver un caballo pixelado caminar en 16×2 sigue produciendo satisfacción científica legítima.



## Diagramas

## Evidencias de implementación

## Conclusiones