
#include "settings.h"
#include <ArduinoOSCWiFi.h>
#ifndef _VRCOSC_H_
#define _VRCOSC_H_

namespace VMTOSC {
  #define OSCPATH_VMT_ROOM_UNITY "/VMT/Room/Unity"
  #define OSCPATH_VMT_BUTTON "/VMT/Input/Button"
  #define OSCPATH_VMT_TRIGGER "/VMT/Input/Trigger"
  #define OSCPATH_VMT_STICK "/VMT/Input/Joystick"
  #define OSCPATH_VMT_STICK_CLICK "/VMT/Input/Joystick/Click"

  inline void SendVmtEnableOSC() {
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_ROOM_UNITY, VMT_INDEX, VMT_ENABLE, 0.0, 
      0.0, 0.0, 0.30,        //position xyz
      0.0, 0.0, 0.0, 1.0 ); //quaternion xyzw
  }

  // buttonIndex: 0 = System, 1 = A, 3 = B
  inline void SendVmtButtonOsc( int buttonIndex, bool pressed ) {
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_BUTTON, VMT_INDEX, buttonIndex, 0.0, pressed ? 1:0);
  }

  // triggerIndex 0 = trigger, 1 = grip.  floatValue 0.0 to 1.0
  inline void SendVmtTriggerOsc( int triggerIndex, float floatValue ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_TRIGGER, VMT_INDEX, triggerIndex, 0.0, floatValue);
  }

  // float x, float y: -1.0 to 1.0. 
  inline void SendVmtStickXYOsc( float x, float y ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_STICK, VMT_INDEX, 1, 0.0, x, y);
  }

  void SendVmtStickClickOsc( bool clicked ) {
    // joystickIndex: 1 = thumbStick
    OscWiFi.send(VMT_OSC_IPADDR, VMT_OSC_PORT, OSCPATH_VMT_STICK_CLICK, VMT_INDEX, 1, 0.0, clicked ? 1:0);
  }
}

namespace VRCOSC {
  inline void OSCSendBool(const char* path, bool boolValue) {
    OscWiFi.send(VRC_OSC_IPADDR, VRC_OSC_PORT, path, boolValue ? 1 : 0);
  }

  inline void OSCSendInt(const char* path, uint8_t intValue) {
    OscWiFi.send(VRC_OSC_IPADDR, VRC_OSC_PORT, path, intValue);
  }

  inline void OSCSendFloat(const char* path, float_t floatValue) {
    OscWiFi.send(VRC_OSC_IPADDR, VRC_OSC_PORT, path, floatValue);
  }

  // See also https://docs.vrchat.com/docs/osc-as-input-controller

  // Each AXIS is used with a float in the range from -1 to +1
  #define OSCPATH_AXIS_VERTICAL	"/input/Vertical" //Move forwards (1) or Backwards (-1)
  #define OSCPATH_AXIS_HORIZONTAL	"/input/Horizontal" //Move right (1) or left (-1)
  #define OSCPATH_AXIS_LOOK_HORIZONTAL	"/input/LookHorizontal" //Look Left and Right. Smooth in Desktop, VR will do a snap-turn when the value is 1 if Comfort Turning is on.
  //#define OSCPATH_AXIS_USE_AXIS_RIGHT	"/input/UseAxisRight" //Use held item - not sure if this works
  //#define OSCPATH_AXIS_GRAB_AXIS_RIGHT	"/input/GrabAxisRight" //Grab item - not sure if this works
  #define OSCPATH_AXIS_MOVE_HOLD_FB	"/input/MoveHoldFB" //Move a held object forwards (1) and backwards (-1)
  #define OSCPATH_AXIS_SPIN_HOLD_CWCCW	"/input/SpinHoldCwCcw" //Spin a held object Clockwise or Counter-Clockwise
  #define OSCPATH_AXIS_SPIN_HOLD_UD	"/input/SpinHoldUD" //Spin a held object Up or Down
  #define OSCPATH_AXIS_SPIN_HOLD_LR	"/input/SpinHoldLR" //Spin a held object Left or Right

  // Each BUTTON is used with an integer of 0 or 1
  #define OSCPATH_BUTTON_MOVE_FORWARD "/input/MoveForward" // Move forward while this is 1.
  #define OSCPATH_BUTTON_MOVE_BACKWARD "/input/MoveBackward" // Move backwards while this is 1.
  #define OSCPATH_BUTTON_MOVE_LEFT "/input/MoveLeft" // Strafe left while this is 1.
  #define OSCPATH_BUTTON_MOVE_RIGHT "/input/MoveRight" // Strafe right while this is 1.
  #define OSCPATH_BUTTON_LOOK_LEFT "/input/LookLeft" // Turn to the left while this is 1. Smooth in Desktop, VR will do a snap-turn if Comfort Turning is on.
  #define OSCPATH_BUTTON_LOOK_RIGHT "/input/LookRight" // Turn to the right while this is 1. Smooth in Desktop, VR will do a snap-turn if Comfort Turning is on.
  #define OSCPATH_BUTTON_JUMP "/input/Jump" // Jump if the world supports it.
  #define OSCPATH_BUTTON_RUN "/input/Run" // Walk faster if the world supports it.
  #define OSCPATH_BUTTON_COMFORTLEFT "/input/ComfortLeft" //Snap-Turn to the left - VR Only.
  #define OSCPATH_BUTTON_COMFORTRIGHT "/input/ComfortRight" //Snap-Turn to the right - VR Only.
  #define OSCPATH_BUTTON_DROPRIGHT "/input/DropRight" //Drop the item held in your right hand - VR Only.
  #define OSCPATH_BUTTON_USERIGHT "/input/UseRight" //Use the item highlighted by your right hand - VR Only.
  #define OSCPATH_BUTTON_GRABRIGHT "/input/GrabRight" //Grab the item highlighted by your right hand - VR Only.
  #define OSCPATH_BUTTON_DROPLEFT "/input/DropLeft" //Drop the item held in your left hand - VR Only.
  #define OSCPATH_BUTTON_USELEFT "/input/UseLeft" //Use the item highlighted by your left hand - VR Only.
  #define OSCPATH_BUTTON_GRABLEFT "/input/GrabLeft" //Grab the item highlighted by your left hand - VR Only.
  #define OSCPATH_BUTTON_PANICBUTTON "/input/PanicButton" //  Turn on Safe Mode.
  #define OSCPATH_BUTTON_QUICK_MENU_TOGGLE_LEFT "/input/QuickMenuToggleLeft" // Toggle QuickMenu On/Off. Will toggle upon receiving '1' if it's currently '0'.
  #define OSCPATH_BUTTON_QUICK_MENU_TOGGLE_RIGHT "/input/QuickMenuToggleRight" //  Toggle QuickMenu On/Off. Will toggle upon receiving '1' if it's currently '0'.
  #define OSCPATH_BUTTON_VOICE "/input/Voice" //Toggle Voice - the action will depend on whether "Toggle Voice" is turned on in your Settings

  #ifdef LEFT_HAND
      #define OSCPATH_BUTTONA OSCPATH_BUTTON_VOICE
      #define OSCPATH_BUTTONB OSCPATH_BUTTON_QUICK_MENU_TOGGLE_LEFT
  #else
      #define OSCPATH_BUTTONA OSCPATH_BUTTON_JUMP
      #define OSCPATH_BUTTONB OSCPATH_BUTTON_QUICK_MENU_TOGGLE_RIGHT
  #endif
}

#endif

