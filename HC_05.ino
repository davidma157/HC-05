#include "Arduino.h"

// Basic Bluetooth sketch HC-05_02_38400
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to commincation mode when first powered on.
// Needs to be placed in to AT mode
// After a factory reset the default baud rate for communication mode is 38400
//
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need voltage divider)

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); // RX | TX // @suppress("Abstract class cannot be instantiated")
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.

char c = ' ';
long int configBaudRate = 38400;
long int commBaudRate = 9600;
bool configMode = false;

String answer;
String msg;

void setup() {
//    configMode = true;
  // start th serial communication with the host computer
  Serial.begin(9600);
  Serial.println();
  Serial.println("Arduino with HC-05 is ready. V3");

  Serial.println("BTserial started at " + String(getBaudRate()));
  BTSerial.begin(getBaudRate());
  Serial.println("BTserial started at " + String(getBaudRate()));
}

void loop() {
  if (configMode)
    loopConfig();
  else
    loopComm();
}

void loopComm() {
  //Read command from monitor
  readSerialPort();

  //Read answer from slave
  while (BTSerial.available()) {
    delay(10);
    if (BTSerial.available() > 0) {
      char c = BTSerial.read();  //gets one byte from serial buffer
      answer += c; //makes the string readString
    }
  }
  //Send data to slave
  if (msg != "") {
    Serial.print("Master sent : ");
    Serial.println(msg);
    BTSerial.print(msg);
    msg = "";
  }
  //Send answer to monitor
  if (answer != "") {
    Serial.print("Slave received : ");
    Serial.println(answer);
    answer = "";
  }

}

void readSerialPort() {
  while (Serial.available()) {
    delay(10);
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      msg += c; //makes the string readString
    }
  }
  Serial.flush();
}

void loopConfig() {
  while (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }

  while (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

long int getBaudRate() {
  if (configMode) {
    //    Serial.println("BaudRate: " + String(configBaudRate));
    return configBaudRate;
  } else {
    //    Serial.println("BaudRate: " + String(commBaudRate));
    return commBaudRate;
  }
}

