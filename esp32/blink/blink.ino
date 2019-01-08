// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the ESP32 is connected to the NeoPixels?
#define PIN            15

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      22

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 75; // delay 

void setup() {

  pixels.begin(); // This initializes the NeoPixel library.
  clear();
}

void loop() {

  right();
  left();
  stop();

}

void clear() {

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));        
  }
  pixels.show();
}


void left() {

  for (int t = 0; t < 2; t++) {
    
    for (int i = 11; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 140, 0)); // orange.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).

    }     
    clear();   
    delay(delayval);
  }
}

void right() {

  for (int t = 0; t < 2; t++) {
    
    for (int i = 10; i > 0; i--) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 140, 0)); // orange.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).

    }     
    clear();   
    delay(delayval);
  }
}

void stop() {

  for (int t = 0; t < 4; t++) {
    
    for (int i = 0; i < (NUMPIXELS/2) ; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i+10, pixels.Color(255, 0, 0)); 
      pixels.setPixelColor(11-i, pixels.Color(255, 0, 0));

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(30); // Delay for a period of time (in milliseconds).

    }     
    clear();   
    delay(30);
  }
}
