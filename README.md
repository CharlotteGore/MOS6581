# MOS6581

C++ library for controlling a MOS 6581/8590 via SPI on Arduino. Rather than requiring a MIDI in signal, this library is about putting direct and full control of the SID's 24 writeable registers into the programmer's hands in a fairly friendly API.

## Basics

The Arduino is connected to 2 74HC595N shift registers. The first register (with the data in pin connected to the MOSI pin on the Arduino) is connected to pins Q0->D0, Q1->D1 ... Q7->D7 on the 6581/8590. The Q7' pin connects to the second 74HC595, which connects Q0->A0, Q1->A1 .. Q4->A4 on the 6581/8590.

Pin 9 on the Arduino is connected to the SID Clock input pin if you're using a Nano/Uno. It's actually Timer 1, so I believe it's Pin 11 on the Mega instead, but this is untested.

Pin 4 on the Arduino is connected to the SID CS (Chip Select) pin.

Pin 7 on the Arduino is connected to both shift register's latch pins.

Pin 11 is connected to the first shift register's data input pin.

Pin 13 is connected to both shift register's clock input pin.

The following code is about the minimum required to get a SID to make a noise - any noise. Set the volume, set an envelope, set a waveform and turn a channel on, and set a frequency. This software makes that trivial. 

```c++
MOS6581 sid;

void setup(){

    sid = MOS6581();
    // or if you need to change pin assignments...
    // sid = MOS6581(SHIFT_REG_LATCH_PIN, SID_CHIP_SELECT_PIN, SID_CLOCK_PIN)

   sid.reset();
   sid.volume();

   sid.voiceOneMode( SID_RAMP );
   sid.voiceOneEnvelope(0,0,15,0);
   sid.voiceOneFrequency(1600);

   sid.voiceOneOn();

}
```

## Using/Compiling

I've only been able to get this library to work with the Visual Studio Arduino plugin. It doesn't seem to work with the official Arduino IDE. It's possible I've just missed something simple - haven't done C++ or an Arduino library before.

## API

## Global Controls

### .reset()

Populates all the registers with 0.

### .volume(byte vol)

Set the global output volume. 0 - 15. 

## Individual voice controls

A number between 0 and 15 to set the master output volume.

### .voiceOneMode( byte mode ) 
### .voiceTwoMode( byte mode )
### .voiceThreeMode( byte mode )

Select a waveform for a voice. Options are SID_SQUARE, SID_RAMP, SID_TRIANGLE, SID_TEST, SID_RING, SID_SYNC. You can use binary maths to select multiple waveform generators but this isn't recommended.

### .voiceOneFrequency(word frequency) 
### .voiceTwoFrequency(word frequency) 
### .voiceThreeFrequency(word frequency) 

Set a frequency for a voice. Frequency is a 16bit number, 0-65535. TO DO: Lookup for real notes->frequency. 

### .voiceOnePulseWidthFrequency(word frequency) 
### .voiceTwoPulseWidthFrequency(word frequency) 
### .voiceThreePulseWidthFrequency(word frequency) 

Set a frequency for the Square wave duty cycle. No effect unless SID_SQUARE is selected as a voice's mode. Frequency is a 12 bit number, 0-4095.

### .voiceOneEnvelope(byte attack, byte decay, byte sustain, byte release)
### .voiceTwoEnvelope(byte attack, byte decay, byte sustain, byte release)
### .voiceThreeEnvelope(byte attack, byte decay, byte sustain, byte release)

Set the amplitude modulation envelope of a voice. Each parameter is a number between 0-15. 

To simply play and hold a note, indefinitely, the correct setting is 0,0,15,0.

### .voiceOneOn()
### .voiceTwoOn()
### .voiceThreeOn()

Turn on a voice. 

### .voiceOneOff()
### .voiceTwoOff()
### .voiceThreeOff()

Turn off a voice

### .voiceOneFilterOn()
### .voiceTwoFilterOn()
### .voiceThreeFilterOn()

Put the voice through the filter. 

### .voiceOneFilterOff()
### .voiceTwoFilterOff()
### .voiceThreeFilterOff()

Stop a voice going through the filter.

## Filter Controls

### .filterLP()

Engage low pass mode on the filter

### .filterBP()

Engage band pass mode on the filter

### .filterHP()

Engage high pass mode on the filter

### .filterNotch()

Engage high and low pass modes to make a Notch filter.

### .filterFrequency(word frequency)

Set the filter cutoff frequency. This is a 10 bit number, 0 - 1023

### .filterResonance(byte resonance)

Set the resonance amount. This is a 4 bit number, 0-15.


## Just in Case

### .transfer(byte address, byte value)

Write whatever you want directly to a SID register, if you know what you're doing. Generally the API exposes everything except the read-only registers so perhaps this might be useful for that. 

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



