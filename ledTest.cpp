#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

const int LED_PIN = 26;

// Random UUIDs for our test service
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define LED_CHARACTERISTIC  "abcdefab-1234-5678-1234-abcdefabcdef"

class LedCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) override {
    String value = characteristic->getValue();

    if (value.length() == 0) {
      return;
    }

    Serial.print("Received: ");
    Serial.println(value);

    if (value == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");
    }

    if (value == "0") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }
  }
};

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Name that will appear when scanning
  BLEDevice::init("LEDTest");

  BLEServer *server = BLEDevice::createServer();

  BLEService *service =
      server->createService(SERVICE_UUID);

  BLECharacteristic *ledCharacteristic =
      service->createCharacteristic(
        LED_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_READ
      );

  ledCharacteristic->setValue("0");
  ledCharacteristic->setCallbacks(new LedCallbacks());

  service->start();

  BLEAdvertising *advertising =
      BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);

  BLEDevice::startAdvertising();

  Serial.println("BLE started.");
  Serial.println("Look for: LEDTest");
}

void loop() {
  delay(1000);
}