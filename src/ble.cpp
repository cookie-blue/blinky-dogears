#include "ble.h"

NimBLECharacteristic *BLE::_bleCharacteristic;

static bool parseUnsignedCommandValue(const std::string &command, int &value)
{
    if (command.length() <= 1)
    {
        return false;
    }

    for (size_t i = 1; i < command.length(); i++)
    {
        if (!isdigit(static_cast<unsigned char>(command[i])))
        {
            return false;
        }
    }

    value = atoi(command.substr(1, 3).c_str());
    return true;
}

class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *bleServer, NimBLEConnInfo &connInfo) override
    {
        Log::log("Client %s connected", connInfo.getAddress().toString().c_str());
        uint8_t color[] = {0, 255, 0};
        Indicator::blink(2, 50, 50, color);
    }

    void onDisconnect(NimBLEServer *bleServer, NimBLEConnInfo &connInfo, int reason) override
    {
        Log::log("Client %s disconnected. Reason %d", connInfo.getAddress().toString().c_str(), reason);
        uint8_t color[] = {255, 0, 0};
        Indicator::blink(3, 50, 50, color);
        NimBLEDevice::startAdvertising();
    }
};

class CommandCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *bleCharacteristic, NimBLEConnInfo &connInfo) override
    {
        std::string rx = bleCharacteristic->getValue();

        Log::log("Received command: %s", rx.c_str());
        uint8_t color[] = {0, 0, 255};
        Indicator::blink(1, 50, color);

        std::string response = "P" + std::to_string(PatternManager::getCurrentPattern());

        if (rx == "PING")
        {
            Log::log("PING received");
            response = "PONG";
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
            StaticOutput::off();
        }
        else if (rx.length() > 1 && rx[0] == 'B')
        {
            int brightness = 0;
            if (!parseUnsignedCommandValue(rx, brightness) || brightness < 0 || brightness > 100)
            {
                Log::log("Invalid brightness: %s", rx.c_str());
                response = "ERR";
            }
            else
            {
                PatternManager::override(255 * brightness / 100);
            }
        }
        else if (rx.length() > 1 && rx[0] == 'D')
        {
            int brightness = 0;
            if (!parseUnsignedCommandValue(rx, brightness) || brightness < 0 || brightness > 100)
            {
                Log::log("Invalid brightness: %s", rx.c_str());
                response = "ERR";
            }
            else
            {
                PatternManager::overrideBrightness(255 * brightness / 100);
            }
        }
        else if (rx.length() > 1 && rx[0] == 'P')
        {
            int pattern = 0;
            if (!parseUnsignedCommandValue(rx, pattern) || pattern < 1 || pattern > PatternManager::getNumPatterns())
            {
                Log::log("Pattern command out of range: %s", rx.c_str());
                response = "ERR";
            }
            else
            {
                PatternManager::setPattern(pattern - 1);
            }
        }
        else if (rx.length() > 1 && rx[0] == 'S')
        {
            int output = 0;
            if (!parseUnsignedCommandValue(rx, output) || output < 1 || output > STATIC_OUTPUT_COUNT)
            {
                Log::log("Static output command out of range: %s", rx.c_str());
                response = "ERR";
            }
            else
            {
                StaticOutput::toggle(output - 1);
            }
        }
        else
        {
            Log::log("Unknown command!");
            response = "ERR";
        }

        bleCharacteristic->setValue(response);
        bleCharacteristic->notify();
    }

    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        std::string address = connInfo.getAddress().toString();

        if (subValue == 0)
        {
            Log::log("BLE Client %s Unsubscribed", address.c_str());
        }
        if (subValue == 1)
        {
            Log::log("BLE Client %s Subscribed to notifications", address.c_str());
        }
        if (subValue == 2)
        {
            Log::log("BLE Client %s Subscribed to indications", address.c_str());
        }
        if (subValue == 3)
        {
            Log::log("BLE Client %s Subscribed to notifications and indications", address.c_str());
        }
    }
};

void BLE::init()
{
#ifndef ENABLE_BLE
    Log::log("BLE is disabled in Configuration");
    return;
#endif
#ifdef BLE_NAME
    char ble_name[] = BLE_NAME;
#else
    uint32_t chipid = ESP.getEfuseMac();
    char ble_name[7 + 13 + 1];
    snprintf(ble_name, (7 + 13 + 1), "Blinky %08X", (uint32_t)chipid);
#endif
    Log::log("using BLE device name '%s'", ble_name);
    NimBLEDevice::init(ble_name);

    NimBLEServer *bleServer = NimBLEDevice::createServer();
    bleServer->setCallbacks(new ServerCallbacks());

    NimBLEService *bleService = bleServer->createService(BLE_SERVICE_UUID);

    BLE::_bleCharacteristic = bleService->createCharacteristic(
        BLE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::READ |
            NIMBLE_PROPERTY::NOTIFY);

    BLE::_bleCharacteristic->setCallbacks(new CommandCallbacks());
    BLE::_bleCharacteristic->setValue("READY");

    NimBLEAdvertising *bleAdvertising = NimBLEDevice::getAdvertising();
    bleAdvertising->addServiceUUID(BLE_SERVICE_UUID);

    NimBLEAdvertisementData scanResponseData;
    scanResponseData.setName(ble_name);
    bleAdvertising->setScanResponseData(scanResponseData);

    bleAdvertising->start();

    Log::log("BLE ready, waiting for connection...");
}
