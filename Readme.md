# ESP32 as a JoyCon's host
ESP32がBluetoothのホストとなってJoyConからボタン等の情報を取得できるプログラムです。
取得した内容に基づいて処理を分岐したり、取得した情報をWiFiでPC等に送ったりできます。
PCにJoyConを接続すると毎回ペアリングからで面倒なので、PCに転送するだけでも便利な場合もあるかと思います。

(JoyConへ Subcommand を送る機能はまだ不安定ですが、JoyCon から情報を取得する方は安定しています。)

## サンプルプログラム
サンプルとして OSCで VRChat、あるいはVMT(Virtual Motion Tracker)が動いているPCにスティックやボタンの情報を送信して操作できるプログラム(src/main.cpp)をつけてあります。
VSCode の PlatformIO で Arduino ベースで書いていますので、src/settings.h の内容を編集して使ってください。
ライブラリは platformio.ini の lib_depsに書いてあるものが必要です。

サンプルは WiFiManager を使っているので、WiFi接続用のSSID/Passが保存されていないESP32の場合は、ESP32が自らアクセスポイントになります。src/settings.h で設定してある一時的なSSIDとPASSで接続できますので、スマホで接続して家のWiFiに繋ぐための設定してください。

## JoyCon の Bluetooth アドレスの調べ方
サンプルプログラムの src/settings.h には対象とする JoyCon の Bluetoothアドレスを記述する必要があります。

Bluetoothアドレス(Bluetoothデバイスアドレス)は、6バイト(48bits)の数値で示されるアドレスです。通常16進数で表記されます。<br>
ネットワークデバイスのMACアドレスと同じ桁数なので、MACアドレスと書かれている場合がありますが、本来は別物です。

PCにいったん接続して、デバイスマネージャーでBluetoothグループの中にあるデバイスのプロパティを表示して、詳細タブで「Bluetooth デバイスアドレス」を選択すると表示されます。
右手、左手それぞれ確認して、2桁(1バイト)ずつ区切って 0xXX を6個書く様式でsrc/settings.h に記述してください。

例：左手 JoyCon (L) のBluetoothアドレスが 10203040AABBの場合の書き方

`#define TARGET_BT_ADDR  { 0x10, 0x20, 0x30, 0x40, 0xAA, 0xBB } // 左手JoyCon のBluetoothアドレス`

スマホのBluetoothスキャナアプリ(Bluetooth Classic対応のもの)などでも調べられます。(10:20:30:40:AA:BB のようにコロンで区切られている場合も多いです。)

## 注意事項
1. Bluetooth Classic のホストとして機能する必要があるので ESP32(無印)である必要があります。<br>ESP32C3やESP32S3などの後継はBLE (Bluetooth Low Energy)しか対応していないため、使えません。
2. 1つのESP32で左右一方しか対応していません。両手用に2つ使ってください。（両対応できるのかな？）
3. 容量が大きくなるのでパーティションは no_ota になっています。（WiFiManagerをやめれば収まるかも）

なお、ESP32 DevKitC などにUSBで給電すれば、それ以外の配線等は不要です。

## 参考にしたサイト等
### Bluetooth Classic, HID, L2CAP 関連
1. ESP32にBluetooth3.0のキーボードをつなげる<br>
https://qiita.com/poruruba/items/a90c117fb13e23b55c5f<br>
　input 0x3f report の受信だけならここのコードだけでいけました。

2. ESP32-ps3<br>
https://github.com/jvpernis/esp32-ps3<br>
　上記サイトの参照元ですが、output report を送信するためのコードが上記サイトでは移植されてなかったので、ps3_l2cap.c の ps3_l2cap_send_hid を参照しました。

3. L2CAP（プロトコル）によるチャンネルの確保<br>
http://www.yts.rdy.jp/pic/GB002/l2cap.htm<br>
　前述 ps3_l2cap_send_hid が読んでいる L2CA_DataWrite の パラメーターのとなる BT_HDR 構造体、その手前の hid_cmd_t 構造体の設定方法がわからずにいたので、
    - output を送るには interrupt channel を使って、
    - code は 0xA1が input reportで、 0xA2 が output reportで…
    - identifier はその次の byte だから…

    的なところが参考になりました。<br>
（Wiiリモコン前提のサイトですが、使う数値はともかくやることは一緒）

### JoyCon 操作関連
4. Nintendo_Switch_Reverse_Engineering<br>
https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering<br>
　特に input/output report とか subcommands とかのパート<br>
　JoyConで使う数値はここを見ないとわからないです。<br>
　英語なので、まずは日本語である程度把握したいという方は次の5のサイトを先に見ると良いでしょう。<br>

5. Joy-ConにPythonからBluetooth接続をして6軸センサーと入力情報を取得する<br>
https://qiita.com/tocoteron/items/9a5d81c8f640ecaff7a9<br>
　4のサイトの内容を抜粋・開設しつつ、具体的なプログラム例(python)が示されており、Subcommandの送り方等が参考になります。<br>
 `JoyconHid::SendSubcommand` の Rumbleのダミーデータはこちらのサイトを参考にしました。

## その他
こちらで公開しているプログラム・ソースコードは無保証です。
ライセンスは（ソースコードに記載されているものを除き）特に定められていません(参考元でも定められていないため)が、将来、参考元で変更あった場合はこちらでも変更される可能性があります。
