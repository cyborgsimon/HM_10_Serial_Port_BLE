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

#include <SoftwareSerial.h>
#include <QueueList.h>
#include "HM_10_BLE.h"

HM_10_BLE::HM_10_BLE(int8_t TXD, int8_t RXD) : SoftwareSerial(TXD, RXD) {
}

void HM_10_BLE::begin(char delimiter) {
  begin("HM_10_BLE", "123456", delimiter);
}

// init the HM-10 with custom name and password
void HM_10_BLE::begin(char* name, char* pass, char delimiter) {
  SoftwareSerial::begin(HM_10_BLE_BAUDRATE);

  #if HM_10_BLE_DEBUG
    Serial.begin(9600);
  #endif

  messageDelimiter = delimiter;

  atCommand("TYPE", "3");
  atCommand("NAME", name);
  atCommand("PASS", pass);
  atCommand("RESET");
}

void HM_10_BLE::queueATCommand(String cmd) {
  #if HM_10_BLE_DEBUG
    Serial.print("queue AT command: ");
    Serial.println(cmd);
  #endif
  atCommands.push(cmd);
}

// send AT commands
void HM_10_BLE::atCommand() {
  queueATCommand(String("AT"));
}

// send AT command without parameter
void HM_10_BLE::atCommand(char* cmd) {
  String cmdString = "AT+";
  cmdString += String(cmd);
  queueATCommand(cmdString);

}

// send AT command with parameter
void HM_10_BLE::atCommand(char* cmd, char* param) {
  String cmdString = "AT+";
  cmdString += String(cmd);
  cmdString += String(param);
  queueATCommand(cmdString);
}

// send, wait for and read AT command answer
bool HM_10_BLE::atHandler() {
  if (atCommands.isEmpty()) {
    return false; 
  }
  if (waitForMessage && !waitForATCommand) {
    return false;
  }
  if (!waitForATCommand) {
    #if HM_10_BLE_DEBUG
      Serial.print("send AT command: ");
    #endif
    String cmdString = atCommands.peek();
    char cmd[cmdString.length()+1];
    cmdString.toCharArray(cmd, cmdString.length()+1);
    write(cmd);
    waitForATCommand = true;
    return true;
  }
  if (available() <= 0) {
    if (readCount == 0) {
      return true;
    }
    readCount++;
    if (readCount > readTries) {
      handleATAnswer();
    }
    return true;
  }
  readCount = 1;
  atAnswer += (char)read();
  return true;
}

// remove AT command from queue after answer was received
void HM_10_BLE::handleATAnswer() {
  #if HM_10_BLE_DEBUG
    Serial.println(atAnswer);
  #endif
  atAnswer = "";
  waitForATCommand = false;
  readCount = 0;
  atCommands.pop();
}

// read messages from BLE serial and wait for delimiter
bool HM_10_BLE::messageHandler() {
  if (!atCommands.isEmpty() && !waitForMessage) {
    atHandler();
    return false; 
  }
  if (available() <= 0) {
    return true;
  }
  char character = read();
  if (character == messageDelimiter) {
    handleMessage();
    return true;
  }
  waitForMessage = true;
  message += character;
  return true;
}

void HM_10_BLE::processMessage(char* msg){
    // Overwrite in subclasses if startWrite is defined!
    #if HM_10_BLE_DEBUG
      Serial.print("process: ");
      Serial.println(msg);
    #endif
}

// received message via BLE
void HM_10_BLE::handleMessage() {
  // received an AT command
  // if (message.equals("AT") || message.startsWith("AT+")) {
  //   queueATCommand(message);
  // } else {
    char msg[message.length()+1];
    message.toCharArray(msg, message.length()+1);
    processMessage(msg);
  // }
  #if HM_10_BLE_DEBUG
    Serial.println(message);
  #endif
  message = "";
  waitForMessage = false;
}

// write message via BLE serial
void HM_10_BLE::writeMessage(char* msg) {
  write(msg);
  write(messageDelimiter);
}