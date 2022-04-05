//---------------------------------------------------------
/*
NHD_12232KZ_mega.ino
Program for writing to Newhaven Display 122x32(KZ) Graphic LCD with SBN1661G controller

(c)2013 Mike LaVine - Newhaven Display International, LLC.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.
*/
//---------------------------------------------------------

// The 8 bit data bus is connected to PORTA of the Arduino Mega2560

int RS = 30;  // A0 signal connected to Arduino digital pin 30
int RW = 31;  // R/W signal connected to Arduino digital pin 31
int E = 32;   // E signal connected to Arduino digital pin 32
int RES = 33; // /RES signal connected to Arduino digital pin 33
int CS1 = 34; // /CS1 signal connected to Arduino digital pin 34
int CS2 = 35; // /CS2 signal conencted to Arduino digital pin 35

int i, m, n;

unsigned char page;

void comleft(unsigned char i)
{
  digitalWrite(CS2, LOW);
  digitalWrite(RW, LOW);
  digitalWrite(RS, LOW);
  digitalWrite(E, HIGH);
  PORTA = i;
  delay(2);
  digitalWrite(E, LOW);
  digitalWrite(CS2, HIGH);
}

void comright(unsigned char i)
{
  digitalWrite(CS1, LOW);
  digitalWrite(RW, LOW);
  digitalWrite(RS, LOW);
  digitalWrite(E, HIGH);
  PORTA = i;
  delay(2);
  digitalWrite(E, LOW);
  digitalWrite(CS1, HIGH);
}

void writeleft(unsigned char i)
{
  digitalWrite(CS2, LOW);
  digitalWrite(RW, LOW);
  digitalWrite(RS, HIGH);
  digitalWrite(E, HIGH);
  PORTA = i;
  delay(2);
  digitalWrite(E, LOW);
  digitalWrite(CS2, HIGH);
}

void writeright(unsigned char i)
{
  digitalWrite(CS1, LOW);
  digitalWrite(RW, LOW);
  digitalWrite(RS, HIGH);
  digitalWrite(E, HIGH);
  PORTA = i;
  delay(2);
  digitalWrite(E, LOW);
  digitalWrite(CS1, HIGH);
}

/*****************************************/
void bothsides(unsigned char i)
{
  comleft(i);
  comright(i);
}

/*****************************************/
void nhd_12232kz_init()
{
  digitalWrite(RES, LOW); // Reset RST
  delay(10);
  digitalWrite(RES, HIGH); // Reset RST= M68 Interface
  delay(150);
  digitalWrite(RS, LOW);
  digitalWrite(RW, HIGH);
  bothsides(0xE2);
  delay(100);
  bothsides(0xA4);
  bothsides(0xA9);
  bothsides(0xA0);
  bothsides(0xEE);
  bothsides(0xC0);
  bothsides(0xAF);
  delay(10);
}

/*****************************************/
void setup()
{
  DDRC = 0xFF; // set PORTC as output (used for control signals)
  DDRA = 0xFF; // set PORTA as output (used for data bus)
  PORTA = 0x00;
  delay(1);
  digitalWrite(E, LOW);
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, HIGH);
  nhd_12232kz_init();
}

void loop()
{
  unsigned char d = 0xFF;

  for (i = 0; i < 2; i++)
  {
    page = 0xB8;
    for (m = 0; m < 4; m++)
    {
      bothsides(page);
      bothsides(0x00);
      for (n = 0; n < 61; n++)
      {
        writeleft(d);
      }
      for (n = 0; n < 61; n++)
      {
        writeright(~d);
      }
      page++;
      d = ~d;
    }
    d = ~d;
    delay(1000);
  }
}
