// Copyright 2018 Takashi Toyoshima <toyoshim@gmail.com>. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// PSX modification by nzgamer41 <nzgamer41@gmail.com>

#include "jvsio/clients/NanoClient.cpp"
#include "jvsio/JVSIO.cpp"
#include "psx/Psx.cpp"
#include "psx/Psx.h"

// PIO pins used by JVSIO
//  RX0 - JVS Data+
//  D2  - JVS Data-
//  D3  - JVS Sense
//  D13 - LED

// PIO pins used by PSX Controller
// D9 - PSX Data
// D10 - PSX CMD
// D11 - PSX Att
// D12 - PSX Clock

NanoDataClient data;
NanoSenseClient sense;
NanoLedClient led;
JVSIO io(&data, &sense, &led);
Psx Psx;

uint16_t psx_data = 0;
static const char gamepad_id[] = "SEGA ENTERPRISES,LTD.compat;IONA-PSX-NANO;ver1.00;Gamepad Mode";

uint8_t coin_count = 0;
uint8_t coin = 0;


void updateState() {
  psx_data = Psx.read();
  //Serial.println(psx_data);
//check button
}

uint8_t report(size_t player, size_t line) {
  //       |  B7 |  B6 |  B5 |  B4 |  B3 |  B2 |  B1 |  B0 |
  // ------+-----+-----+-----+-----+-----+-----+-----+-----+
  // P0-L0 |TEST |  -  | -   |  -  |  -  |  -  |  -  |  -  |
  // P0-L1 |Start|  -  | Up  |Down |Left |Right|  A  |  B  |
  // P0-L2 |  C  |  X  |  Y  |  Z  |  L  |  R  |  -  |  -  |
  psx_data = Psx.read();
  char playertxt[] = "Player is 0";
  //Serial.println(psx_data);
  //Serial.println(line);
  //Serial.println(player);
  if (player != 0 || line > 2)
    return 0x00;

  /*else if (psx_data & psxSlct){
    coin_count++;
    }*/

  //Serial.println(playertxt);
  //ew jvs
  uint8_t result = 0x00;
  if (line == 0){
    //Serial.println("line0");
    // TEST
    if (psx_data & psxL2){
      Serial.println("L2, 0x80");
      return 0x80;
    }
  }
  if (line == 1) {
    if (psx_data & psxLeft) {
      Serial.println("Left, 0x08");
      result |= 0x08;
    } else if (psx_data & psxRight) {
      Serial.println("Right, 0x04");
      result |= 0x04;
    } else if (psx_data & psxUp) {
      Serial.println("Up, 0x20");
      result |= 0x20;
    } else if (psx_data & psxDown) {
      Serial.println("Down, 0x10");
      result |= 0x10;
    } else if (psx_data & psxStrt) {
      Serial.println("Start, 0x80");
      result |= 0x80;
    } else if (psx_data & psxX) {
      Serial.println("X, 0x02");
      // PUSH1
      result |= 0x02;
    } else if (psx_data & psxO) {
      Serial.println("O, 0x01");
      // PUSH2
      result |= 0x01;
    } else if (psx_data & psxR2) {
      Serial.println("L2, 0x40");
      result |= 0x40;
    }
  }
  else if (line == 2){
    if (psx_data & psxSqu) {
      // PUSH3
      result |= 0x80;
    }
    else if (psx_data & psxTri) {
      // PUSH4
      result |= 0x40;
    }
    else if (psx_data & psxR1) {
      // PUSH5
      result |= 0x20;
    }
    else if (psx_data & psxL1) {
      // PUSH6
      result |= 0x10;
    }
  }
    uint8_t newCoin = (psx_data & psxSlct);
    if (coin && !newCoin){
      Serial.println("inserting a coin..");
      coin_count++;
      Serial.println(coin_count);
    }
    coin = newCoin;
    //Serial.println(result);
    return result;
}

void setup() {
  Psx.setupPins(8, 9, 11, 12, 10);
  Serial.begin(115200);
  io.begin();
}

void loop() {
  char test[] = "data false";
  char test2[] = "data not false";
  uint8_t len;
  uint8_t* data = io.getNextCommand(&len);
  if (!data) {
    updateState();
    return;
  }
  switch (*data) {
    case JVSIO::kCmdIoId:
      io.pushReport(JVSIO::kReportOk);
      {
        const char* id = gamepad_id;
        for (size_t i = 0; id[i]; ++i)
          io.pushReport(id[i]);
      }
      io.pushReport(0);

      // Initialize.
      coin_count = 0;
      break;
    case JVSIO::kCmdFunctionCheck:
      io.pushReport(JVSIO::kReportOk);

      io.pushReport(0x01);  // sw
      io.pushReport(0x02);  // players
      io.pushReport(0x0C);  // buttons
      io.pushReport(0x00);

      io.pushReport(0x03);  // analog inputs
      io.pushReport(0x08);  // channels
      io.pushReport(0x00);  // bits
      io.pushReport(0x00);

      io.pushReport(0x12);  // general purpose driver
      io.pushReport(0x08);  // slots
      io.pushReport(0x00);
      io.pushReport(0x00);

      io.pushReport(0x02);  // coin
      io.pushReport(0x02);  // slots
      io.pushReport(0x00);
      io.pushReport(0x00);

      io.pushReport(0x00);
      break;
    case JVSIO::kCmdSwInput:
      io.pushReport(JVSIO::kReportOk);
      //io.pushReport(0x00);  // TEST, TILT1-3, and undefined x4.
      for (size_t player = 0; player < data[1]; ++player) {
        for (size_t line = 0; line <= data[2]; ++line)
          io.pushReport(report(player, line));
      }
      break;
    case JVSIO::kCmdCoinInput:
      io.pushReport(JVSIO::kReportOk);
      for (size_t slot = 0; slot < data[1]; ++slot) {
        io.pushReport((0 << 6) | 0);
        io.pushReport(slot ? 0x00 : coin_count);
      }
      break;
    case JVSIO::kCmdAnalogInput:
      io.pushReport(JVSIO::kReportOk);
      for (size_t channel = 0; channel < data[1]; ++channel) {
        io.pushReport(0x80);
        io.pushReport(0x00);
      }
      break;
    case JVSIO::kCmdCoinSub:
      if (data[1] == 0)
        coin_count -= data[3];
      io.pushReport(JVSIO::kReportOk);
      break;
    case JVSIO::kCmdCoinAdd:
      coin_count += data[3];
      io.pushReport(JVSIO::kReportOk);
      break;
    case JVSIO::kCmdDriverOutput:
      io.pushReport(JVSIO::kReportOk);
      break;
  }
}
