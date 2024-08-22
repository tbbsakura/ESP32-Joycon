
#include <Arduino.h>
#include "hid_l2cap.h"

// For details on Joycon's subcommands, refer to the webpage
// https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/blob/master/bluetooth_hid_subcommands_notes.md

namespace JoyconHid {
    // Sending subcommand as an output 0x01 report, "with" dummy rumble data, via hid interrupt channel.
    void SendSubcommand( uint8_t subcmd, uint8_t* data, uint8_t len);

    // Sending subcommand with 1 byte argument
    void SendSubcommandOneArg( uint8_t subcmd, uint8_t arg);

    // Subcommand 0x00: Get Controller State
    inline void GetControllerState() { SendSubcommand( 0x00, 0, 0 ); }

    //  Subcommand 0x02: Request device info
    inline void RequestDeviceInfo() { SendSubcommand( 0x02, 0, 0 ); }

    // Subcommand 0x30: Set player lights
    inline void SetPlayerLights(uint8_t dat) { SendSubcommandOneArg(0x30, dat); }

    // parameter: index (1-4)
    void SetPlayerLEDByNumber(uint8_t playerNumber); 

    // Subcommand 0x03: Set input report mode. Default parameter is 0x3f
    inline void SetInputReportMode(uint8_t mode = 0x3f) { SendSubcommandOneArg(0x03, mode); }  

    // Subcommand 0x40: Enable/Disable IMU (default: enabling = true)
    inline void EnableIMU(bool enabling = true) { SendSubcommandOneArg(0x40,  (uint8_t)(enabling ? 0x01 : 0x00)); }  

    // Disable IMU, using Subcommand 0x40
    inline void DisableIMU() { EnableIMU(false); }
}
