#include <Arduino.h>

#define DEBUGLOG_DEFAULT_LOG_LEVEL_INFO

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOSCWiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "hid_l2cap.h"
#include "joycon_hid.h"

#include "settings.h"

#ifdef SEND_VRC_OSC
#include "vrcosc.h"
#endif
#ifdef SEND_VMT_OSC
#include "vmtosc.h"
#endif

#define BT_CONNECT_TIMEOUT  10000
static uint32_t startMillis;

static int initStep = 0;

#ifdef LEFT_HAND
  uint8_t btn1Assign[] = { 1,2,4,8 };
#else
  uint8_t btn1Assign[] = { 8,4,2,1 };
  const uint8_t rightHandstickPos[] = { 4,5,6,7,0,1,2,3,8 };
#endif

void OnConnect()
{
  if ( initStep == 0 ) {
#ifdef SEND_VMT_OSC
    VMTOSC::SendVmtEnableOSC();
#endif
    JoyconHid::SetInputReportMode();
    delay(200);
    initStep++;
  }
  else if ( initStep == 2 ) {
    JoyconHid::SetPlayerLEDByNumber(1);
    initStep++;
  }
}

void TestFunction()
{
  static int nFunc = 0;
  switch (nFunc) {
    case 0: JoyconHid::GetControllerState(); break;
    case 1: JoyconHid::RequestDeviceInfo(); break;
  }
  nFunc++;
  if ( nFunc > 1 ) nFunc = 0;
}

// callback in receiving input 0x3f report
#ifdef SEND_VMT_OSC
void recv_callback_0x3f_vmtosc(uint8_t *p_msg)
{
  if ( p_msg[1] != 0x3f ) return;
  uint8_t btn1 = p_msg[2];
  uint8_t btn2 = p_msg[3];
  uint8_t stickPos = p_msg[4];

  #ifndef LEFT_HAND
    stickPos = rightHandstickPos[stickPos]; // convert
  #endif

  VMTOSC::SendVmtButtonOsc( 0, btn2 & 0x30 ); // Home or Capture button 
  VMTOSC::SendVmtButtonOsc( 1, btn1 & btn1Assign[1] ); // down/B button 
  VMTOSC::SendVmtButtonOsc( 3, btn1 & btn1Assign[2] ); // up/X button

  VMTOSC::SendVmtTriggerOsc( 0, (btn2 & 0x40) ? 1.0 : 0.0 ); // L or R
  VMTOSC::SendVmtTriggerOsc( 1, (btn2 & 0x80) ? 1.0 : 0.0 ); // ZL or ZR
  VMTOSC::SendVmtTriggerOsc( 2, (btn2 & 0x80) ? 0.5 : 0.0 ); // ZL or ZR Grip Force for VRChat
  VMTOSC::SendVmtStickClickOsc(btn2 & (0x04 | 0x08)); // Left or Right stick clicked

  const int8_t stickPosToX[] = {1,1,0,-1,-1,-1,0,1,0};
  const int8_t stickPosToY[] = {0,-1,-1,-1,0,1,1,1,0};
  VMTOSC::SendVmtStickXYOsc((float)stickPosToX[stickPos], (float)stickPosToY[stickPos]  );
}

#endif

#ifdef SEND_VRC_OSC
void recv_callback_0x3f_vrcosc(uint8_t *p_msg)
{
  if ( p_msg[1] != 0x3f ) return;
  uint8_t btn1 = p_msg[2];
  uint8_t btn2 = p_msg[3];
  uint8_t stickPos = p_msg[4];

  // BUTTONs part
  VRCOSC::OSCSendBool( OSCPATH_BUTTON_COMFORTLEFT, btn1 & btn1Assign[0] ); // left/Y button
  VRCOSC::OSCSendBool( OSCPATH_BUTTONA, btn1 & btn1Assign[1] ); // down/B button 
  VRCOSC::OSCSendBool( OSCPATH_BUTTONB, btn1 & btn1Assign[2] ); // up/X button
  VRCOSC::OSCSendBool( OSCPATH_BUTTON_COMFORTRIGHT, btn1 & btn1Assign[3] ); // right/A button
 
  if ( btn2 & 0x80 ) { // ZL or ZR : test function
    TestFunction();
  }

  // STICK part
  #ifdef LEFT_HAND
    if ( stickPos >= 0 && stickPos <=  8) {
      if ( stickPos == 8 ) {
        VRCOSC::OSCSendFloat(OSCPATH_AXIS_HORIZONTAL, 0.0);
        VRCOSC::OSCSendFloat(OSCPATH_AXIS_VERTICAL, 0.0);
      }
      else {
        if ( stickPos == 3 || stickPos == 4 || stickPos == 5 ) {
          VRCOSC::OSCSendFloat(OSCPATH_AXIS_HORIZONTAL, -1.0);
        }
        if ( stickPos == 7 || stickPos == 0 || stickPos == 1 ) {
          VRCOSC::OSCSendFloat(OSCPATH_AXIS_HORIZONTAL, 1.0);
        }
        if ( stickPos == 1 || stickPos == 2 || stickPos == 3 ) {
          VRCOSC::OSCSendFloat(OSCPATH_AXIS_VERTICAL, -1.0);
        }
        if ( stickPos == 5 || stickPos == 6 || stickPos == 7 ) {
          VRCOSC::OSCSendFloat(OSCPATH_AXIS_VERTICAL, 1.0);
        }
      }
    }
  #else
    stickPos = rightHandstickPos[stickPos]; // convert

    if ( stickPos >= 0 && stickPos <=  8) {
      if ( stickPos == 3 || stickPos == 4 || stickPos == 5 ) {
        VRCOSC::OSCSendFloat(OSCPATH_AXIS_LOOK_HORIZONTAL, -1.0);
      }
      else if ( stickPos == 7 || stickPos == 0 || stickPos == 1 ) {
        VRCOSC::OSCSendFloat(OSCPATH_AXIS_LOOK_HORIZONTAL, 1.0);
      }
      else {
        VRCOSC::OSCSendFloat(OSCPATH_AXIS_LOOK_HORIZONTAL, 0.0);
      }
    }
  #endif
}
#endif

void recv_callback_0x3f_printlog(uint8_t *p_msg)
{
  if ( p_msg[1] != 0x3f ) return;
  uint8_t btn1 = p_msg[2];
  uint8_t btn2 = p_msg[3];
  uint8_t stickPos = p_msg[4];

  if ( btn1 & btn1Assign[0] ) { Serial.println("Left/Y button"); } //SL button
  if ( btn1 & btn1Assign[1] ) { Serial.println("Down/B button"); } //SR button
  if ( btn1 & btn1Assign[2] ) { Serial.println("Up/X button"); } //SL button
  if ( btn1 & btn1Assign[3] ) { Serial.println("Right/A button"); } //SR button
  if ( btn1 & 0x10 ) { Serial.println("SL"); } //SL button
  if ( btn1 & 0x20 ) { Serial.println("SR"); } //SR button

  if ( btn2 & 0x01 ) { Serial.println("Minus"); } //Minus button
  if ( btn2 & 0x02 ) { Serial.println("Plus"); } //Plus button
  if ( btn2 & 0x04 ) { Serial.println("Left Stick Clicked"); } //Left Stick Clicked
  if ( btn2 & 0x08 ) { Serial.println("Right Stick Clicked"); } //Right Stick Clicked
  if ( btn2 & 0x10 ) { Serial.println("Home"); } //Home
  if ( btn2 & 0x20 ) { Serial.println("Capture"); } //Capture
  if ( btn2 & 0x40 ) { Serial.println("L or R");} // L or R
  if ( btn2 & 0x80 ) { Serial.println("ZL or ZR"); }// ZL or ZR : test function

  #ifndef LEFT_HAND
    stickPos = rightHandstickPos[stickPos]; // convert
  #endif

  const char* stickPosName[] = {
    "Right", "RightDown", "Down", //0,1,2,
    "LeftDown", "Left", "LeftUp", //3,4,5,
    "Up", "RightUp", "neutral"    //6,7,8
  };
  if ( stickPos >= 0 && stickPos <=  8) {
    Serial.printf("Stick: %s\n", stickPosName[stickPos]);
  }

  if ( btn1 & 0xf ) {
    static int playerLED = 0;    
    playerLED++;
    if ( playerLED > 4 ) playerLED = 1;
    JoyconHid::SetPlayerLEDByNumber(playerLED);
  }

}


void recv_callback_0x21_printlog(uint8_t *p_msg)
{
  if ( p_msg[1] != 0x21 ) return;
  uint8_t batteryLevel = (p_msg[3] & 0xf0) >> 4;
  Serial.printf( "0x21 report: in reply to command %02x\n", p_msg[15]);
  Serial.printf( "\tBattery Level: %d\n", batteryLevel );

  uint8_t btn2 = p_msg[5]; // LR Shared

  #ifdef LEFT_HAND
  uint8_t btn1 = p_msg[6];
  uint8_t* stickData = p_msg + 7;
  #else
  uint8_t btn1 = p_msg[4];
  uint8_t* stickData = p_msg + 10;
  #endif

  String buttonStr;
  if (btn1 & 0x01) buttonStr += "Y/Down,";
  if (btn1 & 0x02) buttonStr += "X/Up,";
  if (btn1 & 0x04) buttonStr += "B/Right,";
  if (btn1 & 0x08) buttonStr += "A/Left,";
  if (btn1 & 0x10) buttonStr += ",SR";
  if (btn1 & 0x20) buttonStr += ",SL";
  if (btn1 & 0x40) buttonStr += ",L/R";
  if (btn1 & 0x80) buttonStr += ",ZL/ZR";

  if (btn2 & 0x01) buttonStr += ",Minus";
  if (btn2 & 0x02) buttonStr += ",Plus";
  if (btn2 & 0x04) buttonStr += ",R Stick";
  if (btn2 & 0x08) buttonStr += ",L Stick";
  if (btn2 & 0x10) buttonStr += ",Home";
  if (btn2 & 0x20) buttonStr += ",Cap";
  //if (btn2 & 0x40) buttonStr += "";
  if (btn2 & 0x80) buttonStr += ",Charging Grip";
  Serial.printf("\tButtons: %s\n", buttonStr );

  uint16_t stick_horizontal = stickData[0] | ((stickData[1] & 0xF) << 8);
  uint16_t stick_vertical = (stickData[1] >> 4) | (stickData[2] << 4);
  Serial.printf("\tStick: x= %d, y= %d\n", stick_horizontal, stick_vertical);
}

void recv_callback(uint8_t *p_msg)
{
  // For details on JoyCon's report, refer to the webpage
  // https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/blob/master/bluetooth_hid_notes.md
  uint8_t reportID = p_msg[1];
  if (p_msg[0] == 0xa1) { // input report
    switch (reportID) {
      case 0x3f: 
        recv_callback_0x3f_printlog(p_msg);
#ifdef SEND_VRC_OSC
        recv_callback_0x3f_vrcosc(p_msg); 
#endif
#ifdef SEND_VMT_OSC
        recv_callback_0x3f_vmtosc(p_msg);
#endif
        break;
      case 0x21:
        if (initStep < 2 && p_msg[15] == 0x03 ) initStep++;
        recv_callback_0x21_printlog(p_msg);
        break;
    }
  }
  else if (p_msg[0] == 0xa2 ) { // output report
  }
  else if (p_msg[0] == 0xa3 ) { // feature report
  }
  return;
}

void setup() {
  // Serial
  Serial.begin(115200);
  Serial.printf("[%s] Setup start.\n", __func__ );

  // WiFi
  WiFiManager wifiManager;
  Serial.printf("[%s] Trying WiFiManager's autoConnect.\n", __func__ );
  Serial.printf("[%s] If it will not proceed, check WiFi SSID %s with your smartphone.", __func__, WIFIMAN_SSID);
  wifiManager.autoConnect(WIFIMAN_SSID, WIFIMAN_PASS);
  Serial.printf("[%s] WiFi autoConnect done.\n", __func__ );

  // Bluetooth classic, HID
  long ret = hid_l2cap_initialize(recv_callback);
  if( ret != 0 ){
    Serial.printf("[%s] hid_l2cap_initialize error.\n", __func__ );
    return;
  }

  BD_ADDR addr = TARGET_BT_ADDR;
  startMillis = millis();
  ret = hid_l2cap_connect(addr); // 1st trial to connect
  Serial.printf("[%s] Setup finished.\n", __func__ );
}

void loop() {
  BT_STATUS status = hid_l2cap_is_connected();
  uint32_t millisNow = millis();
  if( status == BT_CONNECTING ){
    initStep = 0;
    if( (millisNow - startMillis) >= BT_CONNECT_TIMEOUT ){
      startMillis = millisNow;
      hid_l2cap_reconnect();
    }
  }
  else if( status == BT_DISCONNECTED ){
    initStep = 0;
    startMillis = millisNow;
    hid_l2cap_reconnect();
  }
  else if ( status == BT_CONNECTED ) { 
    if (initStep % 2 == 0) {
      OnConnect();
    }
  }
  delay(1);
}

