#ifndef MOS6581_h
#define MOS6581_h

#include "Arduino.h"
#include <SPI.h>
#include "sid_registers.h"

// #define SID_DEBUG

#define SID_NOISE 		128
#define SID_SQUARE		64
#define SID_RAMP		32
#define SID_TRIANGLE		16
#define SID_TEST		8
#define SID_RING		4
#define SID_SYNC		2

#define SID_3OFF		128
#define SID_FILT_HP		64
#define SID_FILT_BP		32
#define SID_FILT_LP		16

#define SID_FILT_VOICE1	1
#define SID_FILT_VOICE2 2
#define SID_FILT_VOICE3 4
#define SID_FILT_EXT	8

class MOS6581
{
	byte ss, cs, clk;

	// create some internal registers for binary manipulation
	byte voice1_register;
	byte voice2_register;
	byte voice3_register;
	byte filter_register;
	byte mode_register;

	
	void startClock(void);
	void initialise(void);
	void voiceFrequency(byte low, byte high, word frequency);
	void voicePulseWidth(byte low, byte high, word frequency);

public:
	MOS6581(void);
	MOS6581(byte shiftPin, byte sidPin, byte clockPin);
	void reset(void);
	void transfer(byte address, byte value);
	void voiceOneOn();
	void voiceTwoOn();
	void voiceThreeOn();

	void voiceOneOff();
	void voiceTwoOff();
	void voiceThreeOff();

	void voiceOneFilterOn();
	void voiceTwoFilterOn();
	void voiceThreeFilterOn();

	void voiceOneFilterOff();
	void voiceTwoFilterOff();
	void voiceThreeFilterOff();

	// fundamental frequency of waveform generator. 16bit number.
	void voiceOneFrequency(word frequency);
	void voiceTwoFrequency(word frequency);
	void voiceThreeFrequency(word frequency);

	// duty cycle of square waves. 12bit number
	void voiceOnePulseWidthFrequency(word frequency);
	void voiceTwoPulseWidthFrequency(word frequency);
	void voiceThreePulseWidthFrequency(word frequency);

	void voiceOneMode(byte mode);
	void voiceTwoMode(byte mode);
	void voiceThreeMode(byte mode);

	void voiceOneEnvelope(byte attack, byte decay, byte sustain, byte release);
	void voiceTwoEnvelope(byte attack, byte decay, byte sustain, byte release);
	void voiceThreeEnvelope(byte attack, byte decay, byte sustain, byte release);

	// filter volume and output
	void volume(byte value);
	void filterOff();
	void filterLP();
	void filterHP();
	void filterBP();
	void filterNotch();

	// set the filter frequency. 11bit number
	void filterFrequency(word frequency);
	void filterResonance(byte resonance);



};


#endif
