
#include <Arduino.h>
#include "hid_l2cap.h"
#include "joycon_hid.h"

namespace JoyconHid {
  // Sending subcommand as an output 0x01 report, with dummy rumble data, via hid interrupt channel.
  void SendSubcommand( uint8_t subcmd, uint8_t* data, uint8_t len)
  {
    static uint8_t globalNum = 0;
    hid_cmd_t xhid_cmd_t;

    uint16_t maxlen = sizeof(xhid_cmd_t.data);
    uint16_t l2cap_len = 1 + 8 + 1 + len; // globalNum(1) + rumbleData(8) + subcommand(1)
    if ( maxlen < l2cap_len ) { 
      // Size of JoyCon subcommand's argument is 38 bytes or less, as far as I know.
      // hid_cmd_t has a buffer enough to contain 48-byte data.
      Serial.printf("[%s] Too long arguments of subcommand (length = %d). Max length is %d. Stop sending.\n", __func__, len, maxlen-10 );
    }

    memset( &xhid_cmd_t, 0, sizeof(xhid_cmd_t));
    globalNum++;
    if ( globalNum >= 0x10 ) globalNum = 0;

    xhid_cmd_t.code = 0xa2;
    xhid_cmd_t.identifier = 0x1;
    xhid_cmd_t.data[0] = globalNum;

    const uint8_t dummyRumble[8] = { 0x0, 0x1, 0x40, 0x40, 0x0, 0x1, 0x40, 0x40 };
    memcpy( xhid_cmd_t.data+1, dummyRumble, 8);

    xhid_cmd_t.data[9] = subcmd;

    if (data == 0) len = 0;
    if (len > 0)  memcpy( (xhid_cmd_t.data) +10, data, len );
    hid_l2cap_send(true, &xhid_cmd_t, l2cap_len); // 1st param "true" means sending to device's interrupt channel 
  }

  void SendSubcommandOneArg( uint8_t subcmd, uint8_t arg)
  {
    uint8_t data[1] = { arg };
    SendSubcommand( subcmd, data, 1 );
  }

  // parameter: playerNumber (1-4)
  void SetPlayerLEDByNumber(uint8_t playerNumber) 
  {
    if ( playerNumber < 1 ) playerNumber = 1;
    if ( playerNumber > 4 ) playerNumber = 4;
    uint8_t index = playerNumber -1;
    SetPlayerLights( 1 << index );
  }
}
