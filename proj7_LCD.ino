#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define RS PD0
#define E PD1

void E_pulse(); 

void LCD_command(unsigned char com);

void LCD_sign(unsigned char sign);

void LCD_init();

void LCD_set_cursor(unsigned char row, unsigned char col);

void LCD_clear();

void LCD_print(const char *array);

int main()
{
  LCD_init();

  LCD_set_cursor(0, 0);
  LCD_print("0,2,3,4,5");
  _delay_ms(3000);
 
  while(1)
  {}
  return 0;
}


void E_pulse()
{
  PORTD |= (1<<E);
  _delay_us(1);
  PORTD &= ~(1<<E);
  _delay_ms(2);
}

void LCD_command(unsigned char com)
{
  PORTD &= ~(1<<RS);
  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  PORTD = ((com & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  PORTD = (((com<<4) & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //_delay_ms(2);
}

void LCD_sign(unsigned char sign)
{
  PORTD |= (1<<RS);
  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  PORTD = ((sign & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  PORTD = (((sign<<4) & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  _delay_us(2);
}

void LCD_init()
{
  //ustawienie jako wyjście pinów D7,6,5,4,1,0 (danych równoległych oraz RS i E)
  DDRD = 0b11111111;

  _delay_ms(20);
 
  //Display/cursor home - Ustawia adres DDRAM 0 w liczniku adresów. Przywraca również przesunięcie wyświetlacza do pierwotnej pozycji. Zawartość pamięci DDRAM pozostaje niezmieniona
  LCD_command(0b00000010);
  _delay_us(2000);
 
  //Function set - przesył, ilosc linijek, czcionka
  LCD_command(0b00101000);
  _delay_us(50);

  //Display ON/OFF - - Włącza/wyłącza cały wyświetlacz, włącza/wyłącza kursor i znak pozycji kursora.
  LCD_command(0b00001100);
  _delay_us(50);

  //Entry mode set - Ustawia kierunek ruchu kursora i określa przesunięcie wyświetlania. Operacje te są wykonywane podczas zapisu i odczytu danych.
  LCD_command(0b00000110);
  _delay_us(50);

  //Display cursor shift - Przesuwa kursor i przesuwa ekran bez zmiany zawartości pamięci DDRAM.
  LCD_command(0b00010100);
  _delay_us(50);

  //Display clear - czyści wyświetlacz
  LCD_command(0b00000001);
}

void LCD_clear()
{
   LCD_command(0b00000001);
}

void LCD_set_cursor(unsigned char row, unsigned char col)
{
    unsigned char pos;
    
    if (row == 0) {
        pos = 0x80 + col; // Pierwsza linia
    } else if (row == 1) {
        pos = 0xC0 + col; // Druga linia
    } else {
        return; // Nieprawidłowy wiersz
    }
    
    LCD_command(pos);
}

void LCD_print(const char *array)
{
  for(int i=0; i<strlen(array); i++)
  {
      LCD_sign(array[i]); 
  }
}
