
#include "settings.h"
#include <ArduinoOSCWiFi.h>
#ifndef _VMTOSC_H_
#define _VMTOSC_H_

namespace VMTOSC {
  #define OSCPATH_VMT_ROOM_UNITY "/VMT/Room/Unity"
  #define OSCPATH_VMT_BUTTON "/VMT/Input/Button"
  #define OSCPATH_VMT_TRIGGER "/VMT/Input/Trigger"
  #define OSCPATH_VMT_STICK "/VMT/Input/Joystick"
  #define OSCPATH_VMT_STICK_CLICK "/VMT/Input/Joystick/Click"

  inline void SendVmtEnableOSC() {
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_ROOM_UNITY, VMT_INDEX, VMT_ENABLE, 0.0, 
      0.0, 0.0, 0.0,        //position xyz
      0.0, 0.0, 0.0, 1.0 ); //quaternion xyzw
        Serial.printf("[%s] called\n", __func__ );

  }

  // buttonIndex: 0 = System, 1 = A, 3 = B
  inline void SendVmtButtonOsc( int buttonIndex, bool pressed ) {
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_BUTTON, (int)VMT_INDEX, (int)buttonIndex, (float)0.0, (int)(pressed ? 1:0));
        Serial.printf("[%s] called\n", __func__ );
  }

  // triggerIndex 0 = trigger, 1 = grip.  floatValue 0.0 to 1.0
  inline void SendVmtTriggerOsc( int triggerIndex, float floatValue ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_TRIGGER, (int)VMT_INDEX, (int)triggerIndex, (float)0.0, floatValue);
        Serial.printf("[%s] called: float: %f\n", __func__ , floatValue );
  }

  // float x, float y: -1.0 to 1.0. 
  inline void SendVmtStickXYOsc( float x, float y ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_STICK, (int)VMT_INDEX, (int)1, (float)0.0, x, y);
        Serial.printf("[%s] called (%f, %f)\n", __func__,x,y );
  }

  inline void SendVmtStickClickOsc( bool clicked ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_STICK_CLICK, (int)VMT_INDEX, (int)1, (float)0.0, clicked ? 1:0);
        Serial.printf("[%s] called %d\n", __func__ , clicked ? 1:0);
  }
}


#endif

