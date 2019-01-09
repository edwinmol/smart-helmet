/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <NeoPixelBus.h>

//#include <Adafruit_NeoPixel.h> --> This lib does not work, problems with timing on ESP32

const uint16_t PixelCount = 22; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 15;  // make sure to set this to the correct pin, ignored for Esp8266
int delayval = 75; // delay

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
      for (int t = 0; t < 5; t++) {

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

  MyCallbacks *callbacks = new MyCallbacks();
  pCharacteristic->setCallbacks(callbacks);

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  callbacks->clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
