#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <NeoPixelBus.h>

//#include <Adafruit_NeoPixel.h> --> This lib does not work, problems with timing on ESP32

const uint16_t PixelCount = 22; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 15;  // make sure to set this to the correct pin, ignored for Esp8266
int delayval = 75; // delay
int shortdelayval = 50; // delay

#define colorSaturation 255

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor lightred(20, 0, 0);
RgbColor orange(255, 140, 0);
RgbColor white(colorSaturation);
RgbColor black(0);

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "051cf1f1-3f95-4068-8150-80ffbc111bd4"
#define CHARACTERISTIC_UUID "e2532f4c-b771-4db3-a5cf-1eead92d026f"


class MyCallbacks: public BLECharacteristicCallbacks {
    
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        //for (int i = 0; i < value.length(); i++)
        //  Serial.print(value[i]);
        if (value == "left") {          
          left();
        } else if (value == "right") {
          right();
        } else if (value == "stop") {
          stop();
        } else if (value == "clear") {
          clear();
        } else if (value == "light") {
          light();
        } else if (value == "nr") {
          knightRider();
        }
      }
    }

    void left() {

      clear();
      for (int t = 0; t < 7; t++) {

        for (int i = 11; i < PixelCount; i++) {

          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          strip.SetPixelColor(i, orange); // orange.
          strip.Show();
          delay(delayval); // Delay for a period of time (in milliseconds).

        }
        clear();
        delay(delayval);
      }
    }

    void right() {

      clear();
      for (int t = 0; t < 7; t++) {

        for (int i = 10; i >= 0; i--) {

          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
          strip.SetPixelColor(i, orange); // orange.
          strip.Show();
          delay(delayval); // Delay for a period of time (in milliseconds).
        }
        clear();
        delay(delayval);
      }
    }

    void stop() {

      for (int i = 0; i < PixelCount ; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        strip.SetPixelColor(i, red);
      }
      strip.Show();
    }

    void light() {

      for (int i = 0; i < PixelCount ; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        strip.SetPixelColor(i, lightred);
      }
      strip.Show();
    }

    void knightRider() {
      clear();
      for (int t = 0; t < 10; t++)  {
        for (int i = 10; i >= 0; i--) {
          strip.SetPixelColor(i, red); // orange.
          strip.SetPixelColor(i+1, red); // orange.
          strip.SetPixelColor(i+2, red); // orange.
          strip.Show();
          delay(shortdelayval); // Delay for a period of time (in milliseconds).
          clear();
        }
        for (int i = 1; i <= 19; i++) {
          strip.SetPixelColor(i, red); // orange.
          strip.SetPixelColor(i+1, red); // orange.
          strip.SetPixelColor(i+2, red); // orange.
          strip.Show();
          delay(shortdelayval); // Delay for a period of time (in milliseconds).
          clear();
        }
        for (int i = 18; i >= 11; i--) {
          strip.SetPixelColor(i, red); // orange.
          strip.SetPixelColor(i+1, red); // orange.
          strip.SetPixelColor(i+2, red); // orange.
          strip.Show();
          delay(shortdelayval); // Delay for a period of time (in milliseconds).
          clear();
        }
      }
    }
    
    public : void clear() {
      for (int i = 0; i < PixelCount; i++) {
        strip.SetPixelColor(i, black);
      }
      strip.Show();
    }

};

void setup() {
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  BLEDevice::init("Blinker");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  MyCallbacks *callbacks = new MyCallbacks();
  pCharacteristic->setCallbacks(callbacks);

  pCharacteristic->setValue("light");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  callbacks->clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
