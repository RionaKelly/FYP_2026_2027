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
#define RED_LED_PIN 12
#define GREEN_LED_PIN 11
#define VMOTOR_PIN 10

int draw_count = 0;
int x = 0;
int y = 0;
int direction = true;
int sprite_spot = 0;
bool red_led_on = false;
bool green_led_on = false;
bool unpressed = true;
uint32_t bg_color = TFT_WHITE;
int center_x = (TFT_HEIGHT/2) - (logoHeight/2);
int sprite_frame = -1;
int green_led_time = 0;

// Handles the serial inputs every loop if data has been recieved, doing something specific depending on what was read
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
  else {
    tft.fillScreen(TFT_RED);
    bg_color = TFT_RED;
    Serial.println("TO_GODOTSignal not recognised, Red Screen");
  }
  digitalWrite(GREEN_LED_PIN, HIGH);
}

// Sets up the sprites once so that they can be drawn to the screen
void setupSprite() {
  //spr.setColorDepth(4); //Set or get the colour depth to 1, 4, 8 or 16 bits
  spr.createSprite(TFT_HEIGHT, TFT_WIDTH); // Create a sprite of defined size
  //spr.pushImage(0, 0, logoWidth, logoHeight, (uint16_t *)bunny);// push the image, only need to do this once
}

// Flips a given Bitmap
const unsigned char* flipBitmap(const unsigned char* bitmap) {
  for (int i = 0; i < 3200; i++) {
    x = i + 8;
    Serial.println(String(bitmap[i]));
  }
  //Serial.println(bitmap[105000]);

  unsigned char new_bitmap [] {
  };
  return new_bitmap;
}

// Draws the sprite as the correct frame when called
void drawSprite() {
  //spr.drawBitmap(0, 0, bunny_mask, logoWidth, logoHeight, TFT_BLUE);
  spr.fillSprite(bg_color);//(bg_color);
  if (direction) {
    sprite_frame++;
  }
  else {
    sprite_frame--;
  }

  if (sprite_frame > 7 && direction) {
    if (sprite_spot > 3) {
      if (sprite_frame > 14) {
        direction = false;
      }
    }
    else{
      sprite_frame = 0; // Sets the frame to 0 once it reaches the right end (8 frames total)
      sprite_spot++;
    }
  }
  else if (sprite_frame < 0 && !direction) {
    if (sprite_spot < 1) {
      if (sprite_frame < -7) {
        direction = true;
      }
    }
    else {
      sprite_frame = 7; // Sets the frame to 7 once it reaches the left end (8 frames total)
      sprite_spot--;
    }
  } 

  int chosen_sprite = 0; // 0 = base, 1 = squash, 2 = stretch, 3 = smile, 4 = sit

  switch (sprite_frame) {
    case 0:
      chosen_sprite = 0;
      y = (TFT_WIDTH - logoHeight);
      if (!direction) {
        digitalWrite(VMOTOR_PIN, LOW);
      }
      break;
    case 1:
      chosen_sprite = 1;
      y = (TFT_WIDTH - logoHeight);
      if (!direction) {
        digitalWrite(VMOTOR_PIN, HIGH);
      }
      break;
    case 2:
      chosen_sprite = 0;
      y = (TFT_WIDTH - logoHeight);
      break;
    case 3:
      chosen_sprite = 2;
      y = (TFT_WIDTH - logoHeight) - 10;
      if (direction) {
        x = x + 10;
      }
      else {
        x = x - 10;
      }
      break;
    case 4:
      chosen_sprite = 3;
      y = (TFT_WIDTH - logoHeight) - 20;
      if (direction) {
        x = x + 10;
      }
      else {
        x = x - 10;
      }
      break;
    case 5:
      chosen_sprite = 2;
      y = (TFT_WIDTH - logoHeight);
      if (direction) {
        x = x + 10;
      }
      else {
        x = x - 10;
      }
      break;
    case 6:
      chosen_sprite = 1;
      y = (TFT_WIDTH - logoHeight);
      break;
    case 7:
      chosen_sprite = 0;
      y = (TFT_WIDTH - logoHeight);
      if (direction) {
        digitalWrite(VMOTOR_PIN, HIGH);
      }
      break;
    default: 
      chosen_sprite = 4;
      y = (TFT_WIDTH - logoHeight);
      if (direction) {
        digitalWrite(VMOTOR_PIN, LOW);
      }
      break;
  }

  // Draw bitmap with top left corner at x,y with foreground and background colors
  //             x  y  xbm   xbm width  xbm height  fg color   bg color
  spr.drawBitmap(x, y, sprite_array[chosen_sprite], logoWidth, logoHeight, TFT_PINK);
  spr.drawBitmap(x, y, sprite_array[chosen_sprite + 5], logoWidth, logoHeight, TFT_BLACK);
  if (!direction) {
    spr.fillRect(x, (TFT_WIDTH - logoHeight), 160, 20 , TFT_RED);
  }
  spr.setRotation(2);
  spr.pushSprite(0, 0);
  Serial.println("Sprite drawn, frame: " + String(sprite_frame) + ", moving right: " + String(direction) + ", spot: " + String(sprite_spot));
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Bitmap Display Test");

  tft.begin();               // Initialise the display
  tft.setRotation(1);
  pinMode(TFT_BL, OUTPUT); // sets up the backlight pin for setting
	analogWrite(TFT_BL, 100); // sets the brightness of the backlight to the brightness variable (0-255)
  tft.fillScreen(TFT_BLUE); // Screen fill
  Serial.println("TFT Setup Complete");

  setupSprite();

  //Set_Color(0, 140, 225); // sets the outside RGB lights using a premade .h and .cpp file, GRB for some reason

	pinMode(BUTTON_PIN, INPUT_PULLUP); // sets the button to be able to be read
	pinMode(RED_LED_PIN, OUTPUT); // sets the led to output
  pinMode(GREEN_LED_PIN, OUTPUT); // sets the led to output
  pinMode(VMOTOR_PIN, OUTPUT); // sets the vibration motor to output

  drawSprite();

  delay(1000);
  Serial.println("Setup Finished, Loop Starting...");
}

void loop()
{
  if (Serial.available() > 0) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    green_led_on = true;
    handleSerial();
    green_led_time = millis();
  }
  else if (green_led_on && millis() > green_led_time + 1000) {
    digitalWrite(GREEN_LED_PIN, LOW);
    green_led_on = false;
  }

  // Button Checker 
  int buttonState = digitalRead(BUTTON_PIN);
	if (buttonState == LOW) {
    if (unpressed == true) { // anything in this loop happens when the button is pressed/held, until release
      Serial.println("Button Pressed");
      unpressed = false;
      analogWrite(TFT_BL, 160); // sets the brightness of the backlight to the brightness variable (0-255)
      digitalWrite(VMOTOR_PIN, HIGH);

      if (red_led_on) { // Check is the led is already on to only toggle it on or off when pressed, no matter how long it is held
        digitalWrite(RED_LED_PIN, LOW);
        red_led_on = false;
        Serial.println("LED Off :(");
      } else {
        digitalWrite(RED_LED_PIN, HIGH);
        red_led_on = true;
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
    digitalWrite(VMOTOR_PIN, LOW);
	}

  // Draws the Pet after set amount of loops
  if (draw_count >= 1) {
    drawSprite();
    draw_count = 0;
  }
  else {
    draw_count = draw_count + 1;
  }

  delay(50);
}
