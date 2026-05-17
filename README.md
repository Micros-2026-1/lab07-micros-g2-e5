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


## i2c.h

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

<p align="justify" style="text-indent:40px;">
Este archivo contiene las definiciones necesarias para configurar y utilizar la comunicación serial mediante el módulo MSSP. Las directivas #ifndef, #define y #endif evitan múltiples inclusiones del archivo durante la compilación, previniendo errores de redefinición. La librería <xc.h> permite acceder a los registros internos del PIC, mientras que las macros simplifican el acceso a los bits asociados a los pines SCL y SDA. Finalmente, se declaran los prototipos de las funciones encargadas de inicializar el módulo I2C, generar condiciones START y STOP, y transmitir datos a través del bus.
</p>

## i2c.c

```c
#include "i2c.h"
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
<p align="justify" style="text-indent:40px;">
Este archivo incluye la cabecera i2c.h, permitiendo utilizar todas las definiciones. Primero se inicializa el módulo MSSP en modo I2C Maestro y se configuran los pines SCL y SDA como entradas, debido a que el protocolo I2C trabaja con líneas  mediante resistencias pull-up y se desactiva la función analógica de ambos pines para asegurar su funcionamiento digital. Los registros SSPSTAT, SSPCON1 y SSPCON2 establecen la configuración del módulo MSSP, habilitando el modo I2C Master y definiendo el comportamiento de la comunicación serial. El valor cargado en SSPADD ajusta la frecuencia de operación aproximadamente a 100 kHz, mientras que el bit SSPEN habilita finalmente el funcionamiento del periférico.
</p>

```c
void I2C_start(void)
{
    SSPCON2bits.SEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}

void I2C_stop(void)
{
    SSPCON2bits.PEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
```

<p align="justify" style="text-indent:40px;">
Esta función genera la condición START para iniciar cualquier transmisión sobre el bus. El bit SEN produce la secuencia de inicio entre las líneas SDA y SCL, el programa permanece en espera mediante un ciclo while hasta que la bandera SSPIF indica que la operación ha finalizado correctamente y finalmente, dicha bandera se limpia manualmente para preparar el módulo para futuras operaciones de comunicación. La siguiente función genera la condición STOP para finalizar una transmisión y liberar el bus de comunicación, el bit PEN activa automáticamente la secuencia y el ciclo de espera verifica continuamente la bandera SSPIF hasta confirmar que concluyó, entonces la bandera de interrupción es reiniciada manualmente para evitar conflictos en futuras transmisiones.
</p>

```c
void I2C_write(unsigned char data)
{
    SSPBUF = data;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
```
<p align="justify" style="text-indent:40px;">
Esta función transmite un byte de información, el dato recibido como parámetro es cargado en el registro SSPBUF, lo que inicia automáticamente la transmisión serial, entonces, el programa espera hasta que la bandera SSPIF indique que el envío del dato ha finalizado y la bandera es limpiada manualmente para dejar el periférico preparado para futuras transmisiones.
</p>

## i2c_lcd.h

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

<p align="justify" style="text-indent:40px;">
Aqui encontramos las definiciones y prototipos necesarios para controlar una pantalla LCD mediante comunicación I2C. La constante _XTAL_FREQ define la frecuencia de operación del microcontrolador y es utilizada por las funciones de retardo del compilador. La constante ADDRESS_LCD almacena la dirección I2C del módulo adaptador conectado al LCD. Además, se declaran las funciones encargadas de inicializar la pantalla, enviar comandos, posicionar el cursor, escribir caracteres y cadenas de texto, limpiar el display y crear caracteres personalizados dentro de la memoria CGRAM del LCD.
</p>

## i2c_lcd.c

```c
#include <xc.h>
#include "i2c.h"
#include "i2c_lcd.h"

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

<p align="justify" style="text-indent:40px;">
Se incluyen los .h y .c antes de definir la función que inicializa la pantalla LCD en modo de comunicación de 4 bits siguiendo la secuencia requerida por el controlador HD44780,se aplica un retardo para asegurar la estabilización del display. Los comandos enviados configuran el modo de operación, habilitan dos líneas de visualización, desactivan el cursor visible y establecen el desplazamiento automático del cursor después de cada escritura. Finalmente, el comando de limpieza borra cualquier información previa almacenada en pantalla y el retardo adicional garantiza que el LCD complete correctamente la operación interna.
</p>

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
<p align="justify" style="text-indent:40px;">
Esta función permite enviar comandos de control al LCD utilizando comunicación I2C en modo de 4 bits. El comando recibido se divide en un nibble alto y un nibble bajo debido a la forma de operación interna del display. Posteriormente se inicia la comunicación I2C y se transmite la dirección del módulo adaptador. Cada nibble es enviado activando y desactivando el bit Enable mediante operaciones lógicas OR, generando así el pulso necesario para que el LCD almacene correctamente la información recibida. Finalmente, se genera la condición STOP para finalizar la transmisión.
</p>

```c
void lcd_set_cursor(unsigned char row, unsigned char col)
{
    if (row == 0) lcd_cmd(0x80 + col);
    else lcd_cmd(0xC0 + col);
}

void lcd_write_string(const char *str)
{
    while(*str != '\0')
    {
        lcd_write_char(*str++);
    }
}
```

<p align="justify" style="text-indent:40px;">
La primera función posiciona el cursor del LCD utilizando las direcciones internas de memoria DDRAM . Si se selecciona la primera fila, se utiliza la dirección base 0x80, mientras que para la segunda fila se utiliza 0xC0. La columna deseada se suma a la dirección correspondiente para ubicar exactamente la posición donde comenzará la escritura del siguiente carácter. Seguidamente la otra función permite escribir cadenas completas de texto en la pantalla LCD recorriendo secuencialmente cada carácter almacenado en memoria con un puntero El ciclo se ejecutara  hasta encontrar el carácter nulo '\0', el cual indica el final de la cadena.
</p>

```c
void lcd_clear(void)
{
    lcd_cmd(0x01);
    __delay_ms(2);
}

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

<p align="justify" style="text-indent:40px;">
Esta función limpia  el contenido de la pantalla LCD con el comando 0x01. Debido a que esta operación requiere un tiempo de procesamiento interno se implementa un retardo de 2 ms. La siguiente función permite almacenar caracteres personalizados dentro de la memoria CGRAM del LCD. Inicialmente se limita la variable location a valores entre 0 y 7, ya que el controlador únicamente admite ocho caracteres definidos por el usuario. Posteriormente se calcula la dirección correspondiente dentro de la memoria CGRAM utilizando desplazamiento de bits. El ciclo for recorre las ocho filas binarias que conforman el patrón gráfico del carácter y las escribe secuencialmente en la memoria interna del display. Finalmente, se retorna al modo normal de escritura sobre la memoria DDRAM para continuar mostrando texto convencional.
</p>


## main (CADENA DE TEXTO Y TEXTI ESTATICO)

```C
#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config WDTEN = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 48000000UL

#include <xc.h>

#include "i2c.h"
#include "i2c_lcd.h"
```

<p align="justify" style="text-indent:40px;">
Este bloque inicial configura los bits de configuración del microcontrolador PIC, definiendo parámetros fundamentales de funcionamiento del sistema. Se selecciona el oscilador interno como fuente de reloj, se desactiva el PLL, el Watchdog Timer y la programación en bajo voltaje para evitar reinicios o conflictos innecesarios durante la ejecución. Además, se deshabilita el modo analógico por defecto del PORTB. La constante _XTAL_FREQ define la frecuencia de operación utilizada por las funciones de retardo del compilador. Finalmente, se incluyen las librerías necesarias para acceder tanto a los registros internos del PIC como a las funciones de comunicación I2C y control del LCD implementadas en los módulos anteriores.
</p>


```C
void scrollLinea2(const char *texto)
{
    unsigned int len = 0;

    while(texto[len] != '\0')
        len++;

    while(1)
    {
        for(unsigned int inicio = 0; inicio < len + 16; inicio++)
        {
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
```
 <p align="justify" style="text-indent:40px;"> 
Aqui se implementa un efecto de desplazamiento horizontal de texto sobre la segunda fila del LCD. Inicialmente se calcula manualmente la longitud de la cadena recibida recorriendo el arreglo carácter por carácter hasta encontrar el terminador nulo '\0'. Posteriormente se utiliza un ciclo infinito para mantener el desplazamiento continuo del mensaje. La variable inicio controla la posición inicial visible del texto, mientras que el ciclo interno recorre las 16 columnas disponibles del display. En cada iteración se calcula la posición real del carácter a mostrar y, si dicha posición excede la longitud del mensaje, se escriben espacios en blanco para generar el efecto visual de salida progresiva del texto. Finalmente, se aplica un retardo de 200 ms para controlar la velocidad del desplazamiento.
</p>

```c
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
    lcd_write_string("ORNITORRINCO");

    scrollLinea2("¿Un ornitorrinco?...... !!PERRY EL ORNITORRINCO!!   ");
}
```

 <p align="justify" style="text-indent:40px;"> 
 La función principal realiza toda la configuración e inicialización primero se configura el oscilador interno mediante el registro OSCCON, estableciendo la frecuencia de operación y se deshabilitan las funciones analógicas de los puertos para el modo digital. Luego se inicializa el módulo I2C y se aplica un pequeño retardo para garantizar la estabilización al iniciar la pantalla LCD. Una vez inicializado el display, se limpia la pantalla y se posiciona el cursor al inicio de la primera fila para mostrar un texto fijo. Finalmente, se llama a la función scrollLinea2(), la cual ejecuta indefinidamente el desplazamiento del mensaje en la segunda línea del LCD. Básicamente el programa entra en un bucle permanente mostrando el texto animado indefinidamente.
 </p>

## main (CABALLO)

```c
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
```

 <p align="justify" style="text-indent:40px;"> 
Este bloque inicial establece la configuración principal del microcontrolador PIC y define los parámetros básicos de funcionamiento del sistema. Se selecciona el oscilador interno como fuente de reloj y se desactivan módulos como el PLL, el Watchdog Timer y la programación en bajo voltaje para evitar reinicios o interferencias innecesarias durante la ejecución. La constante _XTAL_FREQ define la frecuencia de trabajo utilizada por las funciones de retardo del compilador. Además, se incluyen las librerías necesarias para acceder a los registros internos del PIC, controlar la comunicación I2C, manejar el display LCD y utilizar las funciones de animación del caballo definidas en el módulo caballo.h.
 </p>

```c
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
```

 <p align="justify" style="text-indent:40px;"> 
Esta es la función principal, configura la inicialización necesaria para ejecutar la animación del caballo sobre la pantalla LCD. Inicialmente se activa oscilador interno y establece su frecuencia de operación, luego se deshabilitan las funciones analógicas para usarlos digital. Entonces se inicializa el módulo I2C y la pantalla LCD, aplicando un pequeño retardo. Después de limpiar la pantalla, se muestra el texto fijo “Caballo I2C”y seguidamente el programa entra en un ciclo infinito donde se ejecuta continuamente la función CabTrot(), encargada de realizar toda la animación del caballo, incluyendo los movimientos de galope mediante caracteres personalizados y el desplazamiento horizontal progresivo a través del display hasta llegar al extremo final de la pantalla, repitiendo posteriormente toda la secuencia de manera continua.
 </p>

## Diagramas

 <p align="justify" style="text-indent:40px;"> 
Para la conexión de este laboratorio nos guiamos por el esquema de la figura 1 que fue brindado por el profesor,  de igual manera vale la pena resaltar que el I2C es un elemento el cual facilita mucho la conexión de la LCD, ya que reduce los pines de conexión a solo 4, dos de estos son de alimentación y los otros dos para la sincronización y transmición de la información, por medio de un protocolo de esclavbo maestro.
 </p>

<p align="center">
  <img src="montaje (1).png" width="700"><br>
  <em> <b> Figura 1.</b> Imagen de referencia de la conexión entre los elementos. </em>
</p>

## Evidencias de implementación

 <p align="justify" style="text-indent:40px;"> 
El funcionamiento del montaje se puede observar a continuación en primera instancia, combinamos los dos primeros ejercicios y en la primera linea de la LCD se puede leer el texto estatico, mientras que en la segunda aparece el texto haciendo scroll por la pantalla. De igual forma vale la pena resaltar que es muy importante variar el potenciometro del I2C,para obtener un brillo aceptable o si no la pantalla parece apagada.
 </p>

<p align="center">
  <img src="dets.gif" width="700"><br>
  <em> <b> Figura 2.</b> Imagen del funcionamiento de la primera parte. </em>
</p>

<p align="justify" style="text-indent:40px;"> 
Para la segunda parte del laboratorio, se mostro en la LCD una animación, en nuestro caso fue la de un caballo trotando, esta se puede apreciar en la fiigura a continuación.
 </p>
 
<p align="center">
  <img src="sea.gif" width="700"><br>
  <em> <b> Figura 3.</b> Imagen animación. </em>
</p>

## Conclusiones

* Se pudo comprender, como funciona la comunicación entre el microcontrolador y otros dispositivos, de igual forma se modularizaron todos los codigos y de esta manera los podemos volver a utilizar en algun proyecto y el main.c queda mucho más corto.

* Se pudo comprobar la eficiencia para transmitir la información, a traves de diferentes elementos y de igual manera se comprendió como funciona el I2C tanto del pic como el físico para la transmición de información.

* Se pudo verificar el funcionamiento del I2C, al mostrar caracteres especiales (como la animación), strings y texto movible. Esto convierte este protocolo de transmición de información en algo muy eficiente al no gastar tantos pines de conexión.

* El módulo MSSP en modo maestro permitió establecer una comunicación I²C estable, demostrando la importancia de sincronizar correctamente las señales SDA y SCL para garantizar la transmisión de información.
