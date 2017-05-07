// The MIT License (MIT)

// Copyright (c) 2017 Simon Kirchner (cyborg.simon@gmail.com)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _HM_10_BLE_H_
#define _HM_10_BLE_H_

#if ARDUINO >= 100
 #include "Arduino.h"
 #define WIRE_WRITE Wire.write
#else
 #include "WProgram.h"
  #define WIRE_WRITE Wire.send
#endif

#include <SoftwareSerial.h>
#include <QueueList.h>

#define HM_10_BLE_DEBUG false
#define HM_10_BLE_BAUDRATE 9600

class HM_10_BLE : public SoftwareSerial {
public:
  HM_10_BLE(int8_t RXD, int8_t TXD);

  void begin(char delimiter);
  void begin(char* name, char* pass, char delimiter);
  
  void atCommand();
  void atCommand(char* cmd);
  void atCommand(char* cmd, char* param);

  bool atHandler();
  
  bool messageHandler();

  // overwrite this if you want something to happen when a message is received
  virtual void processMessage(char*);
 
private:
  const int8_t readTries = 20;
  int8_t readCount = 0;

  long baudrate;

  bool waitForATCommand = false;
  QueueList <String> atCommands;
  String atAnswer;
  
  void queueATCommand(String cmd);
  void handleATAnswer();

  bool waitForMessage = false;
  char messageDelimiter = '!';
  String message;
  void handleMessage();
  void writeMessage(char* msg);
};

#endif /* _HM_10_BLE_H_ */
