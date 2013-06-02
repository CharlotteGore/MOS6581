# MOS6581

C++ library for controlling a MOS 6581/8590 via SPI on Arduino

## Basics

The Arduino is connected to 2 74HC595N shift registers. The first register (with the data in pin connected to the MOSI pin on the Arduino) is connected to pins Q0->D0, Q1->D1 ... Q7->D7 on the 6581/8590. The Q7' pin connects to the second 74HC595, which connects Q0->A0, Q1->A1 .. Q4->A4 on the 6581/8590.

Pin 9 on the Arduino is connected to the SID Clock input pin if you're using a Nano/Uno. It's actually Timer 1, so I believe it's Pin 11 on the Mega instead, but this is untested.

Pin 4 on the Arduino is connected to the SID CS (Chip Select) pin.

Pin 7 on the Arduino is connected to both shift register's latch pins.

Pin 11 is connected to the first shift register's data input pin.

Pin 13 is connected to both shift register's clock input pin.

```c++
MOS6581 sid;

void setup(){

    sid = MOS6581();
    // or if you need to change pin assignments...
    // sid = MOS6581(SHIFT_REG_LATCH_PIN, SID_CHIP_SELECT_PIN, SID_CLOCK_PIN)

   sid.reset();
   sid.volume();

}
```

## DISCLAIMER OF DOOM

Copyright (c) 2013 Charlotte Gore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



