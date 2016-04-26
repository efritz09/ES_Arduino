#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "Arduino.h"

#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>

#include "BluefruitConfig.h"
#include "Bluetooth.h"

#include "TemplateFSM.h"

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

bool connection = false;

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/*
 * WriteBluetooth
 */
void WriteBluetooth(char c) {
  ble.print(c);
}
/*
 * CheckBluetooth
 * Checks for an incomming message. Decodes the message and posts the appropriate event
 */
void CheckBluetooth(void) {
  //checks for serial characters to send
  char n, inputs[BUFSIZE+1];
  if (Serial.available()) {
    n = Serial.readBytes(inputs, BUFSIZE);
    inputs[n] = 0;
    // Send characters to Bluefruit
    Serial.print("Sending: ");
    Serial.println(inputs);
    // Send input data to host via Bluefruit
    ble.print(inputs);
  }
  //checks for incomming messages
  while ( ble.available() ) {
    int c = ble.read();    
    ES_Event thisEvent;
    char cmd = (char)c;
    switch (cmd) {
      case 'B' : //mode: blink 
        thisEvent.EventType = ES_BLINK;
        Serial.println("mode: blink");
        break;
      case 'O' : //mode: on 
        thisEvent.EventType = ES_ON;
        Serial.println("mode: on");
        break;
      case 'S' : //state: solid
        thisEvent.EventType = ES_SOLID;
        Serial.println("state: solid");
        break;
      case 'A' : //state: auto
        thisEvent.EventType = ES_AUTO;
        Serial.println("state: auto");
        break;
      case 'C' : //connected
        connection = true;
        thisEvent.EventType = ES_CONNECTED;
        Serial.println("connected");
        break;
      case 'D' : //disconnected
        connection = false;
        thisEvent.EventType = ES_DISCONNECTED;
        ble.disconnect();
        ble.reset();
        Serial.println("disconnected");
        break;
      default : 
        thisEvent.EventType = ES_WRONG;
        break;
    }
    PostTemplateFSM(thisEvent);
  }
}
//bool CheckBluetoothConnection(void) {
//  return false;
//}

/*
 * CheckBluetoothConnection
 * This is sort of a hack to keep ble.isConnected() from bricking the code. 
 */
bool CheckBluetoothConnection(void) {
  ES_Event newEvent;
  if(!connection) { //keeps ble.isConnected from blocking the code when connected
//    Serial.println("in btconnect");
    if(ble.isConnected()) {
      Serial.println("BLUETOOTH CONNECTED!!!");
        if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
        // Change Mode LED Activity
        Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
        ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
      }
      ble.setMode(BLUEFRUIT_MODE_DATA);
      newEvent.EventType = ES_CONNECTED;
      connection = true;
      PostTemplateFSM(newEvent);
      return true;
    }
    //may not need this...
//    else {
//      Serial.println("disconnecting");
//      ble.disconnect();
//    }
  }
}
/*
 * CheckBluetoothConnection
 * An older version that I thought should work but it appears that 
 * ble.isConnected has serious blocking code
 */
//bool CheckBluetoothConnection (void) {
//  static bool connection = false;
//  ES_Event newEvent;
//  bool isconnected = ble.isConnected();
//  if(isconnected) {
//    Serial.println("BLUETOOTH CONNECTED!!!");
//    if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
//    // Change Mode LED Activity
//    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
//    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
//  }
//    ble.setMode(BLUEFRUIT_MODE_DATA);
//    newEvent.EventType = ES_CONNECTED;
////    connection = true;
//    PostTemplateFSM(newEvent);
//    return true;
////  }else if(!isconnected) {
////    Serial.println("BLUETOOTH DISCONNECTED...");
////    newEvent.EventType = ES_DISCONNECTED;
////    connection = false;
////    PostTemplateFSM(newEvent);
////    return true;
//  }
//  return false;
//}

void setup() {
  // put your setup code here, to run once:
  
  while (!Serial);  // required for Flora & Micro
  delay(500);
  Serial.begin(9600);
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));
  if ( !ble.begin(VERBOSE_MODE) ) error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  Serial.println( F("OK!") );
  if ( FACTORYRESET_ENABLE ) {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) error(F("Couldn't factory reset"));
  }
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();
  ble.verbose(false);  // debug info is a little annoying after this point!
  /* This has been moved to an event checker */
//  while (! ble.isConnected()) delay(500);
//  Serial.println(F("******************************"));
//  // LED Activity command is only supported from 0.6.6
//  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
//    // Change Mode LED Activity
//    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
//    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
//  }
//  // Set module to DATA mode
//  Serial.println( F("Switching to DATA mode!") );
//  ble.setMode(BLUEFRUIT_MODE_DATA);
//  Serial.println(F("******************************"));   
}

void loop() {
  
  ES_Return_t ErrorType = ES_Initialize(ES_Timer_RATE_1MS);
  if ( ErrorType == Success ) {
    ErrorType = ES_Run();
  }
  //if we got to here, there was an error
  switch (ErrorType){
    case FailedPost:
      Serial.println("Failed on attempt to Post");
      break;
    case FailedPointer:
      Serial.println("Failed on NULL pointer");
      break;
    case FailedInit:
      Serial.println("Failed Initialization");
      break;
   default:
      Serial.println("Other Failure");
      break;
}

for(;;)
  ;

}
