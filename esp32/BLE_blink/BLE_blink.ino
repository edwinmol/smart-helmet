/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the ESP32 is connected to the NeoPixels?
#define PIN            15
#define NUMPIXELS      22

int delayval = 75; // delay

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
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
        }

        Serial.println();
        Serial.println("*********");
      }
    }

    void left() {

      clear();
      for (int t = 0; t < 5; t++) {

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

      clear();
      for (int t = 0; t < 5; t++) {

        for (int i = 10; i >= 0; i--) {

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

      for (int i = 0; i < NUMPIXELS ; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      }
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);   
      pixels.show();
    }

    void light() {

      for (int i = 0; i < NUMPIXELS ; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(20, 0, 0));
      }
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);   
      pixels.show();
    }

    void clear() {      
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));                
      }
      pixels.show();
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));   
      delay(delayval);   
      pixels.show();
      
    }
};

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
