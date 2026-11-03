// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/04/18

#include "xbm.h"             // Sketch tab header for xbm images
#include <RGB_lamp.h>
#include <TFT_eSPI.h>        // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();   // Invoke library

int draw_count = 5;
int x = 0;
int y = 0;
bool led_on = false;
bool unpressed = true;

void drawPet() {
  // Erase old one by drawing over with background colour
  tft.drawBitmap(x, y, bunny_mask, logoWidth, logoHeight, TFT_WHITE);

  // Random x and y coordinates
  x = (TFT_WIDTH/2) - (logoWidth/2);//random(tft.width()  - logoWidth);
  y = (TFT_HEIGHT/2) - (logoHeight/2);//random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //             x  y  xbm   xbm width  xbm height  fg color   bg color
  tft.drawBitmap(x, y, bunny_in, logoWidth, logoHeight, TFT_PINK);
  tft.drawBitmap(x, y, bunny_out, logoWidth, logoHeight, TFT_BLACK);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Bitmap Display Test");

  tft.begin();               // Initialise the display
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE); // Screen fill

  pinMode(TFT_BL, OUTPUT); // sets up the backlight pin for setting
	analogWrite(TFT_BL, 120); // sets the brightness of the backlight to the brightness variable (0-255)

  //Set_Color(0, 140, 225); // sets the outside RGB lights using a premade .h and .cpp file, GRB for some reason

	pinMode(13, INPUT_PULLUP); // sets the button to be able to be read
	pinMode(12, OUTPUT); // sets the led to output
}

void loop()
{
  int buttonState = digitalRead(13);
	if (buttonState == LOW) {
    if (unpressed == true) {
      unpressed = false;
      analogWrite(TFT_BL, 30); // sets the brightness of the backlight to the brightness variable (0-255)
      if (led_on) {
        digitalWrite(12, LOW);
        led_on = false;
      } else {
        digitalWrite(12, HIGH);
        led_on = true;
      }
    }
		Set_Color(0, 200, 140);
		//digitalWrite(12, HIGH); // if button is pressed turn on led
	} else {
    unpressed = true;
		Set_Color(0, 0, 0);
		//digitalWrite(12, LOW); // if button is not pressed turn off led
    analogWrite(TFT_BL, 120); // sets the brightness of the backlight to the brightness variable (0-255)
	}

  if (draw_count >= 5) {
    drawPet();
    draw_count = 0;
  }
  else {
    draw_count = draw_count + 1;
  }

  delay(100);
}
