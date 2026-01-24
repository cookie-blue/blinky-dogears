#include "ble.h"
#include "config.cpp"
#include "pattern_manager.h"
#include <NimBLEDevice.h>

NimBLECharacteristic *bleCharacteristic;

class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *bleServer, NimBLEConnInfo &connInfo) override
    {
        log("Client %s connected", connInfo.getAddress().toString().c_str());
        #ifdef STATUS_LED_PIN
            for (uint8_t i = 0; i < 2; i++)
            {
                digitalWrite(STATUS_LED_PIN, LOW);
                delay(50);
                digitalWrite(STATUS_LED_PIN, HIGH);
                delay(50);
            }
        #endif
    }

    void onDisconnect(NimBLEServer *bleServer, NimBLEConnInfo &connInfo, int reason) override
    {
        log("Client %s disconnected. Reason %d", connInfo.getAddress().toString().c_str(), reason);
        #ifdef STATUS_LED_PIN
            for (uint8_t i = 0; i < 3; i++)
            {
                digitalWrite(STATUS_LED_PIN, LOW);
                delay(50);
                digitalWrite(STATUS_LED_PIN, HIGH);
                delay(50);
            }
        #endif
        NimBLEDevice::startAdvertising();
    }
};

class CommandCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *bleCharacteristic, NimBLEConnInfo &connInfo) override
    {
        std::string rx = bleCharacteristic->getValue();

        log("Received command: %s", rx.c_str());

        #ifdef STATUS_LED_PIN
            digitalWrite(STATUS_LED_PIN, LOW);
            delay(50);
            digitalWrite(STATUS_LED_PIN, HIGH);
        #endif

        if (rx == "PING")
        {
            log("PING received");
            bleCharacteristic->setValue("PONG");
            bleCharacteristic->notify();
        }
        else if (rx == "+")
        {
            PatternManager::prevPattern();
        }
        else if (rx == "-")
        {
            PatternManager::nextPattern();
        }
        else if (rx == "ON")
        {
            PatternManager::on();
        }
        else if (rx == "OFF" || rx == "0")
        {
            PatternManager::off();
        }
        else if (rx == "25")
        {
            PatternManager::override(64);
        }
        else if (rx == "50")
        {
            PatternManager::override(128);
        }
        else if (rx == "75")
        {
            PatternManager::override(192);
        }
        else if (rx == "100")
        {
            PatternManager::override(255);
        }
        else if (rx.length() > 1 && rx[0] == 'P')
        {
            int pattern = std::stoi(rx.substr(1));
            PatternManager::setPattern(pattern - 1);
        }
        else
        {
            log("Unknown command!");
        }

        std::string response = "P" + std::to_string(PatternManager::getCurrentPattern());
        bleCharacteristic->setValue(response);
        bleCharacteristic->notify();
    }

    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        std::string address = connInfo.getAddress().toString();

        if (subValue == 0)
        {
            log("BLE Client %s Unsubscribed", address.c_str());
        }
        if (subValue == 1)
        {
            log("BLE Client %s Subscribed to notifications", address.c_str());
        }
        if (subValue == 2)
        {
            log("BLE Client %s Subscribed to indications", address.c_str());
        }
        if (subValue == 3)
        {
            log("BLE Client %s Subscribed to notifications and indications", address.c_str());
        }
    }
};

void BLE::init()
{
#ifdef BLE_NAME
    char ble_name[] = BLE_NAME;
#else
    uint32_t chipid = ESP.getEfuseMac();
    char ble_name[7 + 13 + 1];
    snprintf(ble_name, (7 + 13 + 1), "Blinky %08X", (uint32_t)chipid);
#endif
    log("using BLE device name '%s'", ble_name);
    NimBLEDevice::init(ble_name);

    NimBLEServer *bleServer = NimBLEDevice::createServer();
    bleServer->setCallbacks(new ServerCallbacks());

    NimBLEService *bleService = bleServer->createService(BLE_SERVICE_UUID);

    bleCharacteristic = bleService->createCharacteristic(
        BLE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::READ |
            NIMBLE_PROPERTY::NOTIFY);

    bleCharacteristic->setCallbacks(new CommandCallbacks());
    bleCharacteristic->setValue("READY");

    bleService->start();

    NimBLEAdvertising *bleAdvertising = NimBLEDevice::getAdvertising();
    bleAdvertising->addServiceUUID(BLE_SERVICE_UUID);

    NimBLEAdvertisementData scanResponseData;
    scanResponseData.setName(ble_name);
    bleAdvertising->setScanResponseData(scanResponseData);

    bleAdvertising->start();

    log("BLE ready, waiting for connection...");
}
