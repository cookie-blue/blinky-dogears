#pragma once

#include "indicator.h"
#include "pattern_manager.h"
#include <NimBLEDevice.h>

class BLE
{
private:
  static NimBLECharacteristic *_bleCharacteristic;
public:
  static void init();
};
