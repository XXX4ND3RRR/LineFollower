/*
   -- New project --

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 4.14.08 or later version;
     - for iOS 1.11.2 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"


#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 101 bytes
{ 255, 13, 0, 0, 0, 94, 0, 18, 0, 0, 0, 31, 1, 106, 200, 1, 1, 7, 0, 129,
  7, 3, 15, 12, 64, 17, 75, 112, 0, 129, 8, 31, 11, 12, 64, 17, 75, 105, 0, 129,
  8, 59, 15, 12, 64, 17, 75, 100, 0, 7, 7, 16, 40, 10, 108, 64, 2, 26, 2, 2,
  7, 8, 43, 40, 10, 108, 64, 2, 26, 2, 2, 7, 8, 71, 40, 10, 108, 64, 2, 26,
  2, 2, 1, 16, 121, 75, 75, 0, 2, 31, 115, 116, 97, 114, 116, 45, 115, 116, 111, 112,
  0
};

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  float ValueProp;
  float ValueInt;
  float ValueDif;
  uint8_t StartStop; // =1 if button pressed, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup()
{
  RemoteXY_Init ();


  // TODO you setup code
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  RemoteXY_Handler ();


  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
  if (RemoteXY.StartStop == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
