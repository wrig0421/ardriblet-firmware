/**************************************************************************
 *  -|-|-|-|-|-|-|-|Adriblet Cocktail Dispenser Code-|-|-|-|-|-|-|-|-|-|- *                                    *
 *                                                                        *
 *  @brief      Ardiblet (cocktail dispenser) code                        *
 *  @note       Flashed to Arduino Mega 2560 - Port Manipulation sections *
 *              are specific to this micro                                *
 *  @author     Spencer Wright                                            *
 *  @copyright  Copyright (C) 2016-2018 Spencer Wright                    *
 *                                                                        *
 *  The code is intended to be distributed open-source with NO            *
 *  ASSOCIATED WARRANTY distributed with the code.  Use at your own       *
 *  risk.  Forward any bugs or concerns to wrig0421@umn.edu.              *
 **************************************************************************/

/* 128x64 LCD Display Class */
// For more info on the LCD library visit: https://github.com/olikraus/u8g2/wiki/u8g2reference 
#include <U8g2lib.h>
#include <U8x8lib.h>

/* Libraries used to communicate with the LCD display */ 
#include <SPI.h>
#include <Wire.h>

/* Standard Arduino IDE library */
#include <Arduino.h>

/* Button Pin Assignment controlled by Macro below */
//#define ARDRIBLET
#define EXAMPLE_DISPLAY

#define PRINT_INTRO

#define US_ROUNDTRIP_CM   57   // microseconds (uS) it takes sound to travel "round-trip" 1cm (2cm total)
#define VALVE_OPEN_TIME   8000

/* LCD object created below */ 
// Digital pins 13, 11, 10, 8 are routed the same on the Adriblet & Example display
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 13, 11, 10, 8);

/* 
 * bmps for the introduction artwork are defined below. 
 * The bmps were created with the intention to be displayed on a 128x64 pixel LCD.
 * The display used for this specific project was as follows:
 *    LCD Type:  HJ12864ZW
 *    LCD Controller (in display): ST7920
 *    LCD Supply Voltage:  5 V
 * See link below for more detail on bmps.
 *  http://paulbourke.net/dataformats/bmps/
 */
// ground-zero bmp
static unsigned char ground_zero_bmp[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x01, 0xc0, 0x7f, 0x00, 0x00,
  0x00, 0xf8, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xc0, 0x07,
  0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x3e, 0x00, 0x00, 0xf8,
  0x00, 0x00, 0xc0, 0x07, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00,
  0x3e, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xc0, 0x0f, 0xe0, 0x7f, 0x00, 0x00,
  0x00, 0xf8, 0x01, 0x00, 0x3e, 0x3e, 0x00, 0xf8, 0x00, 0x00, 0xc0, 0x0f,
  0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x3e, 0x3e, 0x00, 0xf8,
  0x00, 0x00, 0xc0, 0x07, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00,
  0x3e, 0x7e, 0x00, 0xf8, 0x00, 0x00, 0xe0, 0x07, 0xe0, 0x7f, 0x00, 0x00,
  0x00, 0xf8, 0x01, 0x00, 0x3e, 0x3e, 0x00, 0xfc, 0x00, 0x00, 0xc0, 0x07,
  0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x16, 0x3e, 0x00, 0xf8,
  0x00, 0x00, 0xc0, 0x07, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00,
  0x00, 0x3e, 0x00, 0xfc, 0x00, 0x00, 0xc0, 0x07, 0xe0, 0x7f, 0x00, 0x00,
  0x00, 0xf8, 0x01, 0x00, 0x00, 0x3e, 0x00, 0xfc, 0x00, 0x00, 0xc0, 0x07,
  0xe0, 0x7d, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x3e, 0x00, 0xfc,
  0x00, 0x00, 0xc0, 0x07, 0xe0, 0x7d, 0x00, 0xc0, 0x01, 0xf8, 0x01, 0x30,
  0x00, 0x3e, 0x00, 0xfc, 0x80, 0x1f, 0xc0, 0x07, 0xe0, 0xf9, 0xc0, 0xe7,
  0x01, 0xf8, 0xf1, 0x39, 0x00, 0x1e, 0x00, 0xfc, 0xe0, 0x3f, 0xe0, 0x07,
  0xe0, 0xf9, 0xc0, 0xe7, 0xc1, 0xf9, 0xf9, 0x3d, 0x00, 0x1e, 0x00, 0xfc,
  0xe0, 0x7f, 0xe0, 0x27, 0xe0, 0xf9, 0xc0, 0xf7, 0xe1, 0xff, 0xf9, 0x3d,
  0x3e, 0x1e, 0x1c, 0xf8, 0xf0, 0x7f, 0xf0, 0x7f, 0xe0, 0xf9, 0xe0, 0xf7,
  0xe1, 0xff, 0xf9, 0x3d, 0x3e, 0x0e, 0x3f, 0xfc, 0xf0, 0x7f, 0xf8, 0x7f,
  0xe0, 0xf9, 0xc0, 0xff, 0xf1, 0xff, 0xf9, 0x3f, 0x3e, 0x8e, 0x3f, 0xfc,
  0xf0, 0xff, 0xf8, 0x7f, 0xf0, 0xf9, 0xc0, 0xff, 0xf1, 0xff, 0xf9, 0x3f,
  0x3e, 0x86, 0x7f, 0xf8, 0xf8, 0xf9, 0xf0, 0x7f, 0xf0, 0xf9, 0xc0, 0xff,
  0xf1, 0xfb, 0xf9, 0x3f, 0x3e, 0xc6, 0x7f, 0xf8, 0xf8, 0xfd, 0xf0, 0x7f,
  0xe0, 0xf9, 0xc1, 0xff, 0xf9, 0xf9, 0xf9, 0x3f, 0x3e, 0xc6, 0x7f, 0xf8,
  0xf8, 0xfd, 0xf0, 0x7f, 0xe0, 0xf9, 0xc1, 0x1f, 0xf8, 0xf9, 0xf9, 0x03,
  0x3e, 0xc6, 0xfe, 0xf8, 0xf8, 0xf9, 0xd0, 0x27, 0xf0, 0xf9, 0xc1, 0x0f,
  0xf8, 0xf9, 0xf9, 0x03, 0x3e, 0xc6, 0x7e, 0xf8, 0xf8, 0xf8, 0xc0, 0x07,
  0xf0, 0xf9, 0xc1, 0x0f, 0xf8, 0xf9, 0xf1, 0x01, 0x3e, 0x86, 0xfc, 0xf8,
  0xf8, 0xf9, 0xc0, 0x07, 0xe0, 0xf9, 0xc1, 0x07, 0xf8, 0xf9, 0xf9, 0x01,
  0x3e, 0x5e, 0xfc, 0xf8, 0xf8, 0xf8, 0xc1, 0x07, 0xf0, 0xf9, 0xc1, 0x07,
  0xf8, 0xf9, 0xf1, 0x01, 0x3e, 0x9e, 0xfc, 0xf8, 0xfc, 0xfd, 0xc0, 0x07,
  0xf0, 0xf9, 0xc1, 0x07, 0xf8, 0xf9, 0xf1, 0x01, 0x3e, 0x1e, 0xfc, 0xf8,
  0xfc, 0xfd, 0xc0, 0x07, 0xf0, 0xf1, 0xc1, 0x07, 0xf8, 0xf9, 0xf1, 0x01,
  0x3e, 0x1e, 0xfc, 0xf8, 0xfc, 0xff, 0xc0, 0x07, 0xf0, 0xf9, 0xc1, 0x07,
  0xf8, 0xf9, 0xf1, 0x01, 0x3e, 0x3e, 0xfc, 0xf8, 0xfc, 0xf7, 0xc0, 0x03,
  0xf0, 0xf1, 0xc1, 0x07, 0xf8, 0xf9, 0xf1, 0x00, 0x3e, 0x3e, 0xf8, 0xf8,
  0xfc, 0xf7, 0xc0, 0x03, 0xf0, 0x31, 0xc0, 0x07, 0xf8, 0xf9, 0xf1, 0x00,
  0x3e, 0x1e, 0xfc, 0xf8, 0xfc, 0xe7, 0xc0, 0x03, 0xf0, 0x01, 0xc0, 0x07,
  0xf8, 0xf9, 0xf1, 0x00, 0x3e, 0x0e, 0xf8, 0xf8, 0xfc, 0x6f, 0xc0, 0x07,
  0xf0, 0x0f, 0xc0, 0x07, 0xf8, 0xf9, 0xf1, 0x00, 0x3e, 0x0e, 0xf8, 0xf8,
  0xfc, 0x01, 0xc0, 0x07, 0xf0, 0x0f, 0xc1, 0x07, 0xf8, 0x99, 0xf1, 0x00,
  0x3e, 0x1e, 0xf8, 0xf8, 0xfc, 0x00, 0xe0, 0x07, 0xf0, 0xcf, 0x83, 0x07,
  0xf8, 0x19, 0xf0, 0x01, 0x3e, 0x1e, 0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x07,
  0xf8, 0xff, 0xc3, 0x07, 0xf8, 0x79, 0xf0, 0x01, 0x3e, 0x1e, 0xf8, 0xf8,
  0xf8, 0x00, 0xe0, 0x07, 0xf8, 0xff, 0xc3, 0x07, 0xf8, 0x79, 0xf8, 0x01,
  0x3e, 0x1e, 0xf8, 0xf8, 0xf8, 0x00, 0xe0, 0x07, 0xf8, 0xff, 0xc3, 0x07,
  0xf8, 0x79, 0xf0, 0x01, 0x3e, 0x0e, 0xf8, 0xf8, 0xf8, 0x71, 0xe0, 0x03,
  0xf8, 0xff, 0xc3, 0x07, 0xf8, 0x79, 0xf0, 0x01, 0x3f, 0x0e, 0xf8, 0xf8,
  0xf8, 0xf0, 0xe0, 0x03, 0xf8, 0xff, 0xc3, 0x07, 0xf8, 0xf9, 0xf0, 0x00,
  0x3f, 0x0e, 0xf8, 0xf8, 0xf0, 0xf9, 0xe0, 0x03, 0xf8, 0xf1, 0xc3, 0x03,
  0xf8, 0xf9, 0x70, 0x00, 0x3f, 0x0e, 0xf8, 0xf8, 0xf0, 0xf9, 0xe0, 0x07,
  0xf8, 0xf0, 0xc3, 0x01, 0xf8, 0xfd, 0x30, 0x00, 0x3f, 0x1e, 0xfc, 0xf8,
  0xf0, 0x79, 0xe0, 0x07, 0xf8, 0xf0, 0x03, 0x00, 0xf8, 0xfd, 0x00, 0x00,
  0x3f, 0x3e, 0x7c, 0xf8, 0xf0, 0x7f, 0xc0, 0x07, 0xf8, 0xf0, 0x07, 0x00,
  0xf8, 0x7f, 0x00, 0x00, 0x3f, 0x3e, 0x7c, 0xf8, 0xe0, 0x7f, 0xe0, 0x07,
  0xf8, 0xf0, 0x07, 0x02, 0xf8, 0xff, 0xc0, 0x00, 0x3f, 0xde, 0x7e, 0xf8,
  0xc0, 0x7f, 0xe0, 0x07, 0xf8, 0xe0, 0x07, 0x07, 0xf0, 0x7f, 0xe0, 0x00,
  0x3f, 0xde, 0x7f, 0xf8, 0xc0, 0x3f, 0xe0, 0x3f, 0xf8, 0xe0, 0x87, 0x07,
  0xf0, 0x7f, 0xf0, 0x00, 0x3f, 0xde, 0x7f, 0xf8, 0x80, 0x3f, 0xe0, 0x3f,
  0xf8, 0xe0, 0xc7, 0x07, 0xf0, 0x7f, 0xf0, 0x00, 0x3f, 0xde, 0x7f, 0xf8,
  0x00, 0x1f, 0xe0, 0x3f, 0x7c, 0xe0, 0xc7, 0x06, 0xe0, 0x7f, 0xf0, 0x00,
  0x3e, 0x0e, 0x3f, 0xf8, 0x01, 0x0e, 0xe0, 0x3f, 0x3c, 0xe0, 0x87, 0x06,
  0xe0, 0x7b, 0x90, 0x00, 0x3f, 0x0e, 0x3f, 0xf8, 0x00, 0x04, 0xe0, 0x3f,
  0x38, 0xc0, 0x07, 0x00, 0x00, 0x70, 0x80, 0x00, 0x1e, 0x0e, 0x1e, 0xf8,
  0x00, 0x00, 0xe0, 0x37, 0x98, 0xc0, 0x0f, 0x00, 0x00, 0x70, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x78, 0x00, 0x00, 0xc0, 0x2f, 0xc0, 0xc0, 0x0f, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0e,
  0xc0, 0xc0, 0x0f, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0xe0, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xe0, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

// plane bmp
static unsigned char plane_bmp[] = 
{
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x02, 0x00,
  0x00, 0x80, 0x3f, 0x00, 0x80, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x60, 0x00,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x7f, 0x00, 0x02, 0x00, 0x22, 0x22, 0x22, 0xc0, 0xff,
  0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x82, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x80, 0xa0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x10, 0x86, 0xff,
  0xff, 0x00, 0x00, 0x80, 0x80, 0x0f, 0x58, 0x00, 0x80, 0x00, 0x00, 0x80,
  0x00, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x00,
  0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x10,
  0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x00,
  0x04, 0x00, 0x00, 0x07, 0x00, 0x00, 0x01, 0x00, 0xfa, 0xff, 0xff, 0x01,
  0x00, 0x80, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80,
  0x0b, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x40, 0x0b, 0x00, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x60, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x90,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00
};

// bomb bmp
static unsigned char bombski_bmp[] 
{
  0x00, 0x40, 0x02, 0x00, 0x9c, 0x04, 0x00, 0x42, 0x01, 0x80, 0x21, 0x00,
  0x80, 0x81, 0x02, 0xf0, 0x4f, 0x00, 0xf8, 0x1f, 0x00, 0xfc, 0x3f, 0x00,
  0xfe, 0x7f, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00,
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00,
  0xfe, 0x7f, 0x00, 0xfc, 0x3f, 0x00, 0xf8, 0x1f, 0x00, 0xf0, 0x0f, 0x00
};

// arduino logo bmp
static unsigned char little_arduino_bmp[] 
{
  0xe0, 0x7f, 0x00, 0xfc, 0x0f, 0x00, 0xf0, 0xff, 0x00, 0xfe, 0x1f, 0x00,
  0xf8, 0xff, 0x01, 0xff, 0x3f, 0x00, 0x3c, 0xc0, 0x83, 0x07, 0x78, 0x00,
  0x1e, 0x80, 0xc7, 0x03, 0xf0, 0x00, 0x0e, 0x00, 0xef, 0x01, 0xe0, 0x01,
  0x07, 0x00, 0xfe, 0x00, 0xc0, 0x01, 0x07, 0x00, 0xfe, 0xc0, 0xc0, 0x01,
  0x07, 0x00, 0xfe, 0xc0, 0xc0, 0x01, 0x87, 0x1f, 0xfe, 0xf0, 0xc3, 0x01,
  0x87, 0x1f, 0xfe, 0xf0, 0xc3, 0x01, 0x07, 0x00, 0xfe, 0xc0, 0xc0, 0x01,
  0x07, 0x00, 0xfe, 0xc0, 0xc0, 0x01, 0x07, 0x00, 0xfe, 0x00, 0xc0, 0x01,
  0x0f, 0x00, 0xef, 0x01, 0xe0, 0x01, 0x1e, 0x80, 0xc7, 0x03, 0xf0, 0x00,
  0x3c, 0xc0, 0x83, 0x07, 0x78, 0x00, 0xf8, 0xff, 0x01, 0xff, 0x3f, 0x00,
  0xf0, 0xff, 0x00, 0xfe, 0x1f, 0x00, 0xe0, 0x7f, 0x00, 0xfc, 0x0f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0xe3, 0x48, 0x4e, 0x64, 0x00,
  0x92, 0xa4, 0x49, 0xc4, 0x94, 0x00, 0x92, 0x24, 0x49, 0xc4, 0x95, 0x00,
  0x9e, 0x23, 0x49, 0x44, 0x97, 0x00, 0x92, 0xa6, 0x49, 0x44, 0x96, 0x00,
  0x92, 0xe4, 0x78, 0x4e, 0x64, 0x00
};

// radio_active bmp
static unsigned char radio_active_bmp[] 
{
  0x00, 0x00, 0x7f, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x07, 0x00, 0x00, 0x18,
  0x00, 0x38, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00, 0x01, 0x00, 0x80,
  0x00, 0xc0, 0x04, 0x00, 0x20, 0x01, 0x40, 0x0e, 0x00, 0x70, 0x06, 0x20,
  0x1f, 0x00, 0xf8, 0x04, 0x90, 0x1f, 0x00, 0xf8, 0x09, 0xc8, 0x3f, 0x00,
  0xfc, 0x13, 0xe8, 0x3f, 0x00, 0xfc, 0x17, 0xe4, 0x7f, 0x00, 0xfe, 0x27,
  0xf4, 0xff, 0x00, 0xff, 0x2f, 0xf2, 0xff, 0x00, 0xff, 0x4f, 0xfa, 0xff,
  0x81, 0xff, 0x5f, 0xfa, 0xff, 0x00, 0xff, 0x5f, 0xfa, 0x7f, 0x18, 0xfe,
  0x9f, 0xf9, 0x3f, 0x7e, 0xfe, 0x9f, 0xf9, 0x3f, 0x7e, 0xfc, 0x9f, 0xf9,
  0x3f, 0xff, 0xfc, 0x9f, 0x01, 0x00, 0xff, 0x00, 0x80, 0x01, 0x00, 0x7e,
  0x00, 0x80, 0x01, 0x00, 0x7e, 0x00, 0x80, 0x02, 0x00, 0x18, 0x00, 0x40,
  0x02, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00,
  0xff, 0x00, 0x40, 0x04, 0x00, 0xff, 0x00, 0x20, 0x04, 0x80, 0xff, 0x01,
  0x20, 0x08, 0x80, 0xff, 0x01, 0x10, 0x08, 0xc0, 0xff, 0x03, 0x10, 0x10,
  0xc0, 0xff, 0x03, 0x08, 0x20, 0xe0, 0xff, 0x07, 0x04, 0x40, 0xf0, 0xff,
  0x07, 0x06, 0xc0, 0xf0, 0xff, 0x0f, 0x01, 0x00, 0xf1, 0xff, 0x8f, 0x00,
  0x00, 0xe6, 0xff, 0x63, 0x00, 0x00, 0x18, 0x5a, 0x38, 0x00, 0x00, 0xe0,
  0x00, 0x07, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00
};

// explosion bmp
static unsigned char explosion_bmp[] 
{
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x70, 0x10, 0x18,
  0x10, 0x83, 0x03, 0xc0, 0x03, 0x02, 0x18, 0xc1, 0x18, 0x00, 0x20, 0x46,
  0x31, 0x06, 0x00, 0x47, 0x00, 0x00, 0x20, 0x04, 0x60, 0x00, 0x03, 0x00,
  0x20, 0x8f, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x60, 0x00, 0x40, 0x20,
  0x00, 0x0c, 0x41, 0xf0, 0x01, 0x00, 0xf0, 0x30, 0x08, 0x0c, 0x4c, 0xfc,
  0xff, 0x31, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3f, 0xdf, 0x07, 0xe0, 0x01,
  0x00, 0x40, 0x80, 0x03, 0x1c, 0x0e, 0x02, 0x40, 0x84, 0x41, 0xf8, 0xc1,
  0x31, 0x18, 0x02, 0x40, 0x8c, 0x01, 0xfe, 0x1f, 0x27, 0x30, 0x30, 0x08,
  0x00, 0x04, 0xff, 0x3f, 0x00, 0xf0, 0x00, 0x0c, 0x00, 0x84, 0x03, 0x78,
  0x80, 0xff, 0x01, 0x00, 0x18, 0xc0, 0x01, 0x60, 0xc0, 0x80, 0x73, 0x00,
  0x1c, 0xc0, 0x01, 0x40, 0x60, 0x00, 0x67, 0x00, 0x00, 0xe0, 0xc1, 0xc1,
  0x20, 0x00, 0x06, 0x38, 0x01, 0xf8, 0x1f, 0xf3, 0x3f, 0x3e, 0x04, 0x00,
  0x31, 0xfe, 0x7f, 0xf8, 0x7f, 0xff, 0x1c, 0x00, 0x01, 0x1b, 0xf0, 0xfc,
  0xff, 0x81, 0x30, 0xe8, 0x00, 0x01, 0x80, 0xfd, 0xff, 0x01, 0x70, 0x28,
  0x84, 0x01, 0x00, 0xff, 0xe1, 0x01, 0x60, 0x00, 0x84, 0x05, 0x00, 0x0f,
  0xc0, 0x03, 0x60, 0x06, 0x00, 0x07, 0x00, 0x0e, 0x80, 0xe3, 0x60, 0x00,
  0x00, 0x0f, 0xc0, 0x1f, 0xc0, 0x8f, 0x61, 0x00, 0x62, 0x1e, 0x00, 0x00,
  0x60, 0x08, 0x30, 0x20, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x31,
  0x00, 0xf0, 0x79, 0x00, 0x00, 0xec, 0x87, 0x01, 0x04, 0x84, 0xff, 0x3f,
  0xe0, 0x7f, 0x00, 0x01, 0x04, 0x04, 0xfc, 0x3f, 0xf0, 0x3f, 0x1c, 0x00,
  0x00, 0x00, 0xe0, 0xbf, 0xf9, 0x9f, 0x08, 0x00, 0x40, 0x00, 0xc0, 0xbf,
  0xfd, 0x01, 0x00, 0x00, 0x40, 0x00, 0x81, 0xb8, 0xfd, 0x0c, 0x00, 0x10,
  0x00, 0x00, 0xe1, 0xb0, 0x0d, 0x88, 0x00, 0x00, 0x00, 0x30, 0x00, 0xb6,
  0x4d, 0x80, 0x63, 0x00, 0x30, 0x00, 0x00, 0xb4, 0x4d, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x11, 0xb0, 0x0d, 0x02, 0x10, 0x44, 0x80, 0x01, 0x11, 0xf8,
  0x0c, 0x66, 0x30, 0x4e, 0x80, 0x01, 0x00, 0xc6, 0x0e, 0x40, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x01, 0x60, 0xc0, 0x01, 0x03,
  0xc0, 0x06, 0x04, 0x00, 0x02, 0x00, 0x00, 0x7e, 0x43, 0x40, 0x0c, 0x40,
  0x01, 0x0c, 0x00, 0xfc, 0x3f, 0xc0, 0x00, 0xc2, 0x11, 0x04, 0xc0, 0xf8,
  0x1f, 0x40, 0x10, 0x83, 0x10, 0x00, 0x06, 0xf2, 0x1f, 0x01, 0x38, 0x10,
  0x00, 0x00, 0x0e, 0xb6, 0x0d, 0x07, 0x00, 0x30, 0x18, 0x00, 0x00, 0xb0,
  0x0d, 0x00, 0x00, 0x10, 0x08, 0x38, 0x80, 0xb1, 0x0d, 0x1c, 0x04, 0x01,
  0x00, 0x00, 0x80, 0xb0, 0x9d, 0x10, 0x8c, 0x01, 0x00, 0x00, 0x00, 0xb8,
  0x9d, 0x01, 0x04, 0x00, 0x04, 0x00, 0x10, 0xfc, 0x1d, 0x00, 0x10, 0x00,
  0x26, 0x22, 0x38, 0x3e, 0x1f, 0x40, 0x10, 0x00, 0x02, 0x20, 0x08, 0x0e,
  0x3e, 0x50, 0x04, 0x03, 0x00, 0x03, 0xcc, 0xef, 0xfc, 0x01, 0x1c, 0x01,
  0x00, 0x00, 0xc0, 0xff, 0xfe, 0x01, 0x00, 0x20, 0x00, 0x00, 0xf0, 0xff,
  0xff, 0x07, 0x00, 0x20, 0x06, 0x30, 0xfc, 0xff, 0xff, 0x0f, 0x01, 0x04,
  0x86, 0x01, 0xff, 0xff, 0xff, 0x1f, 0x30, 0x0c, 0x00, 0xf0, 0xff, 0xff,
  0xff, 0x7f, 0x60, 0x40, 0x20, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/// destruction bmp
static unsigned char destruction_bmp[] 
{
  0x10, 0x00, 0x00, 0x02, 0x10, 0x04, 0x06, 0x06, 0x00, 0x0c, 0x02, 0x20,
  0x30, 0x00, 0x00, 0x20, 0x00, 0x82, 0xc0, 0x00, 0x00, 0x83, 0xc1, 0x01,
  0x00, 0x00, 0x01, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0c, 0x20, 0x0c, 0x12,
  0x00, 0x03, 0x0c, 0x36, 0x18, 0x02, 0x00, 0x00, 0x00, 0x08, 0x80, 0x01,
  0x00, 0x10, 0x00, 0x01, 0x78, 0x80, 0x03, 0x20, 0x00, 0x80, 0x00, 0x30,
  0x00, 0x04, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x03, 0x60, 0xc0, 0x21, 0x60,
  0x70, 0x40, 0x30, 0x00, 0x00, 0x02, 0x00, 0x00, 0x04, 0x06, 0x00, 0x06,
  0x06, 0x80, 0x03, 0x07, 0x60, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0xe0,
  0x00, 0xcc, 0x40, 0x08, 0x10, 0x06, 0xc0, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x00, 0x70, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00,
  0x18, 0x02, 0x08, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00,
  0x80, 0x00, 0x01, 0x06, 0x80, 0x01, 0x20, 0x0c, 0x00, 0x00, 0x30, 0x00,
  0x18, 0xc0, 0x18, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x23, 0x00, 0x01,
  0x00, 0x31, 0x00, 0x01, 0x06, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x08, 0x20,
  0x1c, 0x00, 0x00, 0x40, 0xc0, 0x04, 0x00, 0x00, 0x40, 0x84, 0x00, 0x02,
  0x00, 0x80, 0x18, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x04,
  0x0c, 0x10, 0x00, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x80, 0x01, 0x08, 0x01,
  0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x08, 0x00, 0x22, 0xc3, 0x00, 0x20,
  0x22, 0x86, 0x40, 0x60, 0x22, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x03,
  0x98, 0x01, 0x00, 0x00, 0x0a, 0x10, 0x02, 0x00, 0x82, 0x31, 0x06, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};

// cup bmp
static unsigned char cup_bmp[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x0f, 0xf0, 0x00,
  0xc0, 0x01, 0x80, 0x03, 0x60, 0x00, 0x00, 0x06, 0x20, 0x00, 0x00, 0x04,
  0xe0, 0x00, 0x00, 0x07, 0xa0, 0x01, 0x80, 0x05, 0x20, 0x0f, 0xf0, 0x04,
  0x20, 0xf8, 0x1f, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x08, 0x08, 0x04,
  0x20, 0x14, 0x14, 0x04, 0x20, 0x22, 0x22, 0x04, 0x20, 0x41, 0x41, 0x04,
  0x20, 0x63, 0x63, 0x04, 0x20, 0x55, 0x55, 0x04, 0x20, 0x49, 0x49, 0x04,
  0x20, 0x49, 0x49, 0x04, 0x20, 0x49, 0x49, 0x04, 0x20, 0x4b, 0x49, 0x04,
  0x20, 0xfe, 0x7f, 0x04, 0x60, 0xfc, 0x3f, 0x06, 0xa0, 0x01, 0x80, 0x05,
  0x20, 0x0f, 0xf0, 0x04, 0x60, 0xf8, 0x1f, 0x06, 0xc0, 0x00, 0x00, 0x03,
  0x80, 0x01, 0x80, 0x01, 0x00, 0x03, 0xc0, 0x00, 0x00, 0xfe, 0x7f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// destruction2 bmp (one side of the bomb aftermath)
static unsigned char destruction2_bmp[] 
{
  0x10, 0x00, 0x00, 0x02, 0x10, 0x04, 0x06, 0x06, 0x00, 0x0c, 0x02, 0x20,
  0x30, 0x00, 0x00, 0x20, 0x00, 0x82, 0xc0, 0x00, 0x00, 0x83, 0xc1, 0x01,
  0x00, 0x00, 0x01, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0c, 0x20, 0x0c, 0x12,
  0x00, 0x03, 0x0c, 0x36, 0x18, 0x02, 0x00, 0x00, 0x00, 0x08, 0x80, 0x01,
  0x00, 0x10, 0x00, 0x01, 0x78, 0x80, 0x03, 0x20, 0x00, 0x80, 0x00, 0x30,
  0x00, 0x04, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x03, 0x60, 0xc0, 0x21, 0x60,
  0x70, 0x40, 0x30, 0x00, 0x00, 0x02, 0x00, 0x00, 0x04, 0x06, 0x00, 0x06,
  0x06, 0x80, 0x03, 0x07, 0x60, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0xe0,
  0x00, 0xcc, 0x40, 0x08, 0x11, 0x06, 0xc0, 0x00, 0x13, 0x00, 0x0c, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x00, 0x70, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00,
  0x18, 0x02, 0x08, 0x43, 0x00, 0x02, 0x00, 0x60, 0x00, 0x80, 0x01, 0x00,
  0x87, 0x00, 0x01, 0x06, 0x80, 0x01, 0x20, 0x0c, 0x00, 0x00, 0x30, 0x00,
  0x18, 0xc0, 0x18, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x23, 0x00, 0x01,
  0x00, 0x31, 0x00, 0x01, 0x06, 0x00, 0x0c, 0x01, 0x00, 0x60, 0x08, 0x78,
  0x1c, 0x00, 0x00, 0x40, 0xc0, 0x04, 0x00, 0x00, 0x43, 0x84, 0x00, 0x02,
  0x01, 0x80, 0x18, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xc4,
  0x0c, 0x90, 0x01, 0x9c, 0x08, 0x80, 0x00, 0x00, 0x80, 0x01, 0x08, 0x01,
  0x03, 0x00, 0x08, 0x05, 0x00, 0x00, 0x08, 0x1c, 0x22, 0xc3, 0x00, 0x20,
  0x22, 0x86, 0x42, 0x60, 0x22, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x13,
  0x98, 0x01, 0x00, 0x10, 0x0a, 0x10, 0x02, 0x00, 0x00, 0x30, 0x66, 0x06,
  0xff, 0x00, 0x20, 0x24, 0xff, 0x7f, 0x00, 0x60, 0xff, 0xff, 0x7f, 0x00,
  0xff, 0xff, 0xff, 0xff
};

/// finger point bmp
static unsigned char finger_point_bmp[] 
{
  0x00, 0xc0, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00,
  0x00, 0x0c, 0x01, 0x00, 0x00, 0x87, 0x00, 0x00, 0x80, 0x41, 0x00, 0x00,
  0x80, 0xc0, 0xff, 0x3f, 0xff, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x20,
  0x01, 0x00, 0xfe, 0x3f, 0x01, 0x00, 0x40, 0x00, 0x01, 0x00, 0x40, 0x00,
  0x01, 0x00, 0x7c, 0x00, 0x01, 0x00, 0x20, 0x00, 0x01, 0x00, 0x20, 0x00,
  0x01, 0x00, 0x3e, 0x00, 0xff, 0x01, 0x10, 0x00, 0x00, 0x03, 0x10, 0x00,
  0x00, 0xfe, 0x1f, 0x00
};

// bottle bmp 
static unsigned char bottle_bmp[] = 
{
  0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01, 0xf0, 0x01,
  0xf0, 0x01, 0xf8, 0x03, 0x08, 0x02, 0x08, 0x02, 0x04, 0x04, 0x02, 0x08,
  0x01, 0x10, 0x01, 0x10, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f,
  0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f,
  0xff, 0x1f, 0x01, 0x10, 0x01, 0x10, 0x02, 0x08, 0xfc, 0x07
};

// Yosemite Sam bmp 
static unsigned char sam_bmp[] = 
{
  0x00, 0x80, 0xff, 0x0f, 0x00, 0x00, 0xc0, 0x00, 0x38, 0x00, 0x00, 0x60,
  0x00, 0x60, 0x00, 0x00, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x18, 0x00, 0x80,
  0x01, 0x00, 0x0c, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x02, 0x00,
  0x03, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00,
  0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x00, 0x18,
  0x60, 0x00, 0x00, 0x00, 0x10, 0x30, 0x00, 0x00, 0x00, 0x30, 0x10, 0x00,
  0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x00,
  0x40, 0x08, 0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x40, 0x08,
  0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00,
  0x00, 0x40, 0x08, 0x00, 0x00, 0x00, 0x40, 0x08, 0x00, 0x1c, 0x07, 0x40,
  0x08, 0x00, 0xa3, 0x18, 0x40, 0x08, 0x00, 0x41, 0x10, 0x40, 0x08, 0x00,
  0x1d, 0x17, 0x40, 0x08, 0x00, 0xa5, 0x14, 0x40, 0x08, 0x00, 0x45, 0x14,
  0x40, 0x08, 0x00, 0x45, 0x14, 0x40, 0x08, 0x00, 0x45, 0x14, 0x40, 0x08,
  0x00, 0xe5, 0x14, 0x40, 0x08, 0x00, 0xe5, 0x14, 0x40, 0x08, 0xe0, 0x47,
  0x7c, 0x40, 0x10, 0x20, 0xf0, 0x4f, 0x40, 0x10, 0x10, 0x08, 0xc8, 0x20,
  0x20, 0x10, 0x08, 0x88, 0x21, 0x20, 0x08, 0x08, 0x08, 0x11, 0x40, 0x04,
  0x08, 0x08, 0x11, 0x40, 0x04, 0xf8, 0x07, 0x09, 0x80, 0x06, 0x00, 0x01,
  0x05, 0x00, 0x05, 0xfc, 0x01, 0x03, 0x00, 0x04, 0x54, 0x07, 0x01, 0x78,
  0x04, 0x53, 0x04, 0x1d, 0x48, 0x84, 0x51, 0x04, 0x15, 0x48, 0x86, 0x71,
  0x84, 0x16, 0xd8, 0xc7, 0xd1, 0xc4, 0x13, 0x90, 0x44, 0x99, 0xc5, 0x19,
  0x10, 0x64, 0x09, 0x65, 0x08, 0x18, 0x24, 0x0b, 0x25, 0x08, 0x08, 0x26,
  0x0e, 0x67, 0x38, 0x06, 0x27, 0x00, 0xa4, 0x61, 0x83, 0x25, 0x00, 0x24,
  0xc1, 0x41, 0x24, 0x00, 0x24, 0x83, 0x71, 0xa4, 0xff, 0x25, 0xde, 0x1f,
  0xe4, 0x24, 0x27, 0x60, 0x00, 0x74, 0x24, 0x26, 0x00, 0x00, 0x54, 0x24,
  0x66, 0x00, 0x00, 0x5c, 0x7e, 0x4e, 0x00, 0x00, 0x40, 0x42, 0x7a, 0x00,
  0x00, 0x40, 0x42, 0x02, 0x00, 0x00, 0x80, 0xc3, 0x01, 0x00, 0x00, 0xe0,
  0xc3, 0x07, 0x00, 0x00, 0xe0, 0xc3, 0x07, 0x00
};

// signature bmp #unique
static unsigned char signature_bmp[] = 
{
  0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
  0x08, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00,
  0x08, 0x04, 0x60, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x08, 0x04, 0xa0, 0x04,
  0x48, 0x00, 0x00, 0x00, 0x08, 0x04, 0x60, 0x04, 0x28, 0x00, 0x00, 0x00,
  0x08, 0x04, 0x61, 0x7f, 0x38, 0x00, 0x00, 0x00, 0x08, 0x04, 0x60, 0x0c,
  0x18, 0x00, 0x00, 0x00, 0x08, 0x04, 0xe0, 0x3c, 0x18, 0x70, 0xb6, 0x33,
  0x08, 0x74, 0xb5, 0x2e, 0x24, 0x57, 0x95, 0x4a, 0x08, 0x52, 0xbb, 0x33,
  0x66, 0x75, 0x95, 0x2b, 0xc8, 0xca, 0xaa, 0x31, 0xc3, 0x35, 0x95, 0x29,
  0xc8, 0x47, 0x0c, 0x00, 0x7e, 0xef, 0x7d, 0x47, 0x30, 0x00, 0x0a, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x0a, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

// martini glass bmp
static unsigned char martini_glass_bmp[] = 
{
  0x7f, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x03, 0x00, 0x00, 0xc0,
  0x06, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x18,
  0x30, 0x00, 0xfc, 0x0f, 0x60, 0x00, 0x07, 0x06, 0xc0, 0xc0, 0x01, 0x03,
  0x80, 0x7f, 0x80, 0x01, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x06, 0x60, 0x00,
  0x00, 0x0c, 0x30, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x30, 0x0c, 0x00,
  0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xc0, 0x03, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00,
  0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfc, 0x3f, 0x00
};

/*
 *  Pins used to control the cart are defined below.
 *  
 *  Step pin: used to pulse the coils in the motor (rotate shaft)
 *  Direction pin: used to control whether the cart moves forward or backward
 *  Voltage pin: Enables/disables the voltage to the motor controller
 *  
 */
enum motor_control_pins
{
  dirPin = 7,
  stepPin = 23,
  voltagePin = 22
};

/*  
 *  Strength delay enumerated type
 *  
 *  Controls the delay time on the pumps
 *  
 *  Cocktail delays: weak, regular, strong
 *  Pop strength delay.. one size fits all ;)
 *  Shot delays: single or double
 */
enum drink_strength_delay_e
{
  DELAY_COCKTAIL_WEAK = 500,  // cocktail + pop (cocktail part)
  DELAY_COCKTAIL_REG = 1000, // cocktail + pop (cocktail part)
  DELAY_COCKTAIL_STRONG = 2000,  // cocktail + pop (cocktail part)
  DELAY_POP_COCKTAIL_FILL = 4000, // cocktail + pop (pop part)
  DELAY_POP_CHASE = 1000,  // pop intended for chase
  DELAY_POP_MEDIUM = 2000,  // pop only (for the light-hearted) (;
  DELAY_SHOT_SINGLE = 1000,  // shot only
  DELAY_SHOT_DOUBLE = DELAY_SHOT_SINGLE * 2  // shot only
};

/*
 *  HC-SR04 sensor enumerated type
 *  
 *  These definitions are specifc to the Arduino Mega microcontroller
 *  Each entry in the enumerated type holds the bit corresponding to the 
 *  sensor on a given port on the Arduino Mega
 *  
 *  Example: HC-SR04 sensor #1 is attached to pin 34 on the Arduino Mega.
 *           Pin 34 corresponds to bit 4 on Port C so.. 0b00001000 on Port C
 * 
 *  More info on Arduino Mega Ports here: https://www.arduino.cc/en/Hacking/PinMapping2560
 *  
 */
enum hc_sr04_port_bits_e
{
  PORT_BITS_SENSOR_1 = 0b00001000,      // on Port C digital pin 34
  PORT_BITS_SENSOR_2 = 0b00000001,      // on Port C digital pin 37
  PORT_BITS_SENSOR_3 = 0b00000100,      // on Port L digital pin 47
  PORT_BITS_SENSOR_4 = 0b00000001,      // on Port L digital pin 49
  PORT_BITS_SENSOR_5 = 0b00000100,      // on Port B digital pin 51
  PORT_BITS_SENSOR_6 = 0b00001000,      // on Port E digital pin 5
  PORT_BITS_SENSOR_7 = 0b00100000,      // on Port G digital pin 4
  PORT_BITS_SENSOR_8 = 0b00100000,      // on Port E digital pin 3
  PORT_BITS_SENSOR_9 = 0b01000000,      // on Port B digital pin 12
  PORT_BITS_SENSOR_10 = 0b00010000,     // on Port E digital pin 2
  PORT_BITS_SENSOR_TRIGGER = 0b00000100 // on Port C digital pin 35 (simply a trigger no drink to dispense)
};

/*
 *  Enumerated type for HC-SR04 distances (in cm) to sense on the HC-SR04 sensor
 *  
 *  When the distance sensed by the HC-SR04 sensor is less than or equal to the 
 *  distance defined below for the given sensor then we know the cart is under
 *  the spout of a given spirit or pop
 *  
 */
enum hc_sr04_sensor_distance_e
{
  DISTANCE_SPRITE = 4,        // on Port C
  DISTANCE_COKE = 4,          // on Port C
  DISTANCE_LEMONADE = 4,      // on Port L
  DISTANCE_DEW = 4,           // on Port L
  DISTANCE_GIN = 4,           // on Port B
  DISTANCE_VODKA = 4,         // on Port E
  DISTANCE_TEQUILA = 4,       // on Port G
  DISTANCE_WHISKEY = 3,       // on Port E
  DISTANCE_SPICED_RUM = 4,    // on Port B
  DISTANCE_WHITE_RUM = 5      // on Port E
}; //hc_sr04_sensor_distance_t;

/*
 *  Enumerated type defining all liquids that could be dispensed from the cocktail dispenser
 *  Both pops & spirits are defined below.  
 */
enum liquids_e
{
  LIQUIDS_SPRITE = 0,
  LIQUIDS_COKE = 1,
  LIQUIDS_LEMONADE = 2,
  LIQUIDS_DEW = 3,
  LIQUIDS_GIN = 4,
  LIQUIDS_VODKA = 5,
  LIQUIDS_TEQUILA = 6,
  LIQUIDS_WHISKEY = 7,
  LIQUIDS_SPICED_RUM = 8,
  LIQUIDS_WHITE_RUM = 9
};

/*
 *  Enumerated type defining the pumps used on the cocktail dispenser 
 *  
 *  The pump dispensing is not a time sensitive operation.  So digital writes are used 
 *  instead of direct port manipulation.
 *  Spot numbering begins at the spot closest to the beginning point of the cart
 *  (left side of dispenser when looking @ the front of the dispenser)
 *    ie. spot #1 = Sprite
 */
enum pump_pins_e
{
  PUMP_SPRITE = 33,         // spot 1
  PUMP_COKE = 32,           // spot 2
  PUMP_LEMONADE = 31,       // spot 3
  PUMP_DEW = 30,   // spot 4
  PUMP_GIN = 29,            // spot 5
  PUMP_VODKA = 28,          // spot 6 
  PUMP_TEQUILA = 27,        // spot 7
  PUMP_WHISKEY = 26,        // spot 8
  PUMP_SPICED_RUM = 25,     // spot 9
  PUMP_WHITE_RUM = 24       // spot 10
};

/*
 *  Enumerated type defining the ball valves used for each pop/liquor used on the cocktail dispenser
 *  
 *  The ball valve writes do not represent a time sensitive operations.  Once again digital writes are used
 *  instead of port manipulation. 
 */
enum ball_valve_pins_e
{
  BALL_VALVE_SPRITE = 44,       // spot 1
  BALL_VALVE_COKE = 48,         // spot 2
  BALL_VALVE_LEMONADE = 42,     // spot 3
  BALL_VALVE_DEW = 40, // spot 4
  BALL_VALVE_GIN = 36,          // spot 5
  BALL_VALVE_VODKA = 43,        // spot 6
  BALL_VALVE_TEQUILA = 38,      // spot 7
  BALL_VALVE_WHISKEY = 50,      // spot 8
  BALL_VALVE_SPICED_RUM = 52,   // spot 9
  BALL_VALVE_WHITE_RUM = 9      // spot 10
};

enum menu_levels_e
{
  MAIN_MENU_LVL_1,
  MAIN_MENU_LVL_2,
  MAIN_MENU_LVL_3,
  MAIN_MENU_LVL_4,
};

enum menu_states_e
{
  MENU_STATE_COCKTAILS = 0,
  MENU_STATE_SHOTS,
  MENU_STATE_POPS
};

enum cocktail_menu_state_e
{
  COCKTAIL_STATE_WHISKEY_COKE = 0,       
  COCKTAIL_STATE_WHISKEY_LEMONADE,
  COCKTAIL_STATE_WHISKEY_DEW,
  COCKTAIL_STATE_WHISKEY_SPRITE,
  COCKTAIL_STATE_WHITE_RUM_COKE,
  COCKTAIL_STATE_WHITE_RUM_LEMONADE,
  COCKTAIL_STATE_WHITE_RUM_DEW,
  COCKTAIL_STATE_WHITE_RUM_SPRITE,
  COCKTAIL_STATE_VODKA_COKE,
  COCKTAIL_STATE_VODKA_LEMONADE,
  COCKTAIL_STATE_VODKA_DEW,
  COCKTAIL_STATE_VODKA_SPRITE,
  COCKTAIL_STATE_RUM_COKE,
  COCKTAIL_STATE_RUM_LEMONADE,
  COCKTAIL_STATE_RUM_DEW,
  COCKTAIL_STATE_RUM_SPRITE,
  COCKTAIL_STATE_GIN_COKE,
  COCKTAIL_STATE_GIN_LEMONADE,
  COCKTAIL_STATE_GIN_DEW,
  COCKTAIL_STATE_GIN_SPRITE,
  COCKTAIL_STATE_TEQUILA_COKE,
  COCKTAIL_STATE_TEQUILA_LEMONADE,
  COCKTAIL_STATE_TEQUILA_DEW,
  COCKTAIL_STATE_TEQUILA_SPRITE,
  COCKTAIL_STATE_LONG_ISLAND
};

enum shot_menu_state_e
{
  SHOT_STATE_GIN = 0,
  SHOT_STATE_RUM,
  SHOT_STATE_TEQUILA,
  SHOT_STATE_VODKA,
  SHOT_STATE_WHISKEY,
  SHOT_STATE_WHITE_RUM
};

enum pop_menu_state_e
{
  POP_STATE_SPRITE = 0,
  POP_STATE_COKE,
  POP_STATE_LEMONADE,
  POP_STATE_DEW
};

enum cocktail_strength_state_e
{
  COCKTAIL_STRENGTH_STATE_WEAK = 0,
  COCKTAIL_STRENGTH_STATE_REGULAR,
  COCKTAIL_STRENGTH_STATE_STRONG,
};

enum shot_strength_state_e
{
  SHOT_STRENGTH_STATE_SINGLE = 0,
  SHOT_STRENGTH_STATE_DOUBLE,
};

enum pop_strength_state_e
{
  POP_STRENGTH_STATE_SMALL = 0,
  POP_STRENGTH_STATE_MEDIUM,
};

menu_levels_e menu_level = MAIN_MENU_LVL_1;
cocktail_menu_state_e cocktail_state = COCKTAIL_STATE_WHISKEY_COKE;
shot_menu_state_e shot_state = SHOT_STATE_GIN;
pop_menu_state_e pop_state = POP_STATE_SPRITE;
cocktail_strength_state_e cocktail_strength_state = COCKTAIL_STRENGTH_STATE_WEAK;
shot_strength_state_e shot_strength_state = SHOT_STRENGTH_STATE_SINGLE;
pop_strength_state_e pop_strength_state = POP_STRENGTH_STATE_SMALL;
menu_states_e menu_state = MENU_STATE_COCKTAILS;

// "random" seeds defined below to start bouncing balls at different location on display 
int seeds[60] = {  
  2, 35, 8, 13, 72, 7,
  118, 57, 65, 52, 97, 26,
  40, 36, 100, 39, 105, 29,
  121, 61, 121, 51, 91, 60,
  6, 30, 37, 41, 33, 34,
  36, 44, 4, 49, 62, 36,
  51, 58, 6, 30, 77, 46,
  16, 53, 91, 13, 48, 5,
  54, 39, 15, 51, 43, 2,
  104, 1, 107, 63, 63, 32
}; 

// initial headings for each of the 60 balls are defined below
// headings will change dynamically if balls smash into the 
// outer edges of the display
int heading[60] = {
  1, -1, 1, -1, 1, 1,
 -1, 1, -1, 1, -1, -1,
 -1, -1, -1, 1, 1, -1,
  1, -1, -1, -1, -1, -1,
  1, 1, 1, -1, -1, 1,
  1, 1, 1, -1, -1, 1,
  1, 1, 1, 1, -1, 1,
  1, -1, -1, 1, 1, 1,
  1, 1, 1, 1, -1, 1,
  -1, -1, -1, 1, 1, 1
}; 

/* Buttons used on the display are defined below.
 * 
 * The macro for the Ardriblet display assigns the LCD display pins for the actual Adriblet display
 * The macro for the Example display was used on a separate "test" display that I created to flash code to for testing
 *    Unfortunately I didn't use the same pins on each displays.  Hence the macros used below...
 */
#if defined(EXAMPLE_DISPLAY)
const int up_button = 2;
const int down_button = 3;
const int back_button = 18;
const int select_button = 19;
#elif defined(ARDRIBLET)
const int up_button = 18;
const int down_button = 19;
const int back_button = 20;
const int select_button = 21;
#endif

// the flags below are used handle prints to LCD depending on what button was pressed
int upFlag = 0;
int downFlag = 0;

/*  
 *  Variables used for debounce calculations on all the switches
 *  
 *  lastDebounceTimex:  stores the timestamp of the last time a button was pressed.
 *  debounceDelay: 50 ms delay is enough to differentiate switch noise vs. actual button press
 * 
 */
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeSelect = 0;
unsigned long lastDebounceTimeBack = 0;
unsigned long debounceDelay = 50; // ms

// these values are specific to the HC-SR04 distance calculations
unsigned long initial_time_stamp = 0;
unsigned long high_stamp = 0;
int8_t sensed_distance = 127; // max is 127 (reason for 8 bits)..

/*  
 *  @brief          Setup code for the Ardiblet
 *  @name           setup
 *  @parameter[in]  void
 *  @return         void
 */
void setup(void) {
  u8g2.begin(); // resets, configures, clears, and disables power save mode of the display 
  u8g2.setFontMode(1);  // background of text not drawn
  u8g2.setDrawColor(2); // XOR glyph foreground color
  u8g2.setFont(u8g2_font_courB14_tf); // set default font for the display
  u8g2.setBitmapMode(0);  // bmp draws the background too (not transparent)

  // Arduino Mega digital pins setup below
  Serial.begin(9600);
  // nothing connected to pin 0 RX for programming..
  // nothing connected to pin 1 TX for programming..
  pinMode(2, INPUT);    // 10th HC-SR04 sensor
  pinMode(3, INPUT);    // 8th HC-SR04 sensor
  pinMode(4, INPUT);    // 7th HC-SR04 sensor
  pinMode(5, INPUT);    // 6th HC-SR04 sensor
  pinMode(6, OUTPUT);   // no connection 
  pinMode(7, OUTPUT);   // motor control direction pin
  pinMode(8, OUTPUT);   // LCD connection
  pinMode(9, OUTPUT);   // 10th ball valve
  pinMode(10, OUTPUT);  // LCD connection
  pinMode(11, OUTPUT);  // LCD connection
  pinMode(12, INPUT);   // 9th HC-SR04 sensor
  pinMode(13, OUTPUT);  // LCD connection
  pinMode(14, OUTPUT);  // no connection
  pinMode(15, OUTPUT);  // no connection
  pinMode(16, OUTPUT);  // no connection
  pinMode(17, OUTPUT);  // no connection
  pinMode(18, INPUT);   // up button
  pinMode(19, INPUT);   // down button
  pinMode(20, INPUT);   // back button
  pinMode(21, INPUT);   // select button
  pinMode(22, OUTPUT);  // control pin for motor controller
  pinMode(23, OUTPUT);  // step pin on motor controller
  pinMode(24, OUTPUT);  // 10th pump
  pinMode(25, OUTPUT);  // 9th pump
  pinMode(26, OUTPUT);  // 8th pump
  pinMode(27, OUTPUT);  // 7th pump
  pinMode(28, OUTPUT);  // 6th pump
  pinMode(29, OUTPUT);  // 5th pump
  pinMode(30, OUTPUT);  // 4th pump
  pinMode(31, OUTPUT);  // 3rd pump
  pinMode(32, OUTPUT);  // 2nd pump
  pinMode(33, OUTPUT);  // 1st pump
  pinMode(34, INPUT);   // 1st HC-SR04 sensor
  pinMode(35, OUTPUT);  // Trigger pin for all HC-SR04 sensors 
  pinMode(36, OUTPUT);  // 5th ball valve
  pinMode(37, INPUT);   // 2nd HC-SR04 sensor
  pinMode(38, OUTPUT);  // 7th ball valve
  pinMode(39, OUTPUT);  // no connection
  pinMode(40, OUTPUT);  // 4th ball valve
  pinMode(41, OUTPUT);  // no connection 
  pinMode(42, OUTPUT);  // 3rd ball valve
  pinMode(43, OUTPUT);  // 6th ball valve
  pinMode(44, OUTPUT);  // 1st ball valve
  pinMode(45, OUTPUT);  // no connection
  pinMode(46, OUTPUT);  // no connection
  pinMode(47, INPUT);   // 3rd HC-SR04 sensor
  pinMode(48, OUTPUT);  // 2nd ball valve
  pinMode(49, INPUT);   // 4th HC-SR04 sensor
  pinMode(50, OUTPUT);  // 8th ball valve
  pinMode(51, INPUT);   // 5th HC-SR04 sensor
  pinMode(52, OUTPUT);  // 9th ball valve
  pinMode(53, OUTPUT);  // no connection
#if defined(PRINT_INTRO)
  print_intro();
#endif
  interrupts();  // enable the interrupts after the intro is printed so some *jackass* doesn't
                 // advance the menu while we are going through the intro routine
  /** attach interrupts to the buttons below **/ 
  attachInterrupt(digitalPinToInterrupt(select_button), select, LOW); 
  attachInterrupt(digitalPinToInterrupt(down_button), down, LOW);
  attachInterrupt(digitalPinToInterrupt(up_button), up, LOW);
  attachInterrupt(digitalPinToInterrupt(back_button), back, LOW);
  
}

/*
   * Pictorial representation of the menu levels shown below
   *  
   * menuLevel = 0 |                 Main Menu (Cocktails, Shots, Pop)
   *               |                            |
   *               |         -------------------------------------
   *               |         |                  |                |
   * menuLevel = 1 |   Cocktail Menu        Pop Menu         Shot Menu  
   *               |         |                  |                |
   * menuLevel = 2 | Cocktail Strength     Pop Strength    Shot Strength
   *               |         |                  |                |
   * menuLevel = 3 |  Dispense Display    Dispense Display Dispense Display           
   * 
   * @note: The dispense display will print after a selection is made on menuLevel 2
   */
   

/*  
 *  @brief          Infinitely looping code defined below
 *  @name           loop
 *  @parameter[in]  void
 *  @return         void
 *  @note           Function will never return
 */
void loop(void) 
{
  switch (menu_level) 
  {
    case MAIN_MENU_LVL_1://the main menu
      mainMenuPrint(menu_state);
      break;
      
    case MAIN_MENU_LVL_2: //sub menu 
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          cockTailMenuPrint(cocktail_state);
        break;

        case MENU_STATE_SHOTS:
          shotMenuPrint(shot_state);
        break;

        case MENU_STATE_POPS:
          popMenuPrint(pop_state);
        break;
      }
    break;
    
    case MAIN_MENU_LVL_3: // selection here...
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          cocktailStrengthPrint(cocktail_strength_state);
        break;

        case MENU_STATE_SHOTS:
          shotStrengthPrint(shot_strength_state);
        break;

        case MENU_STATE_POPS:
          popStrengthPrint(pop_strength_state);
        break;
      }
    break;
    
    case MAIN_MENU_LVL_4: // here's where the action happens...
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          pourUpaCocktail(cocktail_state);
        break;

        case MENU_STATE_SHOTS:
          pourUpaShot(shot_state);
        break;

        case MENU_STATE_POPS:
          pourUpaPop(pop_state);
        break;
      }
    break;
  }//end of switch
  u8g2.sendBuffer(); // send the menu data to the LCD
}//end of loop

/*  
 *  @brief          Actuate ball valve, activate pump, deactivate pump after delay
 *  @name           dispenseDrink
 *  @parameter[in]  boolean pop - if true we are dispensing pump, else false
 *  @parameter[in]  pump_pins_t pumpPin - contains the pump pin to turn on 
 *  @parameter[in]  ball_valve_pins_t ball_valvePin - contains the ball valve to open 
 *  @return         void
 */
void dispenseDrink(boolean pop, pump_pins_e pumpPin, ball_valve_pins_e ball_valvePin) 
{
  uint32_t strengthDelay = 0;
  switch(menu_state)
  {
    case MENU_STATE_COCKTAILS:
      if(pop) 
      {
        strengthDelay = DELAY_POP_COCKTAIL_FILL;
      }
      else
      {
        switch(cocktail_strength_state)
        {
          case COCKTAIL_STRENGTH_STATE_WEAK:
            strengthDelay = DELAY_COCKTAIL_WEAK;
          break;
          case COCKTAIL_STRENGTH_STATE_REGULAR:
            strengthDelay = DELAY_COCKTAIL_REG;
          break;
          case COCKTAIL_STRENGTH_STATE_STRONG:
            strengthDelay = DELAY_COCKTAIL_STRONG;
          break;
        }
      }
    break;
    
    case MENU_STATE_SHOTS:
      switch(shot_strength_state)
      {
        case SHOT_STRENGTH_STATE_SINGLE:
          strengthDelay = DELAY_SHOT_SINGLE;
        break;
        case SHOT_STRENGTH_STATE_DOUBLE:
          strengthDelay = DELAY_SHOT_DOUBLE;
        break;
      }
    break;
    
    case MENU_STATE_POPS:
      switch(pop_strength_state)
      {
        case POP_STRENGTH_STATE_SMALL:
          strengthDelay = DELAY_POP_CHASE;
        break;
        case POP_STRENGTH_STATE_MEDIUM:
          strengthDelay = DELAY_POP_MEDIUM;
        break;
      }
    break;
  }
  open_delay_and_close_valve(pumpPin, ball_valvePin, strengthDelay);
}//end of dispenseDrink


void open_delay_and_close_valve(pump_pins_e pumpPin, ball_valve_pins_e ball_valvePin, uint32_t delay_time)
{
  digitalWrite(ball_valvePin, HIGH);  // open ball valve
  delay(VALVE_OPEN_TIME); // allow time for ball valve to open fully
  digitalWrite(pumpPin, HIGH);  // turn on the pump
  delay(delay_time); // delay based on the selected strength
  digitalWrite(pumpPin, LOW); // turn off the pump
  digitalWrite(ball_valvePin, LOW); // turn off the ball valve
  delay(VALVE_OPEN_TIME); // allow time for ball valve to close completely 
}


/*  
 *  @brief          Reset all the control values after a drink has been dispensed
 *  @name           reset_control_values
 *  @parameter[in]  void
 *  @return         void
 */
void reset_control_values(void)
{
  reset_menu_level();
  reset_mainmenu_state();
  reset_submenu_states();
  reset_strength_states();
  reset_button_flags();
}// end of reset_control_values


/*  
 *  @brief          Return the cart back to it's starting position (left side of the dispenser)
 *  @name           go_home_youre_drunk
 *  @parameter[in]  void
 *  @return         void
 */
void go_home_youre_drunk(void)
{
  // after the required liquids are dispensed, the motor controller is not powered off
  // instead the function go_home_youre_drunk() is called.  
  delayMicroseconds(7); // prevent double pulse possibility by just delaying initially
  digitalWrite(dirPin, LOW); // drive motor to the left (back home)

  // the line below looks odd, but makes sense w/ a short explanation..
  // spot 1 (far left) is sprite.  To go home we should drive the motor to sprite then drive it
  // a bit more to move the cart to the left of the first spot to prepare the cart for the next run
  drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE); 

  // drive the motor a little more to the left after we get to spot #1 (sprite)
  for (unsigned int i = 0; i < 40000; i++)  // drive a fixed distance further 
  {
    // port manipulation used to send the pulses to the motor coils
    PORTA ^= 0b00000010; 
    delayMicroseconds(7);
    PORTA ^= 0b00000010;
    delayMicroseconds(7);
  }
  digitalWrite(voltagePin, LOW); // turn off the motor controller we made it home and hopefully didn't get a DUI (;
}// end of go_home_youre_drunk


void pourUpaShot(shot_menu_state_e shot_selection)
{
  digitalWrite(voltagePin, LOW); // force the motor controller to the off state
  digitalWrite(dirPin, HIGH); // setup the motor controller to drive motor to the right
  dispenseDisplay(); // display the dispense display

  switch (shot_selection) 
  {
    case SHOT_STATE_GIN:
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
      break;
    case SHOT_STATE_RUM:
      drive_motor_to_sensor(LIQUIDS_SPICED_RUM, DISTANCE_SPICED_RUM);
      dispenseDrink(false, PUMP_SPICED_RUM, BALL_VALVE_SPICED_RUM);
      break;
    case SHOT_STATE_TEQUILA:
      drive_motor_to_sensor(LIQUIDS_TEQUILA, DISTANCE_TEQUILA);
      dispenseDrink(false, PUMP_TEQUILA, BALL_VALVE_TEQUILA);
      break;
    case SHOT_STATE_VODKA:
      drive_motor_to_sensor(LIQUIDS_VODKA, DISTANCE_VODKA);
      dispenseDrink(false, PUMP_VODKA, BALL_VALVE_VODKA);
      break;
    case SHOT_STATE_WHISKEY:
      drive_motor_to_sensor(LIQUIDS_WHISKEY, DISTANCE_WHISKEY);
      dispenseDrink(false, PUMP_WHISKEY, BALL_VALVE_WHISKEY);
      break;
    case SHOT_STATE_WHITE_RUM:
      drive_motor_to_sensor(LIQUIDS_WHITE_RUM, DISTANCE_WHITE_RUM);
      dispenseDrink(false, PUMP_WHITE_RUM, BALL_VALVE_WHITE_RUM);
      break;
  }//end of switch
  go_home_youre_drunk(); // send the cart back to the homeland
  reset_control_values(); // reset all the control values to prepare for the next dispense
  u8g2.clearBuffer(); // clear the display (don't need to sendBuffer here)
}


void pourUpaPop(pop_menu_state_e pop_selection)
{
  digitalWrite(voltagePin, LOW); // force the motor controller to the off state
  digitalWrite(dirPin, HIGH); // setup the motor controller to drive motor to the right
  dispenseDisplay(); // display the dispense display
  switch (pop_selection) 
  {
    case POP_STATE_SPRITE:
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      break;
    case POP_STATE_COKE:
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      break;
    case POP_STATE_LEMONADE:
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      break;
    case POP_STATE_DEW:
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      break;
  }//end of switch
  go_home_youre_drunk(); // send the cart back to the homeland
  reset_control_values(); // reset all the control values to prepare for the next dispense
  u8g2.clearBuffer(); // clear the display (don't need to sendBuffer here)
}


void pourUpaCocktail(cocktail_menu_state_e cocktail_selection)
{
  digitalWrite(voltagePin, LOW); // force the motor controller to the off state
  digitalWrite(dirPin, HIGH); // setup the motor controller to drive motor to the right
  dispenseDisplay(); // display the dispense display

  switch (cocktail_selection) // determine where to drive and what to dispense based on the selection by the user
  {
    case COCKTAIL_STATE_WHISKEY_COKE: //whiskey coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_WHISKEY, DISTANCE_WHISKEY);
      dispenseDrink(false, PUMP_WHISKEY, BALL_VALVE_WHISKEY);
    break;
    case COCKTAIL_STATE_WHISKEY_LEMONADE: // whiskey lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_WHISKEY, DISTANCE_WHISKEY);
      dispenseDrink(false, PUMP_WHISKEY, BALL_VALVE_WHISKEY);
    break;
    case COCKTAIL_STATE_WHISKEY_DEW: //whiskey dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_WHISKEY, DISTANCE_WHISKEY);
      dispenseDrink(false, PUMP_WHISKEY, BALL_VALVE_WHISKEY);
    break;
    case COCKTAIL_STATE_WHISKEY_SPRITE: // whiskey sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_WHISKEY, DISTANCE_WHISKEY);
      dispenseDrink(false, PUMP_WHISKEY, BALL_VALVE_WHISKEY);
    break;
    case COCKTAIL_STATE_WHITE_RUM_COKE: //white rum + coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_WHITE_RUM, DISTANCE_WHITE_RUM);
      dispenseDrink(false, PUMP_WHITE_RUM, BALL_VALVE_WHITE_RUM);
    break;
    case COCKTAIL_STATE_WHITE_RUM_LEMONADE: //white rum + lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_WHITE_RUM, DISTANCE_WHITE_RUM);
      dispenseDrink(false, PUMP_WHITE_RUM, BALL_VALVE_WHITE_RUM);
    break;
    case COCKTAIL_STATE_WHITE_RUM_DEW: //white rum + mountain dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_WHITE_RUM, DISTANCE_WHITE_RUM);
      dispenseDrink(false, PUMP_WHITE_RUM, BALL_VALVE_WHITE_RUM);
    break;
    case COCKTAIL_STATE_WHITE_RUM_SPRITE: //white rum + sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_WHITE_RUM, DISTANCE_WHITE_RUM);
      dispenseDrink(false, PUMP_WHITE_RUM, BALL_VALVE_WHITE_RUM);
    break;
    case COCKTAIL_STATE_VODKA_COKE: //vodka + coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_VODKA, DISTANCE_VODKA);
      dispenseDrink(false, PUMP_VODKA, BALL_VALVE_VODKA);
    break;
    case COCKTAIL_STATE_VODKA_LEMONADE: //vodka + lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_VODKA, DISTANCE_VODKA);
      dispenseDrink(false, PUMP_VODKA, BALL_VALVE_VODKA);
    break;
    case COCKTAIL_STATE_VODKA_DEW: //vodka + mountain dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_VODKA, DISTANCE_VODKA);
      dispenseDrink(false, PUMP_VODKA, BALL_VALVE_VODKA);
    break;
    case COCKTAIL_STATE_VODKA_SPRITE: //vodka + sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_VODKA, DISTANCE_VODKA);
      dispenseDrink(false, PUMP_VODKA, BALL_VALVE_VODKA);
    break;
    case COCKTAIL_STATE_RUM_COKE: //spiced rum + coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_SPICED_RUM, DISTANCE_SPICED_RUM);
      dispenseDrink(false, PUMP_SPICED_RUM, BALL_VALVE_SPICED_RUM);
    break;
    case COCKTAIL_STATE_RUM_LEMONADE: //spiced rum + lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_SPICED_RUM, DISTANCE_SPICED_RUM);
      dispenseDrink(false, PUMP_SPICED_RUM, BALL_VALVE_SPICED_RUM);
    break;
    case COCKTAIL_STATE_RUM_DEW: //spiced rum + mountain dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_SPICED_RUM, DISTANCE_SPICED_RUM);
      dispenseDrink(false, PUMP_SPICED_RUM, BALL_VALVE_SPICED_RUM);
    break;
    case COCKTAIL_STATE_RUM_SPRITE: //spiced rum + sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_SPICED_RUM, DISTANCE_SPICED_RUM);
      dispenseDrink(false, PUMP_SPICED_RUM, BALL_VALVE_SPICED_RUM);
    break;
    case COCKTAIL_STATE_GIN_COKE: //gin + coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
    break;
    case COCKTAIL_STATE_GIN_LEMONADE: //gin + lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
    break;
    case COCKTAIL_STATE_GIN_DEW: //gin + dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
    break;
    case COCKTAIL_STATE_GIN_SPRITE: //gin + sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
    break;
    case COCKTAIL_STATE_TEQUILA_COKE: //tequila + coke
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_TEQUILA, DISTANCE_TEQUILA);
      dispenseDrink(false, PUMP_TEQUILA, BALL_VALVE_TEQUILA);
    break;
    case COCKTAIL_STATE_TEQUILA_LEMONADE: //tequila + lemonade
      drive_motor_to_sensor(LIQUIDS_LEMONADE, DISTANCE_LEMONADE);
      dispenseDrink(true, PUMP_LEMONADE, BALL_VALVE_LEMONADE);
      drive_motor_to_sensor(LIQUIDS_TEQUILA, DISTANCE_TEQUILA);
      dispenseDrink(false, PUMP_TEQUILA, BALL_VALVE_TEQUILA);
    break;
    case COCKTAIL_STATE_TEQUILA_DEW: //tequila + dew
      drive_motor_to_sensor(LIQUIDS_DEW, DISTANCE_DEW);
      dispenseDrink(true, PUMP_DEW, BALL_VALVE_DEW);
      drive_motor_to_sensor(LIQUIDS_TEQUILA, DISTANCE_TEQUILA);
      dispenseDrink(false, PUMP_TEQUILA, BALL_VALVE_TEQUILA);
    break;
    case COCKTAIL_STATE_TEQUILA_SPRITE: //tequila + sprite
      drive_motor_to_sensor(LIQUIDS_SPRITE, DISTANCE_SPRITE);
      dispenseDrink(true, PUMP_SPRITE, BALL_VALVE_SPRITE);
      drive_motor_to_sensor(LIQUIDS_TEQUILA, DISTANCE_TEQUILA);
      dispenseDrink(false, PUMP_TEQUILA, BALL_VALVE_TEQUILA);
    break;
    case COCKTAIL_STATE_LONG_ISLAND: //long island
    // @TODO complete the long island cocktail dump
      drive_motor_to_sensor(LIQUIDS_COKE, DISTANCE_COKE);
      dispenseDrink(true, PUMP_COKE, BALL_VALVE_COKE);
      drive_motor_to_sensor(LIQUIDS_GIN, DISTANCE_GIN);
      dispenseDrink(false, PUMP_GIN, BALL_VALVE_GIN);
    break;
  }//end of switch
  go_home_youre_drunk(); // send the cart back to the homeland
  reset_control_values(); // reset all the control values to prepare for the next dispense
  u8g2.clearBuffer(); // clear the display (don't need to sendBuffer here)
}


/*  
 *  @brief          This is where all the magic happens.  This function keeps driving the motor and 
 *                  continuously checks the HC-SR04 sensor to determine if the specified distance has
 *                  been sensed.
 *  @name           drive_motor_to_sensor
 *  @parameter[in]  liquids_t sensor - the HC-SR04 pertaining to the spot of the liquid
 *  @parameter[in]  hc_sr04_sensor_distance_t echo_distance - the sensed distance that will stop the motor
 *  @return         void
 *  
 *  @note           HC-SR04 sensors are used to calculate the distance from the sensor to the cart by using 
 *                  the speed of sound.  The approximate distance can be calculated by using the following equation:
 *                  distance(in cm) = (time_sound_pulse_received - time_sound_pulse_sent) / (2 * speed_of_sound_cm)
 *  @note           Direct port manipulation is used here because functions provided by Arduino such as
 *                  PulseIn() take too long to calculate a distance (they use digitalRead calls which are painfully slow).
 *                  The code cannot support this because we actively check the HC-SR04 sensors to determine the distance.
 *                  If we take extra time to calculate a distance then the cart would move significantly slower and would
 *                  be quite jerky in motion.
 *  @note           PORT Manipulation basics:
 *                    * DDRX register for Port 'X' returns whether the pin (corresponding to the bit in the PORT) is an INPUT or OUTPUT (1 or 0)
 *                    * PORTX register for Port 'X' is used to directly modify the state of a pin in the port
 *                      - writing a 1 to the bit in the port will change the pin attached to that port to an input
 *                      - writing a 0 to the bit in the port will change the pin attached to that port to an output
 *                    * PINX register for Port 'X' reads the state of inputs on the Port
 *  @note           PORTA bit 2 is the step pin on the motor controller (you will see that this port is updated                
 *                  throughout the function).  This keeps the motor moving while we do our HC-SR04 checks.
 *                  
 *  @note           The function deserves a breakdown because a lot is going on here.
 *                    1st:  The motor controller is enabled
 *                    2nd:  The motor begins moving by manipulating bit 2 of PORTA
 *                    3rd:  The trigger pin (transmitter) for the HC-SR04 sensors is pulsed low then high (15 us active time) then low
 *                          * The trigger pin sens a fixed duration pulse of sound 
 *                    4th:  The echo pin (receiver) listens for the sound pulse & waits (blocks).
 *                    5th:  The blocking loop breaks when the receiver detects the sound pulse
 *                          * The loop breaks at the first sign of the sound puls
 *                    6th:  An initial timestamp is taken here
 *                    7th:  The echo pin is monitored.  A loop is used to block until the echo pin no longer detects the incident sound pulse
 *                    8th:  The blocking loop breaks after echo pin no longer detects the incident sound pulse
 *                    9th:  A final timestamp is taken here
 *                    10th: The difference in the timestamp represents the time that the transmitted sound pulse traveled
 *                    11th: The calculated distance is calculated and stored into the variable sensed_distance
 *                    12th: Check the calculated distance (sensed_distance) against the expected distance (echo_distance) after a 10 ms delay
 *                    13th: Repeat this process every 10 ms (100 times per second)
 * 
 * @note            The HC-SR04 sensor for the drink is monitored 100 times a second.  The rate of speed of the motor
 *                  will prevent the cart from stopping in different noticeable locations.  In other words the rate that the 
 *                  sensors are monitored contribute to a stopping distance that is within the tolerable range of a given spout.
 *
 * @note            The code is kept local to this function (time spent passing variables to a different function will slow down the motor)
 */
boolean drive_motor_to_sensor(liquids_e sensor, hc_sr04_sensor_distance_e echo_distance)
{
  // each PORT toggle or set takes roughly 2 us
  digitalWrite(voltagePin, HIGH); // digitalWrites take a quarter century to write to pin, but ok here since no time sensitive code is being executed..

  while (sensed_distance > echo_distance) // when distance <= echo_distance then loop breaks
  {
    // All the code in this loop is intended to be wrapped with moving the motor
    PORTA ^= 0b00000010; // step motor 
    PORTC &= 0b11111011; // trigPin LOW
    delayMicroseconds(3); 
    PORTA ^= 0b00000010; 
    PORTC |= 0b00000100; // trigPin HIGH
    delayMicroseconds(3);
    PORTA ^= 0b00000010;
    delayMicroseconds(3);
    PORTA ^= 0b00000010;
    delayMicroseconds(3);
    PORTA ^= 0b00000010;
    PORTC &= 0b11111011; // trigPin LOW
    delayMicroseconds(3);
    PORTA ^= 0b00000010;
    switch (sensor) // Determine which sensor to use
    {
      /*  I'm going to give an example of what is going on in hopes of preventing an inbox blow up
       *  Take for example the case where we wish to dispense sprite
       *    1st:  while(!(PINC & sensor_1_bits)) 
       *      This simply waits until the pulse sent from the HC_SR04 sensor is reflected back to the sensor
       *      We drive the motor while we wait.
       *    2nd:  stamp the time when the pulse is reflected back (stamp at the first sign of the pulse)
       *    3rd:  while(PINC & sensor_1_bits)
       *      Wait until the pulse is no longer incident on the RX side of sensor
       *      We drive the motor while we wait.
       *    4th: stamp the time at the end of the received pulse
       *    5th: calculate the sensed distance in cm
       *      We drive the motor while doing calculation
       *    6th: while ((micros() - high_stamp) < 10000)
       *      Drive the motor another 10 ms (will have negligible effect on the sensed distance)
       *      Need to allow some downtime between sound pulses or else they will interfere with one 
       *      another.  10 ms was arbitrarily chosen here.
       *    7th: while (sensed_distance > echo_distance)
       *      If sensed_distance <= echo_distance break! Stop the motor!
       */
      case LIQUIDS_SPRITE:
        while (!(PINC & PORT_BITS_SENSOR_1)) // PORTC 0b00001000 
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while (PINC & PORT_BITS_SENSOR_1)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_COKE:
        while (!(PINC & PORT_BITS_SENSOR_2)) //  sensor_2_bits = 0b00000001, // on Port C
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while (PINC & PORT_BITS_SENSOR_2) // PORTC 0b00000001
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)  
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_LEMONADE:
        while (!((PINL & PORT_BITS_SENSOR_3) >> 2)) // PORTL 0b00000100
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while (((PINL & PORT_BITS_SENSOR_3) >> 2)) // PORTL 0b00000100
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_DEW:
        while (!(PINL & PORT_BITS_SENSOR_4)) // PORTL 0b00000001
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while (PINL & PORT_BITS_SENSOR_4) // PORTL 0b00000001
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_GIN:
        while (!((PINB & PORT_BITS_SENSOR_5) >> 2)) // PORTB 0b00000100
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PINB & PORT_BITS_SENSOR_5) >> 2) // PORTB 0b00000100
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_VODKA:
        while (!((PINE & PORT_BITS_SENSOR_6) >> 3)) // PORTE 0b00001000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PINE & PORT_BITS_SENSOR_6) >> 3) // PORTE 0b00001000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_TEQUILA:
        while (!((PING & PORT_BITS_SENSOR_7) >> 5)) // PORTG 0b00100000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PING & PORT_BITS_SENSOR_7) >> 5) // PORTG 0b00100000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_WHISKEY:
        while (!((PINE & PORT_BITS_SENSOR_8) >> 5)) // PORTE 0b00100000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PINE & PORT_BITS_SENSOR_8) >> 5) // PORTE 0b00100000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_SPICED_RUM:
        while (!((PINB & PORT_BITS_SENSOR_9) >> 6)) // PORTB 0b01000000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PINB & PORT_BITS_SENSOR_9) >> 6) // PORTB 0b01000000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
      case LIQUIDS_WHITE_RUM:
        while (!((PINE & PORT_BITS_SENSOR_10) >> 4)) // PORTB 0b01000000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        initial_time_stamp = micros();
        while ((PINE & PORT_BITS_SENSOR_10) >> 4) // PORTB 0b01000000
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010; // toggle step pin (1 us)
        }
        high_stamp = micros();
        PORTA ^= 0b00000010; // toggle step pin (1 us)
        sensed_distance = (high_stamp - initial_time_stamp) / US_ROUNDTRIP_CM; // approx 57 microseconds for sound to travel 2 cm (round trip)
        PORTA ^= 0b00000010;
        while ((micros() - high_stamp) < 10000)
        {
          delayMicroseconds(3);
          PORTA ^= 0b00000010;
        }
        break;
    }
  }
  sensed_distance = 127; // reinitialize to largest signed 8 bit value (don't wait until the reinitialization of the control values)
  return true;  // if we don't reach this then we have bigger problems
}// drive_motor_to_sensor


/*
 * @function:   select( void ) 
 * @param[in]:  void
 * @return:     void
 * @note:       This function defines the ISR for the select button.
 * 
 * @note:       The select ISR advances the menu forward.  Once the lowest level
 *              of the menu has been reached, the function reinitializes all the
 *              variables associated with the menu for the next user.
 * 
 * @note:       Notice that the buttons are properly debounced prior to
 *              execution of the code block.
 */ 
void select() 
{

  if ((millis() - lastDebounceTimeSelect) > debounceDelay)
    lastDebounceTimeSelect = millis();
  else
    return;
  switch(menu_level)
  {
    case MAIN_MENU_LVL_1:
      menu_level = MAIN_MENU_LVL_2;
    break;
    case MAIN_MENU_LVL_2:
      menu_level = MAIN_MENU_LVL_3;
    break;
    case MAIN_MENU_LVL_3:
      menu_level = MAIN_MENU_LVL_4;
    break;
    default:
    break;
  }
}// end of select

/*
 * @function:   dispenseDisplay( void )
 * @param[in]:  void
 * @return:     void
 * @note:       This function prints the text & status of the drink on the display
 *              while the drink is being dispensed
 */
void dispenseDisplay() 
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_courB10_tf);
  u8g2.drawStr(5, 10, "Dispensing...");
  u8g2.sendBuffer();
} //end of dispenseDisplay

/*
 * @function:   back( void )
 * @param[in]:  void
 * @return:     void
 * 
 * @note:       This function defines the ISR for the back (return) button.
 * @note:       The back ISR prints the previous menu with the shaded region initialized
 *              to the first selection on the menu.
 * @note:       Notice that the buttons are properly debounced prior to
 *              execution of the code block.
 */
void back() 
{
  if ((millis() - lastDebounceTimeBack) > debounceDelay) lastDebounceTimeBack = millis();
  else return;  // switch noise
  reset_button_flags();
  switch(menu_level)
  {
    case MAIN_MENU_LVL_1:
      // do nothing! 
    break;
    case MAIN_MENU_LVL_2:
      menu_level = MAIN_MENU_LVL_1;
    break;
    case MAIN_MENU_LVL_3:
      menu_level = MAIN_MENU_LVL_2;
    break;
    case MAIN_MENU_LVL_4:
    break;
  }
  // the checks below are after menu_level has been decremented (3->2 || 2->1)
  if(MAIN_MENU_LVL_1 == menu_level)
  {
    reset_mainmenu_state();
    reset_submenu_states();
  }
  else if (MAIN_MENU_LVL_2 == menu_level) 
  {
    reset_submenu_states();
    reset_strength_states();
  }
}//end of back

/*
 * @function:   down( void )
 * @param[in]:  void
 * @return:     void
 * @note:       This function defines the ISR for the down button.
 * 
 * @note:       The down ISR controls the location of the shaded box used to
 *              indicate the current selection on the menu.
 * 
 * @note:       Notice that the buttons are properly debounced prior to
 *              execution of the code block.
 * 
 */ 
void down() 
{
  if ((millis() - lastDebounceTimeDown) > debounceDelay) lastDebounceTimeDown = millis();
  else return;
  downFlag = 1;
  switch (menu_level) {
    case MAIN_MENU_LVL_1: 
      if(MENU_STATE_POPS == menu_state) menu_state = MENU_STATE_COCKTAILS;
      else menu_state = (int)menu_state + 1;
    break;
      
    case MAIN_MENU_LVL_2:
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          if(COCKTAIL_STATE_LONG_ISLAND == cocktail_state) cocktail_state = COCKTAIL_STATE_WHISKEY_COKE;
          else cocktail_state = (int)cocktail_state + 1;
        break;

        case MENU_STATE_SHOTS:
          if(SHOT_STATE_WHITE_RUM == shot_state) shot_state = SHOT_STATE_GIN;
          else shot_state = (int)shot_state + 1;
        break;

        case MENU_STATE_POPS:
          if(POP_STATE_DEW == pop_state) pop_state = POP_STATE_SPRITE;
          else pop_state = (int)pop_state + 1;
        break;
      }
    break;
      
    case MAIN_MENU_LVL_3:
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          if(cocktail_strength_state == COCKTAIL_STRENGTH_STATE_STRONG) cocktail_strength_state = COCKTAIL_STRENGTH_STATE_WEAK;
          else cocktail_strength_state = (int)cocktail_strength_state + 1;
        break;
        case MENU_STATE_SHOTS:
          if(shot_strength_state == SHOT_STRENGTH_STATE_DOUBLE) shot_strength_state = SHOT_STRENGTH_STATE_SINGLE;
          else shot_strength_state = (int)shot_strength_state + 1;
        break;
        case MENU_STATE_POPS:
          if(pop_strength_state == POP_STRENGTH_STATE_MEDIUM) pop_strength_state = POP_STRENGTH_STATE_SMALL;
          else pop_strength_state = (int)pop_strength_state + 1;
        break;
      }
    break;
      
  }//end of switch

}//end of down

/*
 * @function:   up( void )
 * @param[in]:  void
 * @return:     void
 * 
 * @note:       This function defines the ISR for the up button.
 * 
 * @note:       The up ISR controls the location of the shaded box used to
 *              indicate the current selection on the menu.
 * 
 * @note:       Notice that the buttons are properly debounced prior to
 *              execution of the code block.
 * 
 */ 
void up() 
{
  if ((millis() - lastDebounceTimeDown) > debounceDelay) lastDebounceTimeDown = millis();
  else return;
  upFlag = 1;
  switch (menu_level) 
  {
    case MAIN_MENU_LVL_1:  //main menu
      if(MENU_STATE_COCKTAILS == menu_state) menu_state = MENU_STATE_POPS;
      else menu_state = (int)menu_state - 1;
    break;
    case MAIN_MENU_LVL_2:  //sub menu
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          if(COCKTAIL_STATE_WHISKEY_COKE == cocktail_state) cocktail_state = COCKTAIL_STATE_LONG_ISLAND;
          else cocktail_state = (int)cocktail_state - 1;
        break;
        case MENU_STATE_SHOTS:
          if(SHOT_STATE_GIN == shot_state) shot_state = SHOT_STATE_WHITE_RUM;
          else shot_state = (int)shot_state - 1;
        break;
        case MENU_STATE_POPS:
          if(POP_STATE_SPRITE == pop_state) pop_state = POP_STATE_DEW;
          else pop_state = (int)pop_state - 1;
        break;
      }
    break;
    case MAIN_MENU_LVL_3:  //selection menu
      switch(menu_state)
      {
        case MENU_STATE_COCKTAILS:
          if(cocktail_strength_state == COCKTAIL_STRENGTH_STATE_WEAK) cocktail_strength_state = COCKTAIL_STRENGTH_STATE_STRONG;
          else cocktail_strength_state = (int)cocktail_strength_state - 1;
        break;

        case MENU_STATE_SHOTS:
          if(shot_strength_state == SHOT_STRENGTH_STATE_SINGLE) shot_strength_state = SHOT_STRENGTH_STATE_DOUBLE;
          else shot_strength_state = (int)shot_strength_state - 1;
        break;

        case MENU_STATE_POPS:
          if(pop_strength_state == POP_STRENGTH_STATE_SMALL) pop_strength_state = POP_STRENGTH_STATE_MEDIUM;
          else pop_strength_state = (int)pop_strength_state - 1;
        break;
      }
    break;
  }//end of switch

}//end of up


/* 
 * @function:   popStrengthPrint( void )
 * @param[in]:  void 
 * @return:     void 
 * @about:      Prints the strength choices for pop.
 *              Small: (chase), Regular (x oz), Tall (y oz) 
 *  
 */  
void popStrengthPrint(pop_strength_state_e pop_strength) 
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_14b_tf);

  switch(pop_strength) 
  {
    case POP_STRENGTH_STATE_SMALL:
      u8g2.drawBox(0, 2, 110, 20);
      u8g2.drawStr(5, 18, "Small (chase)");
      u8g2.drawStr(5, 33, "Medium (1/2 can)");
    break;

    case POP_STRENGTH_STATE_MEDIUM:
      u8g2.drawBox(0, 19, 110, 17);
      u8g2.drawStr(5, 18, "Small (chase)");
      u8g2.drawStr(5, 33, "Medium (1/2 can)");
    break;
  }//end of switch
  u8g2.sendBuffer();
  return;
}//end of popStrengthPrint


/*
 * @function:   shotStrengthPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
 *              box around the text (indicating possible selections)
 * @return:     void
 * 
 * @note:       Single (x oz), Double (y oz)
 * 
 */
void shotStrengthPrint(shot_strength_state_e strength) 
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_14b_tf);

  switch (strength) 
  {
    case SHOT_STRENGTH_STATE_SINGLE:
      u8g2.drawStr(5, 18, "Single");
      u8g2.drawBox(0, 2, 110, 20);
      u8g2.drawStr(5, 36, "Double");
    break;
    case SHOT_STRENGTH_STATE_DOUBLE:
      u8g2.drawStr(5, 18, "Single");
      u8g2.drawBox(0, 20, 110, 20);
      u8g2.drawStr(5, 36, "Double");
    break;   
  }//end of switch
  u8g2.sendBuffer();
  return;
}//end of shotStrengthPrint


/*
 * @function:   strengthPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
 *              box around the text (indicating possible selections)
 * @return:     void
 * 
 * @note:       Weak: chase, Regular (x oz), Tall (y oz)
 * 
 * 
 */ 
void cocktailStrengthPrint(cocktail_strength_state_e strength) 
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_14b_tf);
  switch (strength) 
  {
    case COCKTAIL_STRENGTH_STATE_WEAK:
      u8g2.drawStr(5, 18, "Weak");
      u8g2.drawBox(0, 2, 110, 20);
      u8g2.drawStr(5, 36, "Regular");
      u8g2.drawStr(5, 54, "Strong");
      break;
    case COCKTAIL_STRENGTH_STATE_REGULAR:
      u8g2.drawStr(5, 18, "Weak");
      u8g2.drawBox(0, 20, 110, 20);
      u8g2.drawStr(5, 36, "Regular");
      u8g2.drawStr(5, 54, "Strong");
      break;
    case COCKTAIL_STRENGTH_STATE_STRONG:
      u8g2.drawStr(5, 18, "Weak");
      u8g2.drawBox(0, 40, 110, 20);
      u8g2.drawStr(5, 36, "Regular");
      u8g2.drawStr(5, 54, "Strong");
      break;
  }//end of switch
  u8g2.sendBuffer();
  return;

}//end of strengthPrint

/*
 * @function:   cockTailMenuPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
 *              box around the text (indicating possible selections)
 * @return:     void
 * 
 * @note:       The cocktail menu is printed in this function.
 * 
 */ 
void cockTailMenuPrint(cocktail_menu_state_e cocktail_state) 
{
  u8g2.setFont(u8g2_font_t0_11b_tf);
  switch (cocktail_state) 
  {
    case COCKTAIL_STATE_WHISKEY_COKE:  //whiskey + coke
      u8g2.clearBuffer();
      u8g2.drawBox(0, 0, 123, 12);
      u8g2.drawStr(5, 10, "Whiskey + Coke");
      u8g2.drawStr(5, 21, "Whiskey + Lemonade");
      u8g2.drawStr(5, 32, "Whiskey + Dew");
      u8g2.drawStr(5, 43, "Whiskey + Sprite");
      u8g2.drawStr(5, 54, "White Rum + Coke");
      u8g2.drawStr(5, 64, "White Rum + Lemonade");
      u8g2.sendBuffer();
      reset_button_flags();
    break;

    case COCKTAIL_STATE_WHISKEY_LEMONADE:  //whiskey + lemonade
      if (upFlag) 
      {
        //cockTailCount = 1;
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Lemonade");
        u8g2.drawStr(5, 21, "Whiskey + Dew");
        u8g2.drawStr(5, 32, "Whiskey + Sprite");
        u8g2.drawStr(5, 43, "White Rum + Coke");
        u8g2.drawStr(5, 54, "White Rum + Lemonade");
        u8g2.drawStr(5, 64, "White Rum + Dew");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 11, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Coke");
        u8g2.drawStr(5, 21, "Whiskey + Lemonade");
        u8g2.drawStr(5, 32, "Whiskey + Dew");
        u8g2.drawStr(5, 43, "Whiskey + Sprite");
        u8g2.drawStr(5, 54, "White Rum + Coke");
        u8g2.drawStr(5, 64, "White Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHISKEY_DEW:  //whiskey + mountain dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Dew");
        u8g2.drawStr(5, 21, "Whiskey + Sprite");
        u8g2.drawStr(5, 32, "White Rum + Coke");
        u8g2.drawStr(5, 43, "White Rum + Lemonade");
        u8g2.drawStr(5, 54, "White Rum + Dew");
        u8g2.drawStr(5, 64, "White Rum + Sprite");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 22, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Coke");
        u8g2.drawStr(5, 21, "Whiskey + Lemonade");
        u8g2.drawStr(5, 32, "Whiskey + Dew");
        u8g2.drawStr(5, 43, "Whiskey + Sprite");
        u8g2.drawStr(5, 54, "White Rum + Coke");
        u8g2.drawStr(5, 64, "White Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHISKEY_SPRITE:  //whiskey + sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Sprite");
        u8g2.drawStr(5, 21, "White Rum + Coke");
        u8g2.drawStr(5, 32, "White Rum + Lemonade");
        u8g2.drawStr(5, 43, "White Rum + Dew");
        u8g2.drawStr(5, 54, "White Rum + Sprite");
        u8g2.drawStr(5, 64, "Vodka + Coke");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 33, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Coke");
        u8g2.drawStr(5, 21, "Whiskey + Lemonade");
        u8g2.drawStr(5, 32, "Whiskey + Dew");
        u8g2.drawStr(5, 43, "Whiskey + Sprite");
        u8g2.drawStr(5, 54, "White Rum + Coke");
        u8g2.drawStr(5, 64, "White Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHITE_RUM_COKE:  //white rum + coke
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Coke");
        u8g2.drawStr(5, 21, "White Rum + Lemonade");
        u8g2.drawStr(5, 32, "White Rum + Dew");
        u8g2.drawStr(5, 43, "White Rum + Sprite");
        u8g2.drawStr(5, 54, "Vodka + Coke");
        u8g2.drawStr(5, 64, "Vodka + Lemonade");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 44, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Coke");
        u8g2.drawStr(5, 21, "Whiskey + Lemonade");
        u8g2.drawStr(5, 32, "Whiskey + Dew");
        u8g2.drawStr(5, 43, "Whiskey + Sprite");
        u8g2.drawStr(5, 54, "White Rum + Coke");
        u8g2.drawStr(5, 64, "White Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHITE_RUM_LEMONADE:  //white rum + lemonade
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Lemonade");
        u8g2.drawStr(5, 21, "White Rum + Dew");
        u8g2.drawStr(5, 32, "White Rum + Sprite");
        u8g2.drawStr(5, 43, "Vodka + Coke");
        u8g2.drawStr(5, 54, "Vodka + Lemonade");
        u8g2.drawStr(5, 64, "Vodka + Dew");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Coke");
        u8g2.drawStr(5, 21, "Whiskey + Lemonade");
        u8g2.drawStr(5, 32, "Whiskey + Dew");
        u8g2.drawStr(5, 43, "Whiskey + Sprite");
        u8g2.drawStr(5, 54, "White Rum + Coke");
        u8g2.drawStr(5, 64, "White Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHITE_RUM_DEW:  //white rum + mountain dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Dew");
        u8g2.drawStr(5, 21, "White Rum + Sprite");
        u8g2.drawStr(5, 32, "Vodka + Coke");
        u8g2.drawStr(5, 43, "Vodka + Lemonade");
        u8g2.drawStr(5, 54, "Vodka + Dew");
        u8g2.drawStr(5, 64, "Vodka + Sprite");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Lemonade");
        u8g2.drawStr(5, 21, "Whiskey + Dew");
        u8g2.drawStr(5, 32, "Whiskey + Sprite");
        u8g2.drawStr(5, 43, "White Rum + Coke");
        u8g2.drawStr(5, 54, "White Rum + Lemonade");
        u8g2.drawStr(5, 64, "White Rum + Dew");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_WHITE_RUM_SPRITE:  //white rum + sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Sprite");
        u8g2.drawStr(5, 21, "Vodka + Coke");
        u8g2.drawStr(5, 32, "Vodka + Lemonade");
        u8g2.drawStr(5, 43, "Vodka + Dew");
        u8g2.drawStr(5, 54, "Vodka + Sprite");
        u8g2.drawStr(5, 64, "Rum + Coke");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Dew");
        u8g2.drawStr(5, 21, "Whiskey + Sprite");
        u8g2.drawStr(5, 32, "White Rum + Coke");
        u8g2.drawStr(5, 43, "White Rum + Lemonade");
        u8g2.drawStr(5, 54, "White Rum + Dew");
        u8g2.drawStr(5, 64, "White Rum + Sprite");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_VODKA_COKE:  //vodka + coke
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Coke");
        u8g2.drawStr(5, 21, "Vodka + Lemonade");
        u8g2.drawStr(5, 32, "Vodka + Dew");
        u8g2.drawStr(5, 43, "Vodka + Sprite");
        u8g2.drawStr(5, 54, "Rum + Coke");
        u8g2.drawStr(5, 64, "Rum + Lemonade");
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Whiskey + Sprite");
        u8g2.drawStr(5, 21, "White Rum + Coke");
        u8g2.drawStr(5, 32, "White Rum + Lemonade");
        u8g2.drawStr(5, 43, "White Rum + Dew");
        u8g2.drawStr(5, 54, "White Rum + Sprite");
        u8g2.drawStr(5, 64, "Vodka + Coke");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_VODKA_LEMONADE:  //vodka + lemonade
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Lemonade");
        u8g2.drawStr(5, 21, "Vodka + Dew");
        u8g2.drawStr(5, 32, "Vodka + Sprite");
        u8g2.drawStr(5, 43, "Rum + Coke");
        u8g2.drawStr(5, 54, "Rum + Lemonade");
        u8g2.drawStr(5, 64, "Rum + Dew");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Coke");
        u8g2.drawStr(5, 21, "White Rum + Lemonade");
        u8g2.drawStr(5, 32, "White Rum + Dew");
        u8g2.drawStr(5, 43, "White Rum + Sprite");
        u8g2.drawStr(5, 54, "Vodka + Coke");
        u8g2.drawStr(5, 64, "Vodka + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_VODKA_DEW:  //vodka + mountain dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Dew");
        u8g2.drawStr(5, 21, "Vodka + Sprite");
        u8g2.drawStr(5, 32, "Rum + Coke");
        u8g2.drawStr(5, 43, "Rum + Lemonade");
        u8g2.drawStr(5, 54, "Rum + Dew");
        u8g2.drawStr(5, 64, "Rum + Sprite");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Lemonade");
        u8g2.drawStr(5, 21, "White Rum + Dew");
        u8g2.drawStr(5, 32, "White Rum + Sprite");
        u8g2.drawStr(5, 43, "Vodka + Coke");
        u8g2.drawStr(5, 54, "Vodka + Lemonade");
        u8g2.drawStr(5, 64, "Vodka + Dew");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_VODKA_SPRITE:  //vodka + sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Sprite");
        u8g2.drawStr(5, 21, "Rum + Coke");
        u8g2.drawStr(5, 32, "Rum + Lemonade");
        u8g2.drawStr(5, 43, "Rum + Dew");
        u8g2.drawStr(5, 54, "Rum + Sprite");
        u8g2.drawStr(5, 64, "Gin + Coke");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Dew");
        u8g2.drawStr(5, 21, "White Rum + Sprite");
        u8g2.drawStr(5, 32, "Vodka + Coke");
        u8g2.drawStr(5, 43, "Vodka + Lemonade");
        u8g2.drawStr(5, 54, "Vodka + Dew");
        u8g2.drawStr(5, 64, "Vodka + Sprite");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_RUM_COKE:  //rum + coke
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Coke");
        u8g2.drawStr(5, 21, "Rum + Lemonade");
        u8g2.drawStr(5, 32, "Rum + Dew");
        u8g2.drawStr(5, 43, "Rum + Sprite");
        u8g2.drawStr(5, 54, "Gin + Coke");
        u8g2.drawStr(5, 64, "Gin + Lemonade");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "White Rum + Sprite");
        u8g2.drawStr(5, 21, "Vodka + Coke");
        u8g2.drawStr(5, 32, "Vodka + Lemonade");
        u8g2.drawStr(5, 43, "Vodka + Dew");
        u8g2.drawStr(5, 54, "Vodka + Sprite");
        u8g2.drawStr(5, 64, "Rum + Coke");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_RUM_LEMONADE:  //rum + lemonade
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Lemonade");
        u8g2.drawStr(5, 21, "Rum + Dew");
        u8g2.drawStr(5, 32, "Rum + Sprite");
        u8g2.drawStr(5, 43, "Gin + Coke");
        u8g2.drawStr(5, 54, "Gin + Lemonade");
        u8g2.drawStr(5, 64, "Gin + Dew");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Coke");
        u8g2.drawStr(5, 21, "Vodka + Lemonade");
        u8g2.drawStr(5, 32, "Vodka + Dew");
        u8g2.drawStr(5, 43, "Vodka + Sprite");
        u8g2.drawStr(5, 54, "Rum + Coke");
        u8g2.drawStr(5, 64, "Rum + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }
    break;

    case COCKTAIL_STATE_RUM_DEW:  //rum + mountain dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Dew");
        u8g2.drawStr(5, 21, "Rum + Sprite");
        u8g2.drawStr(5, 32, "Gin + Coke");
        u8g2.drawStr(5, 43, "Gin + Lemonade");
        u8g2.drawStr(5, 54, "Gin + Dew");
        u8g2.drawStr(5, 64, "Gin + Sprite");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Lemonade");
        u8g2.drawStr(5, 21, "Vodka + Dew");
        u8g2.drawStr(5, 32, "Vodka + Sprite");
        u8g2.drawStr(5, 43, "Rum + Coke");
        u8g2.drawStr(5, 54, "Rum + Lemonade");
        u8g2.drawStr(5, 64, "Rum + Dew");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_RUM_SPRITE:  //rum + sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Sprite");
        u8g2.drawStr(5, 21, "Gin + Coke");
        u8g2.drawStr(5, 32, "Gin + Lemonade");
        u8g2.drawStr(5, 43, "Gin + Dew");
        u8g2.drawStr(5, 54, "Gin + Sprite");
        u8g2.drawStr(5, 64, "Tequila + Coke");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Dew");
        u8g2.drawStr(5, 21, "Vodka + Sprite");
        u8g2.drawStr(5, 32, "Rum + Coke");
        u8g2.drawStr(5, 43, "Rum + Lemonade");
        u8g2.drawStr(5, 54, "Rum + Dew");
        u8g2.drawStr(5, 64, "Rum + Sprite");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_GIN_COKE: // gin + coke
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Coke");
        u8g2.drawStr(5, 21, "Gin + Lemonade");
        u8g2.drawStr(5, 32, "Gin + Dew");
        u8g2.drawStr(5, 43, "Gin + Sprite");
        u8g2.drawStr(5, 54, "Tequila + Coke");
        u8g2.drawStr(5, 64, "Tequila + Lemonade");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Vodka + Sprite");
        u8g2.drawStr(5, 21, "Rum + Coke");
        u8g2.drawStr(5, 32, "Rum + Lemonade");
        u8g2.drawStr(5, 43, "Rum + Dew");
        u8g2.drawStr(5, 54, "Rum + Sprite");
        u8g2.drawStr(5, 64, "Gin + Coke");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_GIN_LEMONADE: // gin + lemonade
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Lemonade");
        u8g2.drawStr(5, 21, "Gin + Dew");
        u8g2.drawStr(5, 32, "Gin + Sprite");
        u8g2.drawStr(5, 43, "Tequila + Coke");
        u8g2.drawStr(5, 54, "Tequila + Lemonade");
        u8g2.drawStr(5, 64, "Tequila + Dew");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Coke");
        u8g2.drawStr(5, 21, "Rum + Lemonade");
        u8g2.drawStr(5, 32, "Rum + Dew");
        u8g2.drawStr(5, 43, "Rum + Sprite");
        u8g2.drawStr(5, 54, "Gin + Coke");
        u8g2.drawStr(5, 64, "Gin + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_GIN_DEW: // gin + dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Dew");
        u8g2.drawStr(5, 21, "Gin + Sprite");
        u8g2.drawStr(5, 32, "Tequila + Coke");
        u8g2.drawStr(5, 43, "Tequila + Lemonade");
        u8g2.drawStr(5, 54, "Tequila + Dew");
        u8g2.drawStr(5, 64, "Tequila + Sprite");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Lemonade");
        u8g2.drawStr(5, 21, "Rum + Dew");
        u8g2.drawStr(5, 32, "Rum + Sprite");
        u8g2.drawStr(5, 43, "Gin + Coke");
        u8g2.drawStr(5, 54, "Gin + Lemonade");
        u8g2.drawStr(5, 64, "Gin + Dew");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_GIN_SPRITE: // gin + sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Dew");
        u8g2.drawStr(5, 21, "Rum + Sprite");
        u8g2.drawStr(5, 32, "Gin + Coke");
        u8g2.drawStr(5, 43, "Gin + Lemonade");
        u8g2.drawStr(5, 54, "Gin + Dew");
        u8g2.drawStr(5, 64, "Gin + Sprite");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_TEQUILA_COKE: // tequila + coke
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 11, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Rum + Sprite");
        u8g2.drawStr(5, 21, "Gin + Coke");
        u8g2.drawStr(5, 32, "Gin + Lemonade");
        u8g2.drawStr(5, 43, "Gin + Dew");
        u8g2.drawStr(5, 54, "Gin + Sprite");
        u8g2.drawStr(5, 64, "Tequila + Coke");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_TEQUILA_LEMONADE: // tequila + lemonade
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 22, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Coke");
        u8g2.drawStr(5, 21, "Gin + Lemonade");
        u8g2.drawStr(5, 32, "Gin + Dew");
        u8g2.drawStr(5, 43, "Gin + Sprite");
        u8g2.drawStr(5, 54, "Tequila + Coke");
        u8g2.drawStr(5, 64, "Tequila + Lemonade");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_TEQUILA_DEW: // tequila + dew
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 33, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Lemonade");
        u8g2.drawStr(5, 21, "Gin + Dew");
        u8g2.drawStr(5, 32, "Gin + Sprite");
        u8g2.drawStr(5, 43, "Tequila + Coke");
        u8g2.drawStr(5, 54, "Tequila + Lemonade");
        u8g2.drawStr(5, 64, "Tequila + Dew");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_TEQUILA_SPRITE: // tequila + Sprite
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 44, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Dew");
        u8g2.drawStr(5, 21, "Gin + Sprite");
        u8g2.drawStr(5, 32, "Tequila + Coke");
        u8g2.drawStr(5, 43, "Tequila + Lemonade");
        u8g2.drawStr(5, 54, "Tequila + Dew");
        u8g2.drawStr(5, 64, "Tequila + Sprite");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;

    case COCKTAIL_STATE_LONG_ISLAND: // long island
      if (upFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        upFlag = 0;
      }//end of if
      else if (downFlag) {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin + Sprite");
        u8g2.drawStr(5, 21, "Tequila + Coke");
        u8g2.drawStr(5, 32, "Tequila + Lemonade");
        u8g2.drawStr(5, 43, "Tequila + Dew");
        u8g2.drawStr(5, 54, "Tequila + Sprite");
        u8g2.drawStr(5, 64, "Long Island (;");
        u8g2.sendBuffer();
        downFlag = 0;
      }//end of else if
    break;
  }//end of switch
  u8g2.sendBuffer();
}//end of cockTailMenuPrint


/*
 * @function:   shotMenuPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
                box around the text (indicating possible selections)
 * @return:     void
 * @note:       The shot menu is printed in this function
 * 
 */
void shotMenuPrint(shot_menu_state_e shot_state) 
{
  u8g2.setFont(u8g2_font_t0_11b_tf);
  switch (shot_state) 
  {
    case SHOT_STATE_GIN://Gin
      u8g2.clearBuffer();
      u8g2.drawBox(0, 0, 123, 12);
      u8g2.drawStr(5, 10, "Gin");
      u8g2.drawStr(5, 21, "Spiced Rum");
      u8g2.drawStr(5, 32, "Tequila");
      u8g2.drawStr(5, 43, "Vodka");
      u8g2.drawStr(5, 54, "Whiskey");
      u8g2.drawStr(5, 64, "White Rum");
      reset_button_flags();
    break;
    case SHOT_STATE_RUM://Spiced Rum
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 11, 123, 12);
        u8g2.drawStr(5, 10, "Gin");
        u8g2.drawStr(5, 21, "Spiced Rum");
        u8g2.drawStr(5, 32, "Tequila");
        u8g2.drawStr(5, 43, "Vodka");
        u8g2.drawStr(5, 54, "Whiskey");
        u8g2.drawStr(5, 64, "White Rum");
        reset_button_flags();
      }//end of if
    break;
    case SHOT_STATE_TEQUILA:
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 22, 123, 12);
        u8g2.drawStr(5, 10, "Gin");
        u8g2.drawStr(5, 21, "Spiced Rum");
        u8g2.drawStr(5, 32, "Tequila");
        u8g2.drawStr(5, 43, "Vodka");
        u8g2.drawStr(5, 54, "Whiskey");
        u8g2.drawStr(5, 64, "White Rum");
        reset_button_flags();
      }//end of if
    break;
    case SHOT_STATE_VODKA:
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 33, 123, 12);
        u8g2.drawStr(5, 10, "Gin");
        u8g2.drawStr(5, 21, "Spiced Rum");
        u8g2.drawStr(5, 32, "Tequila");
        u8g2.drawStr(5, 43, "Vodka");
        u8g2.drawStr(5, 54, "Whiskey");
        u8g2.drawStr(5, 64, "White Rum");
        reset_button_flags();
      }//end of if
    break;
    case SHOT_STATE_WHISKEY:
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 44, 123, 12);
        u8g2.drawStr(5, 10, "Gin");
        u8g2.drawStr(5, 21, "Spiced Rum");
        u8g2.drawStr(5, 32, "Tequila");
        u8g2.drawStr(5, 43, "Vodka");
        u8g2.drawStr(5, 54, "Whiskey");
        u8g2.drawStr(5, 64, "White Rum");
        reset_button_flags();
      }//end of if
    break;
    case SHOT_STATE_WHITE_RUM:
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 55, 123, 12);
        u8g2.drawStr(5, 10, "Gin");
        u8g2.drawStr(5, 21, "Spiced Rum");
        u8g2.drawStr(5, 32, "Tequila");
        u8g2.drawStr(5, 43, "Vodka");
        u8g2.drawStr(5, 54, "Whiskey");
        u8g2.drawStr(5, 64, "White Rum");
        reset_button_flags();
      }//end of if
    break;
  }//end of switch
  u8g2.sendBuffer();
}// end of shotMenuPrint

/*
 * @function:   popMenuPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
 *              box around the text (indicating possible selections)
 * @return:     void
 * 
 * @note:       The pop menu is printed in this funciton
 */ 
void popMenuPrint(pop_menu_state_e pop_state) 
{
  u8g2.setFont(u8g2_font_t0_14b_tf);
  switch (pop_state) 
  {
    case POP_STATE_SPRITE://Sprite
      u8g2.clearBuffer();
      u8g2.drawBox(0, 0, 123, 16);
      u8g2.drawStr(5, 14, "Sprite");
      u8g2.drawStr(5, 28, "Coke");
      u8g2.drawStr(5, 42, "Lemonade");
      u8g2.drawStr(5, 56, "Mountain Dew");
      reset_button_flags();
    break;
    case POP_STATE_COKE://Coke
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 16, 123, 16);
        u8g2.drawStr(5, 14, "Sprite");
        u8g2.drawStr(5, 28, "Coke");
        u8g2.drawStr(5, 42, "Lemonade");
        u8g2.drawStr(5, 56, "Mountain Dew");
        reset_button_flags();
      }//end of if
    break;
    case POP_STATE_LEMONADE://Lemonade
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 30, 123, 16);
        u8g2.drawStr(5, 14, "Sprite");
        u8g2.drawStr(5, 28, "Coke");
        u8g2.drawStr(5, 42, "Lemonade");
        u8g2.drawStr(5, 56, "Mountain Dew");
        reset_button_flags();
      }//end of if
    break;
    case POP_STATE_DEW://Mountain Dew
      if (upFlag || downFlag) 
      {
        u8g2.clearBuffer();
        u8g2.drawBox(0, 45, 123, 16);
        u8g2.drawStr(5, 14, "Sprite");
        u8g2.drawStr(5, 28, "Coke");
        u8g2.drawStr(5, 42, "Lemonade");
        u8g2.drawStr(5, 56, "Mountain Dew");
        reset_button_flags();
      }//end of if
    break;
  }//end of switch
  u8g2.sendBuffer();
}//end of popMenuPrint

/*
 * @function:   mainMenuPrint( int shadedRegion )
 * @param[in]:  int shadedRegion - controls the location of the shaded
 *              box around the text (indicating possible selections)
 * @return:     void
 * 
 * @note:       The main menu is printed in this function.
 *              Cocktails, Shots, Pop
 * 
 */
void mainMenuPrint(menu_states_e menu_state) 
{
  u8g2.setFont(u8g2_font_t0_14b_tf);
  switch (menu_state) 
  {
    case 0:
      u8g2.clearBuffer();
      u8g2.drawBox(0, 2, 110, 20);
      u8g2.drawStr(5, 18, "Cocktails");
      u8g2.drawStr(5, 36, "Shots");
      u8g2.drawStr(5, 54, "Pop");
    break;
    case 1:
      u8g2.clearBuffer();
      u8g2.drawBox(0, 20, 110, 20);
      u8g2.drawStr(5, 18, "Cocktails");
      u8g2.drawStr(5, 36, "Shots");
      u8g2.drawStr(5, 54, "Pop");
    break;
    case 2:
      u8g2.clearBuffer();
      u8g2.drawBox(0, 40, 110, 20);
      u8g2.drawStr(5, 18, "Cocktails");
      u8g2.drawStr(5, 36, "Shots");
      u8g2.drawStr(5, 54, "Pop");
    break;
  }//end of switch
  u8g2.sendBuffer();
}


void reset_button_flags()
{
  upFlag = 0;
  downFlag = 0;
}


void reset_menu_level()
{
  menu_level = MAIN_MENU_LVL_1;
}


void reset_strength_states()
{
  cocktail_strength_state = COCKTAIL_STRENGTH_STATE_WEAK;
  shot_strength_state = SHOT_STRENGTH_STATE_SINGLE;
  pop_strength_state = POP_STRENGTH_STATE_SMALL;
}


void reset_submenu_states()
{
  cocktail_state = COCKTAIL_STATE_WHISKEY_COKE;
  shot_state = SHOT_STATE_GIN;
  pop_state = POP_STATE_SPRITE; 
}


void reset_mainmenu_state()
{
  menu_state = MENU_STATE_COCKTAILS;
}


#if defined(PRINT_INTRO)
/*
 * @function:  print_intro( void )
 * @param[in]: void
 * @return:    void
 * @note:      The introduction is printed on startup.
 *             All the images & text prior to the main menu print
 *             is included in this function.
 * @note:      All of the bmps were created uniquely in photoshop.
 * 
 */
void print_intro()
{
  u8g2.clearBuffer();
  static int radius = 2;
  u8g2.setDrawColor(1);
  for (int y = 0; y < 80; y++) 
  {
    u8g2.setFont(u8g2_font_timB08_tf);
    u8g2.drawStr(42, 18, "Presenting");
    u8g2.setFont(u8g2_font_crox2c_tf);
    u8g2.drawStr(28, 35, "Cocktail");
    u8g2.drawStr(18, 45, "Automation");
    u8g2.setFont(u8g2_font_timB08_tf);
    for (int z = 0; z < 60; z++) 
    {
      if(!(z % 2))
      {
        seeds[z] = seeds[z] + heading[z];
        if (seeds[z] < 0) 
        {
          seeds[z] = 0;
          heading[z] = heading[z] * -1;
        }//end of if
        else if (seeds[z] > 123) 
        {
          seeds[z] = 123;
          heading[z] = heading[z] * -1;
        }//end of else if
      }//end of if
      else 
      {
        seeds[z] = seeds[z] + heading[z];
        if (seeds[z] < 0) 
        {
          seeds[z] = 0;
          heading[z] = heading[z] * -1;
        }//end of if
        else if (seeds[z] > 63) 
        {
          seeds[z] = 63;
          heading[z] = heading[z] * -1;
        }//end of else if
        u8g2.drawDisc(seeds[z - 1], seeds[z], radius, U8G2_DRAW_ALL);  // draw the damn balls
      }//end of else
    }//end of for
    u8g2.sendBuffer();
    u8g2.clearBuffer();
  }// end of for
  u8g2.clearBuffer();
  u8g2.drawXBM(5, 0, 40, 64, sam_bmp);
  u8g2.setFont(u8g2_font_timB08_tf);
  u8g2.drawStr(62, 10, "Created By:");
  u8g2.drawXBM(54, 44, 63, 20, signature_bmp);
  u8g2.drawXBM(48, 12, 32, 32, cup_bmp);
  u8g2.drawXBM(80, 12, 13, 29, bottle_bmp);
  u8g2.drawXBM(96, 12, 32, 32, martini_glass_bmp);
  u8g2.sendBuffer();
  delay(3000);  // delay a bit to show the bit maps
  u8g2.setFont(u8g2_font_timB08_tf);
  // move the bitmaps!
  for (int j = 0; j < 192; j += 5) 
  {
    u8g2.drawXBM(-60 + j, -10, 64, 42, plane_bmp);
    u8g2.drawXBM(85, 32, 41, 29, little_arduino_bmp);
    u8g2.drawXBM(10, 43, 30, 19, finger_point_bmp);
    u8g2.drawStr(0, 42, "Powered By");
    if (j < 60)
        u8g2.drawStr(40 + j - 1, 52, ".");  // did you notice the finger shoots a bullet?!
    else if (j > 120)
      u8g2.drawXBM(58, 15 + (j - 98) / 3, 20, 20, bombski_bmp);
    u8g2.sendBuffer();
    u8g2.clearBuffer();
  }// end of for
  u8g2.setFont(u8g2_font_synchronizer_nbp_tf);
  u8g2.drawXBM(0, 0, 32, 64, destruction_bmp);
  u8g2.drawXBM(32, 0, 64, 64, explosion_bmp);
  u8g2.drawXBM(96, 0, 32, 64, destruction2_bmp);
  u8g2.sendBuffer();
  delay(3000);
  for (int j = 0; j < 64; j += 10) 
  {
    u8g2.drawXBM(0, -64 + j, 128, 64, ground_zero_bmp);
    u8g2.sendBuffer();
    u8g2.clearBuffer();
  }// end of for
  for (int j = 0; j < 15; j += 5) 
  {
    u8g2.drawXBM(0, 0, 128, 64, ground_zero_bmp);
    u8g2.drawStr(18, j, "The");
    u8g2.sendBuffer();
    if (j != 10)
      u8g2.clearBuffer();
  }// end of for
  u8g2.sendBuffer();
  delay(3000);
}// end of print_intro()
#endif
