#include "TM4C123GH6PM.h"
#include <stdint.h>
#include <stdlib.h>

void Delay(unsigned long counter); // Delay Function
void HC05_init(void); // UART module 5 is used to initialize HC05 bluetooth module
char Bluetooth_Read(void); //Data is read from PE4
void Bluetooth_Write(unsigned char data); // Data transmitted to PE5
void Bluetooth_Write_String(char *str); // String Data transmitted to PE5

int main(void){
      HC05_init();
      SYSCTL->RCGCGPIO = 0x20; //Enabling clock to Port F
      GPIOF->DIR = 0x0E; //PF1, PF2, PF3 are used as digital pins
      GPIOF->DEN = 0x0E; // Pins are digitally enabled
      Delay(10);
      while(1)
      {
          char c = Bluetooth_Read(); //Char input from bluetooth function
          if( c == 'A'){
            GPIOF->DATA = 0x02; //PF1 is connected to Red LED Bluetooth_Write_String("RED LED ON\n");
          }
          else if( c == 'B'){
            GPIOF->DATA = 0x04; //PF2 is connected to Blue LED Bluetooth_Write_String("BLUE LED ON\n");
          }
          else if( c == 'C'){
            GPIOF->DATA = 0x08; //PF3 is connected to Green LED Bluetooth_Write_String("GREEN LED ON\n");
          }
          else {
            GPIOF->DATA = 0x00; //Turns LEDs off as no input to any pin of Port F Bluetooth_Write_String("LEDs OFF\n");
          }
      }
}
void HC05_init(void)
{
      SYSCTL->RCGCUART |= 0x20; // enable clock to UART5 /Uart 5 is attached to PE4 and PE5, Pg No. 895 from datasheet/
      SYSCTL->RCGCGPIO |= 0x10; // enable clock to PORT E
      Delay(1);
      /* UART5 initialization */
      UART5->CTL = 0; /* UART5 module disbable 
                        Steps from Bluetooth Controlled RGB LED*/
      UART5->IBRD = 104; /* for 9600 baud rate, integer = 104 */
      UART5->FBRD = 11; /* for 9600 baud rate, fractional = 11*/
      UART5->CC = 0; /*select system clock*/
      UART5->LCRH = 0x60;
      UART5->CTL = 0x301; /* Enable UART5 module, Rx and Tx */
      /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
      GPIOE->DEN = 0x30; /* set PE4 and PE5 as digital */
      GPIOE->AFSEL = 0x30; /* Alternate function for PE4 and PE5 */
      GPIOE->AMSEL = 0; // Analog Functions are turned off
      GPIOE->PCTL = 0x00110000; /* configure PE4 and PE5 for UART */
}
char Bluetooth_Read(void)
{
char data;
while((UART5->FR & 0x10) != 0); // Waits for the input char
data = UART5->DR ;
return (unsigned char) data;
}
void Bluetooth_Write(unsigned char data)
{
while((UART5->FR & 0x20) != 0); //Waits for the output to be printed
UART5->DR = data;
}
void Bluetooth_Write_String(char *str){
while(*str)
{
Bluetooth_Write(*(str++)); // This function is using the above function to print a string char by char
}
}
void Delay(unsigned long counter)
{
unsigned long i = 0;
while(i < counter) i++;
}