# いおな三四郎 JVS - Playstation 1/2 Gamepad I/O Bridge Board

This program makes an Arduino Uno/Nano work as a JVS I/O Board that allows you
to connect a Playstation 1/2 Gamepad to JVS compatible Arcade PCBs (Tested on Taito Type X)

Only the digital buttons are used, so no analog stick mapping yet.

```
  JVS SWs     Gamepad
 1P Start  - Start
 1P Coin   - Select
 1P Up     - Pad Up
 1P Down   - Pad Down
 1P Left   - Pad Left
 1P Right  - Pad Right
 1P Push 1 - X
 1P Push 2 - O
 1P Push 3 - ∆
 1P Push 4 - □
 1P Push 5 - L1
 1P Push 6 - R1
 1P Push 7 - L2
 1P Push 8 - R2
```

## Schematic
```

Arduino Nano/Uno

                         o TX1 (D1 on Uno)  VIN o
JVS Data+ o--------------o RX0 (D0 on Uno)  GND o
                         o RST              RST o
                         o GND               5V o--------------o 5V
JVS Data- o--------------o D2                A7 o
JVS Sense o---o---VVVV---o D3                A6 o
              |  100 Ohm
       100 nF =
              |
            -----
            /////
          o--------------o D4    A5 o--------------o 
          o--------------o D5    A4 o--------------o 
          o--------------o D6    A3 o--------------o 
          o--------------o D7    A2 o--------------o 
          o--------------o D8    A1 o--------------o 
PSX Data  o--------------o D9    A0 o--------------o 
PSX Cmd   o--------------o D10 AREF o--------------o
PSX Att   o--------------o D11  3V3 o
PSX Clock o--------------o D12  D13 o--------------o LED on Nano

Connect PSX GND to Arduino GND.
PSX VCC to 3.3v/5v (Both work fine from my testing, the specifications for PlayStation say 3.3v however)

JVS - Series B connector (same with USB Type B)
Note: Official JVS spec explains that the pin 3 and 4 are swapped, but it is wrong. All pins layout is same with USB.
  ________
 /        \
 | 1#  #2 |  1 Sense
 |  +--+  |  2 Data-
 |  +--+  |  3 Data+
 | 4#  #3 |  4 GND
 +--------+

PlayStation Controller Connector:

 _________________________________
|  #1 #2 #3   #4 #5 #6   #7 #8 #9 |
 \_______________________________/
1 Data
2 Cmd
3 DualShock Vibration Motor Power (unused)
4 GND
5 3.3v
6 Att
7 Clk
8 Unknown (Unused)
9 Ack (Unused)

```
[Pinout obtained from Curious Inventor (https://store.curiousinventor.com/guides/PS2)](https://store.curiousinventor.com/guides/PS2)
