# HM-10 Serial Port BLE Arduino Library
This is an Arduino library for the HM-10 Serial Port BLE Module (Bluetooth 4.0).
It is based on the Arduino SoftwareSerial Library and requires the QueueList library from the Arduino Playground (http://playground.arduino.cc/Code/QueueList)

## Installation

Install the QueueList Library from the Arduino Playground (http://playground.arduino.cc/Code/QueueList).

To download. click the DOWNLOADS button in the top right corner.
Rename the uncompressed folder to HM_10_Serial_Port_BLE.
Place the HM_10_Serial_Port_BLE library folder into your <arduinosketchfolder>/libraries/ folder.
You may need to create the libraries subfolder if its your first library.
Then Restart the IDE.

## Connect it to your Arduino

Connect the GND pin of the HM-10 module to GND pin of your Arduino.
Connect the VCC pin of the HM-10 module to the 3.3V pin of your Arduino.
Connect the RX and TX pins to Digital Pins of your Arduino:
- Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
- Not all pins on the Leonardo and Micro support change interrupts, so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
- On Arduino or Genuino 101 the current maximum RX speed is 57600bps
- On Arduino or Genuino 101 RX doesn't work on Pin 13
See also at (https://www.arduino.cc/en/Reference/SoftwareSerial)


