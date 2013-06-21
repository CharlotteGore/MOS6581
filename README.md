# MOS6581

C++(ish) library for controlling a MOS 6581/8580 via SPI on Arduino. Rather than requiring a MIDI in signal, this library is about putting direct and full control of the SID's 24 writeable registers into the programmer's hands in a fairly friendly API.

There's some random schematic related junk in the repo that probably isn't much use. It's not compatible with this code (you need to reverse the bit order on the SPI interface first and reactivate the clock generator code (or supply a 1mhz clock signal from somewhere else)) and may or may not be up to date anyway. Use at your own risk.

I've recently refactored this library to have a less fluent API but certaintly a more practical one in terms of integrating into embedded systems.

## Basics

This library programs the SID via the hardware SPI on an Arduino, and uses pin 2 as the SID chip select and pin 3 as the shift register latch. NOTE that the schematic isn't compatible with this iteration of the code. The code is based on the breadboard circuit I'm currently using, not the PCB which I haven't actually got in my hands yet. I will update the code once I move to the PCB. 

The following code is about the minimum required to get a SID to make a noise - any noise. Set the volume, set an envelope, set a waveform and turn a channel on, and set a frequency. This software makes that trivial. 

```c++
MOS6581 sid;

void setup(){

    sid = MOS6581();
    // or if you need to change pin assignments...
    // sid = MOS6581(SHIFT_REG_LATCH_PIN, SID_CHIP_SELECT_PIN, SID_CLOCK_PIN)

   sid.reset();
   sid.volume(15); // set volume to the maximum, 15.

   sid.setMode(0, SID_RAMP); //set voice 0 to a ramp waveform
   sid.setADEnvelope(0,0,0); //Set voice 0's Attack and Decay envelope
   sid.setSREnvelope(0,15,0); //Set voice 0's Sustain and Release envelope
   sid.setFrequency(0, 1600); //Set voice 0's frequency
  
   sid.setVoice(0,1); Set voice 0 to 'on'.

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

### .setMode(byte voice, byte mode)

Select a waveform for a voice. Options are SID_SQUARE, SID_RAMP, SID_TRIANGLE, SID_TEST, SID_RING, SID_SYNC. You can use binary maths to select multiple waveform generators but this isn't recommended.

### .setFrequency(byte voice, byte mode)

Set a frequency for a voice. Frequency is a 16bit number, 0-65535. TO DO: Lookup for real notes->frequency. 

### .setPulseWidth(byte voice, word frequency)

Set a frequency for the Square wave duty cycle. No effect unless SID_SQUARE is selected as a voice's mode. Frequency is a 12 bit number, 0-4095.

### .setADEnvelope(byte voice, byte attack, byte decay)
### .setSREnvelope(byte voice, byte sustain, byte release)

Set the amplitude modulation envelope of a voice. Now split into two seperate functions. Each parameter is a number between 0-15. 

To simply play and hold a note, indefinitely, the correct setting is 0,0,15,0.

### .setVoice(byte voice, boolean on)

Turn a voice on or off. Without a valid frequency, waveform and envelope this might not do anythign noticable.  

### .setFilter(byte voice, boolean on)

Set a voice to either go or not go through the filter.

## Filter Controls

### .setFilterMode(byte mode)

Set the filter's mode. Valid options are SID_FILT_LP, SID_FILT_HP, SID_FILT_LP and SID_FILT_OFF (which mutes any voices going through it) you can logical OR them together, i.e, ```SID_FILT_LP | SID_FILT_HP``` to create a notch filter.  

### .filterFrequency(word frequency)

Set the filter cutoff frequency. This is a 10 bit number, 0 - 1023

### .filterResonance(byte resonance)

Set the resonance amount. This is a 4 bit number, 0-15.

## Just in Case

### .transfer(byte address, byte value)

Write whatever you want directly to a SID register, if you know what you're doing. Generally the API exposes everything except the read-only registers so perhaps this might be useful for that. 

## Project updates and things

Current setup: SID/Arduino Interface board with another breadboard with an Arduino Nano, MIDI in circuit, a 1mhz Oscillator and two 4051 demux chips for handling all the analog inputs on the control surface.

![A SID Based synth](https://github.com/CharlotteGore/MOS6581/raw/master/with-pcb.jpg)

Here's the ridiculous hot pink control surface I made. It doesn't quite have enough controls. I need at least 2 more toggle switches... whoops. 

![Control surface for the SID](https://github.com/CharlotteGore/MOS6581/raw/master/plexidreams.jpg)

This was the first working breadboard I set up - just enough to control a SID with pure code and output a sound.

![A SID MOS 6581/MOS 8580 being controlled by an Arduino Nano](https://github.com/CharlotteGore/MOS6581/raw/master/Breadboard-prototype.jpg)

Here's a friend playing on the SID. It demonstrates multiple voices being used, along with the filter.
[Sound sample](https://github.com/CharlotteGore/MOS6581/raw/master/multi-voice-random.mp3)

Some of the first noises I got out of a SID. This one was a complete mess. 
[Sound sample](https://github.com/CharlotteGore/MOS6581/raw/master/sid-test.mp3)

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



