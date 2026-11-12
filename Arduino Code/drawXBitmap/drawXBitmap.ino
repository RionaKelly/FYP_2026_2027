// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/04/18

#include "xbm.h"             // Sketch tab header for xbm images
#include <RGB_lamp.h>        // File for using RGB lights on device
#include <TFT_eSPI.h>        // Hardware-specific library
//#include <SPI.h>
//#include <SD.h>              // Library for interacting with SD card

TFT_eSPI tft = TFT_eSPI();   // Declare object "tft"
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

//const int chipSelect = 16;   // SD Card Setup
//File myFile;

#define BUTTON_PIN 13
#define LED_PIN 12
#define VMOTOR_PIN 11

int draw_count = 0;
int x = 26;
int y = 90;
bool led_on = false;
bool unpressed = true;
uint32_t bg_color = TFT_WHITE;

void drawPet() {
  // Erase old one by drawing over with background colour
  //tft.drawBitmap(x, y, bunny_mask, logoWidth, logoHeight, bg_color);

  // x and y coordinates
  x = (TFT_WIDTH/2) - (logoWidth/2);
  if (y == 90) {
    y = (TFT_HEIGHT/2) - (logoHeight/2) + 20;
  }
  else {
    y = (TFT_HEIGHT/2) - (logoHeight/2);
  }
  

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //             x  y  xbm   xbm width  xbm height  fg color   bg color
  //tft.drawBitmap(x, y, bunny_in, logoWidth, logoHeight, TFT_PINK);
  //tft.drawBitmap(x, y, bunny_out, logoWidth, logoHeight, TFT_BLACK);

  Serial.println("Pet Drawn at " + String(x) + ", " + String(y));
}

void handleSerial() {
  String incoming = Serial.readStringUntil('\n');
  incoming.trim();

  if (incoming == "Hello") {
    tft.fillScreen(TFT_GREEN);
    bg_color = TFT_GREEN;
    Serial.println("TO_GODOT'Hello' recieved, Green Screen");
  }
  else if (incoming == "0") {
    tft.fillScreen(TFT_YELLOW);
    bg_color = TFT_YELLOW;
    Serial.println("TO_GODOT0 recieved, Yellow Screen");
  }
  else if (incoming == "Draw") {
    drawPet();
    Serial.println("TO_GODOT'Draw' recieved, Drawing Pet");
  }
  else {
    tft.fillScreen(TFT_RED);
    bg_color = TFT_RED;
    Serial.println("TO_GODOTSignal not recognised, Red Screen");
  }
}

void setupSprite() {
  spr.setColorDepth(4); //Set or get the colour depth to 1, 4, 8 or 16 bits
  spr.createSprite(80, 80); // Create a sprite of defined size
  spr.pushImage(0, 0, logoWidth, logoHeight, (uint16_t *)bunny);// push the image, only need to do this once
}

void drawSprite() {
  spr.pushSprite(TFT_WIDTH / 2 - logoWidth / 2, TFT_HEIGHT / 2 - logoHeight / 2);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Bitmap Display Test");

  tft.begin();               // Initialise the display
  tft.setRotation(2);
  pinMode(TFT_BL, OUTPUT); // sets up the backlight pin for setting
	analogWrite(TFT_BL, 100); // sets the brightness of the backlight to the brightness variable (0-255)
  tft.fillScreen(bg_color); // Screen fill
  Serial.println("TFT Setup Complete");

  setupSprite();

  //Set_Color(0, 140, 225); // sets the outside RGB lights using a premade .h and .cpp file, GRB for some reason

	pinMode(BUTTON_PIN, INPUT_PULLUP); // sets the button to be able to be read
	pinMode(LED_PIN, OUTPUT); // sets the led to output
  pinMode(VMOTOR_PIN, OUTPUT); // sets the vibration motor to output
  digitalWrite(VMOTOR_PIN, HIGH);

  //drawPet();
  drawSprite();

  delay(1000);
  Serial.println("Setup Finished, Loop Starting...");
}

void loop()
{
  if (Serial.available() > 0) {
    handleSerial();
  }

  // Button Checker 
  int buttonState = digitalRead(BUTTON_PIN);
	if (buttonState == LOW) {
    if (unpressed == true) { // anything in this loop happens when the button is pressed/held, until release
      Serial.println("Button Pressed");
      unpressed = false;
      analogWrite(TFT_BL, 160); // sets the brightness of the backlight to the brightness variable (0-255)

      if (led_on) { // Check is the led is already on to only toggle it on or off when pressed, no matter how long it is held
        digitalWrite(LED_PIN, LOW);
        led_on = false;
        Serial.println("LED Off :(");
      } else {
        digitalWrite(LED_PIN, HIGH);
        led_on = true;
        Serial.println("LED On!");
      }
    }
		Set_Color(0, 200, 140);
		//digitalWrite(VMOTOR_PIN, HIGH);
	} else {
    unpressed = true;
		Set_Color(0, 0, 0);
    //digitalWrite(VMOTOR_PIN, LOW);
    analogWrite(TFT_BL, 100); // sets the brightness of the backlight to the brightness variable (0-255)
	}

  // Draws the Pet after set amount of loops
  if (draw_count >= 50) {
    //drawPet();
    drawSprite();
    draw_count = 0;
  }
  else {
    draw_count = draw_count + 1;
  }

  delay(50);
}
