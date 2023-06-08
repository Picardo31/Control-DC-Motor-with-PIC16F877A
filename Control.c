
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#pragma config FOSC = XT        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#define _XTAL_FREQ 8000000     //Frecuencia del oscilador


#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

long aux = 0;    //Variable para cambio de giro
long cur = 1;    //Variable de posición del cursor en menu
long pren = 0;  //Variable que verifica si ya inicio el teclado
long fle = 0;   //Variable del cambio de cursor
long flee = 0;  //Varible de comprobación de cambio de cursor
long fleq = 0;  //Variable estacionaria de cursor actual
long fi = 0;    //Variable de cursor
long lu = 0;    //Variable de ingreso a administrador
int lux[10];    //Variable de password
int pos __at(0x22);     //Vector de posición de passwrod
long cl = 0;    //Variable de aplicación de velocidad maxima
int ve[10];     //Vector de velocidad de usuario
int vepos __at(0x23); //Vector de posición de velocidad de usuario
long fve = 0;       //Variable de activación de velocidad usuario
long flus = 0;      //Variable de posición del cursor en menu
long flusd = 1;     //Variable de selección de velocidad o dirección de giro
long mo = 0;        //Variable de prendido del motor
int or = 0;         //Variable de velocidad ingresada
int vpr = 0;        //Variable de posición uno en vector velocidad
int vse = 0;        //Variable de posición dos en vector velocidad
int vte = 0;        //Variable de posición tres en vector velocidad
int vmpr = 0;       //Variable de posición uno en vector velocidad maxima
int vmse = 0;       //Variable de posición dos en vector velocidad maxima
int vmte = 0;       //Variable de posición tres en vector velocidad maxima
int max = 0;        //Variable de velocidad maxima
int f = 0;          //Variable de frecuencia usada
float rel = 0;      //Variable de relación de velocidad
int pres = 0;       //Variable de prescalador usado
int dcc = 0;        //Variable de valor dutycycle
float fz = 0;       //Variable uno de comprobación de paso
int fs = 0;         //Variable dos de comprobación de paso
float fzs = 0;      //Variable tres de comprobación de paso
int adcur = 1;      //Variable de cursor en administrador
int vm[10];         //Variable de vector de velocidad maxima
int posvm __at(0x24);   //Vector de posición de velocidad maxima
int pas[10];        //Variable de vector de paso
int pospas __at(0x25);  //Vector de posición del paso
int pas1[10];       //Variable de vector de posición de paso
int obv1[10];       //Variable de vector de posición de velovidad maxima en administrador
int obpas = 0;      //Variable de cambio de paso
int obve = 0;       //Variable de cambio de posición
int paso = 1;       //Variable de paso inicial
int pasose = 0;     //Variable uno de paso
int pasopr = 0;     //Variable dos de paso
char s[10];         //Variable de impresion para resultados
void __interrupt() motor(void)
{
    if (INTCONbits.RBIF == 1)
      {
        //Comprobación de incio
         PORTBbits.RB0 = 0;
         PORTBbits.RB1 = 1;
         PORTBbits.RB2 = 1;
         PORTBbits.RB3 = 1;
         if (PORTBbits.RB7 == 0)
         {
             pren = 1;
             return;
         }
         //Barrido de teclado
         if (pren == 1)
         {
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 1;
            PORTBbits.RB2 = 1;
            PORTBbits.RB3 = 1;
             if (PORTBbits.RB4 == 0)
            {
                 if(lu == 1)        //verifica ingreso a administrador
                 {
                     lux[pos] = 7;
                     pos++;
                 }
                 if(fve == 1)       //activa velocidad de usuario
                 {
                     ve[vepos] = 7;
                     vepos++;
                 }
                 //Comprobación de uso de velocidad maxima
                 if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                 {
                     vm[posvm] = 7;
                     posvm ++;
                 }
                 //comprobación de uso de velocidad maxima
                 if ((cl == 1) && (adcur == 2) && (pospas != 0))
                 {
                     pas[pospas] = 7;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB5 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 4;
                     pos++;
                 }
                if(fve== 1)
                 {
                     ve[vepos] = 4;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 4;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 4;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0;
               return;
            }
            if (PORTBbits.RB6 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 1;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 1;
                     vepos++;
                 }
                if (flusd == 2)
                {
                    aux = 1;
                }
                if ((cl == 1) && (adcur == 1))
                {
                    vm[posvm] = 1;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 1;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB7 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 0;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 0;
                     vepos++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            //__delay_ms(500);
            PORTBbits.RB0 = 1;
            PORTBbits.RB1 = 0;
            PORTBbits.RB2 = 1;
            PORTBbits.RB3 = 1;
            if (PORTBbits.RB4 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 8;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 8;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 8;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2) && (pospas != 0))
                 {
                     pas[pospas] = 8;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB5 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 5;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 5;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 5;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 5;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB6 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 2;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 2;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 2;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 2;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB7 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 0;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 0;
                     vepos++;
                 }
                if (flusd == 2)
                {
                    aux = 0;
                }
                if ((cl == 1) && (adcur == 1))
                {
                    vm[posvm] = 0;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 0;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            //__delay_ms(500);
            PORTBbits.RB0 = 1;
            PORTBbits.RB1 = 1;
            PORTBbits.RB2 = 0;
            PORTBbits.RB3 = 1;
            if (PORTBbits.RB4 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 9;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 9;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 9;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2) && (pospas != 0))
                 {
                     pas[pospas] = 9;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB5 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 6;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 6;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 6;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2) && (pospas != 0))
                 {
                     pas[pospas] = 6;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            if (PORTBbits.RB6 == 0)
            {
                if(lu == 1)
                 {
                     lux[pos] = 3;
                     pos++;
                 }
                if(fve == 1)
                 {
                     ve[vepos] = 3;
                     vepos++;
                 }
                if ((cl == 1) && (posvm != 0) && (vm[0] != 1) && (adcur == 1))
                {
                    vm[posvm] = 3;
                    posvm ++;
                }
                if ((cl == 1) && (adcur == 2))
                 {
                     pas[pospas] = 3;
                     pospas ++;
                 }
               INTCONbits.RBIF = 0; 
               return;
            }
            //Boton de ingreso a los diferentes menus
            if (PORTBbits.RB7 == 0)
            {
                fle = 1;
                fleq = 1;
               INTCONbits.RBIF = 0; 
               return;
            }
            //__delay_ms(500);
            PORTBbits.RB0 = 1;
            PORTBbits.RB1 = 1;
            PORTBbits.RB2 = 1;
            PORTBbits.RB3 = 0;
            //Boton de reinicio al menu principal
            if (PORTBbits.RB4 == 0)
            {
                fleq = 0;
                fi = 0;
                lux[0] = 0;
                lux[1] = 0;
                lux[2] = 0;
                pos = 0;
                cl = 0;
               INTCONbits.RBIF = 0; 
               return;
            }
            //Boton de prendido del motor
            if (PORTBbits.RB5 == 0)
            {
                mo ++;
                if (mo == 2)
                {
                    mo = 0;
                }
               INTCONbits.RBIF = 0; 
               return;
            }
            //Boton de posición del cursor arriba
            if (PORTBbits.RB6 == 0)
            {
                if (fleq == 0)
                {
                    cur = 1;
                }
                if ((flus = 1) && (cl == 0))
                {
                     flusd = 1;
                }
                if (cl == 1)
                {
                    adcur = 1;
                }
               INTCONbits.RBIF = 0; 
               return;
            }
            //Boton de posición del cursor abajo
            if (PORTBbits.RB7 == 0)
            {
                if (fleq == 0)
                {
                    cur = 2;
                }
                if ((flus = 1) && (cl == 0))
                {
                     flusd = 2;
                }
                if (cl == 1)
                {
                    adcur = 2;
                }
               INTCONbits.RBIF = 0; 
               return;
            }
            //__delay_ms(500);
         }
         
      }
    
}

//Función de comprovación de validez de velocidad según paso
void comp()
{
    pasopr = 10*pas[0];
    pasose = 1*pas[1];
    paso = pasopr + pasose; //Paso como int
    fz = (float)or/paso;    //paso como flotante
    fs = (or/paso);         //Paso como int dividido
    fzs = (float)(fs - fz); //paso si es entero o fracción
    if (fzs == 0)
    {
        or = or;
    }
    else
    {
        or = 10;
    }
    return;
}

//Función de obtención de relación de trabajo
void porcentaje()
{
    vpr = 100*ve[0];
    vse = 10*ve[1];
    vte = 1*ve[2];
    or = vpr + vse + vte;
    return;
}

//Función de valor de dutycycle por medio de ecuación de relación
void senal ()
{
    porcentaje();
    comp();
    rel = (float)or/100;
    f = 3000;
    pres = 4;
    // PWM
    dcc =(1/((float)f)*rel*_XTAL_FREQ)/pres;
    //MSB del registro 1
    CCPR1L = dcc>>2; 
    //Bit 0 y bit 1 de LSB del registro 1 
    CCP1X = dcc&0x01;
    CCP1Y = dcc&0x02;
    return;
}

//Función de configuración de Señal PWM
void pwm ()
{
    //Salida
    TRISC2=0;
    //Preescalador 1:4
    T2CONbits.T2CKPS0=1;
    T2CONbits.T2CKPS1=0;
    //Encender el Timer
    T2CONbits.TMR2ON=1;
    //Configurar el módulo CCP en modo PWM
    CCP1CONbits.CCP1M3=1;
    CCP1CONbits.CCP1M2=1;
    return;
}

//Función de obtención de velocidad maxima
void vmax()
{
    vmpr = 100*vm[0];
    vmse = 10*vm[1];
    vmte = 1*vm[2];
    max = vmpr + vmse + vmte;
    return;
}
//Función de menu de usuario
void usuario()
{
    if (flus == 1)
    {
        if (flusd == 1)
        {
            //configuración de velocidad de usuario
            fve = 1;
            Lcd_Set_Cursor(flusd,1);
            Lcd_Write_String(">");
            Lcd_Set_Cursor(1,2);
            Lcd_Write_String("Ve:");
            Lcd_Set_Cursor(2,2);
            Lcd_Write_String("D:");
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("             ");
            Lcd_Set_Cursor(1,5);
            sprintf(s,"%d",ve[0]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,6);
            sprintf(s,"%d",ve[1]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,7);
            sprintf(s,"%d",ve[2]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,8);
            Lcd_Write_String("%");
            vmax();
            porcentaje();
            //Comprueba que el valor de velocidad ingresado este dentro del valor maximo
            if (max < or)
            {
                ve[0] = 0;
                ve[1] = 0;
                ve[2] = 0;
                vepos = 0;
            }
            //Nuevo ingreso de velocidad
            if (vepos == 3)
            {
                vepos = 0;
            }
        }
        else 
        {
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("*");
        }
        if (flusd == 2)
        {
            //Configuración de sentido de giro
            fve = 0;
            Lcd_Set_Cursor(flusd,1);
            Lcd_Write_String(">");
            Lcd_Set_Cursor(2,2);
            Lcd_Write_String("D:");
            Lcd_Set_Cursor(2,4);
            sprintf(s,"%d",aux);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(2,5);
            Lcd_Write_String("             ");
        }
        else 
        {
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("*");
        }
    }
    return;
}

//Función de menu de adminstrador
void admincon()
{
    //Configuración de posición de cursor
    if (adcur == 1)
    {
        Lcd_Set_Cursor(adcur,1);
        Lcd_Write_String(">");
    }
    else 
    {
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("*");
    }
    if (adcur == 2)
    {
        Lcd_Set_Cursor(adcur,1);
        Lcd_Write_String(">");
    }
    else 
    {
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("*");
    }
    if (adcur == 1)
    {
        //Configuración de velocidad maxima dentro de administrador
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("V:");
        Lcd_Set_Cursor(2,2);
        Lcd_Write_String("P:");
        Lcd_Set_Cursor(1,4);
        sprintf(s,"%d",vm[0]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(1,5);
        sprintf(s,"%d",vm[1]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(1,6);
        sprintf(s,"%d ",vm[2]);
        Lcd_Write_String(s);
        
        //Establece valores previos ingresados
        if (obve == 1)
        {
            obv1[3] = vm[0];
            obv1[4] = vm[1];
            obv1[5] = vm[2];
            Lcd_Set_Cursor(1,12);
            sprintf(s,"%d",obv1[3]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,13);
            sprintf(s,"%d",obv1[4]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,14);
            sprintf(s,"%d ",obv1[5]);
            Lcd_Write_String(s);
        }
        
        if (obve == 0)
        {
            obv1[0] = vm[0];
            obv1[1] = vm[1];
            obv1[2] = vm[2];
            Lcd_Set_Cursor(1,8);
            sprintf(s,"%d",obv1[0]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,9);
            sprintf(s,"%d",obv1[1]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(1,10);
            sprintf(s,"%d ",obv1[2]);
            Lcd_Write_String(s);
        }
        
    }
    //Reestablece la posición de los valores ingresados
    if (posvm == 3)
    {
        posvm = 0;
        obve ++;
        if (obve == 3)
        {
            obve = 0;
        }
    }
    if (adcur == 2)
    {
        //Configuración de paso
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("V:");
        Lcd_Set_Cursor(2,2);
        Lcd_Write_String("P:");
        Lcd_Set_Cursor(2,4);
        sprintf(s,"%d",pas[0]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2,5);
        sprintf(s,"%d   ",pas[1]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2,5);
        
        //Establece valores previos ingresados
        if (obpas == 2)
        {
            pas1[4] = pas[0];
            pas1[5] = pas[1];
            Lcd_Set_Cursor(2,15);
            sprintf(s,"%d",pas1[4]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(2,16);
            sprintf(s,"%d",pas1[5]);
            Lcd_Write_String(s);
        }
        
        if (obpas == 1)
        {
            pas1[2] = pas[0];
            pas1[3] = pas[1];
            Lcd_Set_Cursor(2,12);
            sprintf(s,"%d",pas1[2]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(2,13);
            sprintf(s,"%d ",pas1[3]);
            Lcd_Write_String(s);
        }
        
        if (obpas == 0)
        {
            pas1[0] = pas[0];
            pas1[1] = pas[1];
            Lcd_Set_Cursor(2,9);
            sprintf(s,"%d",pas1[0]);
            Lcd_Write_String(s);
            Lcd_Set_Cursor(2,10);
            sprintf(s,"%d ",pas1[1]);
            Lcd_Write_String(s);
        }
        
    }
    //Reestablece la posición de valores ingresados
    if (pospas == 2)
    {
        pospas = 0;
        obpas ++;
        if (obpas == 3)
        {
            obpas = 0;
        }
    }
}

//Función de configuración de ingreso según password
void admin()
{
    if (cl == 0)
    {
        Lcd_Set_Cursor(1,5);
        Lcd_Write_String("             ");
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("Adm");
    }
    
           
    if ((lu == 1) && (lux[0] == 0))
    {
        Lcd_Set_Cursor(2,7);
        Lcd_Write_String("             ");
    }
    //Valores de password
    if (cl == 0)
    {
        Lcd_Set_Cursor(2,2);
        Lcd_Write_String("Pass:");
        Lcd_Set_Cursor(2,7);
        sprintf(s,"%d",lux[0]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2,8);
        sprintf(s,"%d",lux[1]);
        Lcd_Write_String(s);
        Lcd_Set_Cursor(2,9);
        sprintf(s,"%d",lux[2]);
        Lcd_Write_String(s);
    }
    // Verifica si la clave es correcta o no
    if(lux[0]== 4 && lux[1]== 5 && lux[2]== 6)
    {
        cl = 1;
        lu = 0;
        admincon();
    }
    else
    {
        Lcd_Set_Cursor(2,13);
        Lcd_Write_String("INC");
    }
    if (pos == 3)
    {
        pos = 0;
    }
            return;
}

//Función de prendido del motor por puerto E0
void motor_ini()
{
    if (mo == 0)
    {
        PORTEbits.RE0 = 0;
    }
    else if (mo == 1) //Si se encuentra prendido permite configurar el sentido de giro por puerto A1
    {
        PORTEbits.RE0 = 1;
        if (aux == 0)
        {
            PORTAbits.RA1 = 0;
        }
        else if (aux == 1)
        {
            PORTAbits.RA1 = 1;
        }
    }
    return;
}

//Función de posición del cursor en el menú principal
void cursos_ini()
{
    if (fleq == 0)
    {
        if (cur == 1)
        {
            Lcd_Set_Cursor(cur,1);
            Lcd_Write_String(">");
        }
        else 
        {
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("*");
        }
        if (cur == 2)
        {
            Lcd_Set_Cursor(cur,1);
            Lcd_Write_String(">");
        }
        else 
        {
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("*");
        }
    }
    return;
}

//Función de cambio de cursor
void variable_curs()
{
    //Variable de cambio de cursor
    if (fle == 1)
    {
        flee = 1;
        fi = flee*cur;
        fle = 0;
        if (fi == 1)
        {
            fve = 1;
            flus = 1;
        }
        if (fi == 2)
        {
            lu = 1;
            fve = 0;
            flus = 0;
        }
    }
    fle = 0;
    return;
}

//Función principal del programa
void main(void) 
{
    //Declaro el uso del lcd y las salida
    TRISD = 0;
    TRISA = 0;
    TRISE = 0;
    PR2=165;  //165.66 valor conseguido según parámetros iniciales
    
    senal();
    pwm();
    
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,4);
    Lcd_Write_String("Encender");
    //Barrido de teclado
    TRISB = 0B11110000;
    OPTION_REGbits.nRBPU = 0;  //activar los pull ups

    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    Lcd_Clear();
    while (1)
    {
        senal();
        motor_ini();
        cursos_ini();
        variable_curs(); 
        //Se configura menu principal
        if (fi == 0)
        {
            Lcd_Set_Cursor(1,2);
            Lcd_Write_String("Usuario     ");
            Lcd_Set_Cursor(2,2);
            Lcd_Write_String("Administrador   ");
        }
        if (fi == 1)
        { 
            //Función Usuario
            usuario();
        }
        if (fi == 2)
        {
            //Función Administrador
            admin();
        }
    }
    return;
}

