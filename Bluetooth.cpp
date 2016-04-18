//
///****************************************************************************
// Module
//   Bluetooth.c
//
// Revision
//   1.0.1
//
// Description
//   This is a template file for implementing flat state machines under the
//   Gen2 Events and Services Framework.
//
// Notes
//
// History
// When           Who     What/Why
// -------------- ---     --------
// 01/15/12 11:12 jec      revisions for Gen2 framework
// 11/07/11 11:26 jec      made the queue static
// 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
// 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
//****************************************************************************/
///*----------------------------- Include Files -----------------------------*/
///* include header files for this state machine as well as any machines at the
//   next lower level in the hierarchy that are sub-machines to this machine
//*/
//#include <Arduino.h>
//#include <SPI.h>
//#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
//  #include <SoftwareSerial.h>
//#endif
//
//#include <Adafruit_BLE.h>
//#include <Adafruit_BluefruitLE_SPI.h>
//#include <Adafruit_BluefruitLE_UART.h>
//
//#include "BluefruitConfig.h"
//#include "Bluetooth.h"
//#include "Arduino.h"
//
///*----------------------------- Module Defines ----------------------------*/
//#define FACTORYRESET_ENABLE         0
//#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
//#define MODE_LED_BEHAVIOUR          "MODE"
///*---------------------------- Module Functions ---------------------------*/
///* prototypes for private functions for this machine.They should be functions
//   relevant to the behavior of this state machine
//*/
//void error(const __FlashStringHelper*err) {
//  Serial.println(err);
//  while (1);
//}
//
///*---------------------------- Module Variables ---------------------------*/
//
//SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
//
//Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
//                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
//
//
///*------------------------------ Module Code ------------------------------*/
//
//void CheckBluetooth(void) {
//  // Check for user input
//  char n, inputs[BUFSIZE+1];
//
//  if (Serial.available())
//  {
//    n = Serial.readBytes(inputs, BUFSIZE);
//    inputs[n] = 0;
//    // Send characters to Bluefruit
//    Serial.print("Sending: ");
//    Serial.println(inputs);
//
//    // Send input data to host via Bluefruit
//    ble.print(inputs);
//  }
//
//  // Echo received data
//  while ( ble.available() )
//  {
//    int c = ble.read();
//
//    Serial.print((char)c);
//
//    // Hex output too, helps w/debugging!
//    Serial.print(" [0x");
//    if (c <= 0xF) Serial.print(F("0"));
//    Serial.print(c, HEX);
//    Serial.print("] ");
//  }
//}
//
//
///***************************************************************************
// private functions
// ***************************************************************************/

