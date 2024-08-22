#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// 1. 必須：右手の場合はコメント、左手ならコメント解除。
#define LEFT_HAND

// 2. 必須・要更新：JoyCons' Bluetooth device addresses
#ifdef LEFT_HAND
#define TARGET_BT_ADDR  { 0x90, 0x45, 0x28, 0x7c, 0xXX, 0xXX } // 左手JoyCon のBluetoothアドレス
#else
#define TARGET_BT_ADDR  { 0x90, 0x45, 0x28, 0x7d, 0xXX, 0xXX } // 右手JoyCon のBluetoothアドレス
#endif

// 3-1. VRChatにOSC Inputを送信 / VMT に OSCを送信
// 使う場合は必須： VRC_OSCかVMT_OSC、使う方どちらか一方だけをコメント解除してください。
// 両方コメントでもシリアルに押されたボタン等を出力するサンプルとして使えます。
//#define SEND_VRC_OSC // VRChat の OSC Inputを送信する。
//#define SEND_VMT_OSC // VMTの Index互換コントローラーinputをOSC送信する。

// 3-2. 使う場合は必須： OSC送信先IPアドレスを指定してください。
#define VRC_OSC_IPADDR "192.168.1.xxx" // VRChat が動作するPCのIPアドレス
#define VMT_OSC_IPADDR "192.168.1.xxx" // VMT が動作するPCのIPアドレス

//////////////////////////////////////////////////////////////////////
// 以下は通常変更する必要はありません
// 4.WiFi Manager : 変更しなくても使えます
#ifdef LEFT_HAND
#define WIFIMAN_SSID "ESP32AP_LEFT"  // SSID
#else
#define WIFIMAN_SSID "ESP32AP_RIGHT"  // SSID
#endif
#define WIFIMAN_PASS "12341234"      // password

// 5. VRChat OSC Input 送信先ポート（通常そのままで）
#define VRC_OSC_PORT 9000

// 6. VMT (Virtual Motion Tracker) 関連（通常そのままで）
#define VMT_OSC_PORT 39570
#ifdef LEFT_HAND
    #define VMT_INDEX 1
    #define VMT_ENABLE 5
#else
    #define VMT_INDEX 2
    #define VMT_ENABLE 6
#endif


#endif

