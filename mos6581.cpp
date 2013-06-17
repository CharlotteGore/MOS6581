#include "mos6581.h"
#include <SPI.h>
#include "sid_registers.h"


MOS6581::MOS6581(void){
	ss = 3;
	cs = 2;
	//clk = 9;
	initialise();
	//startClock(); // Now using external crystal oscillator
}

MOS6581::MOS6581(byte shiftPin, byte sidPin, byte clockPin){
	ss = shiftPin;
	cs = sidPin;
	//                                                                                                                                                                                                             clk = clockPin;
	initialise();
	//startClock(); // now using external crystal oscillator
}

void MOS6581::initialise(){
	pinMode(ss, OUTPUT);
	pinMode(cs, OUTPUT);

	voice1_register = B00100000;
	voice2_register = B00100000;
	voice3_register = B00100000;

	filter_register = B00000000;
	mode_register = B00000000;

	// set the latch pins to their correct default states
	digitalWrite(cs, HIGH);
	digitalWrite(ss, LOW);
	                                                                                                                                                                                                                                      
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.begin();
}

void MOS6581::reset(){
	// iterate through all the registers and reset them
	for(int i = 0; i < 25; i++){
		transfer(i, 0);
	}
}

void MOS6581::startClock(){
	// set the clock pin as HIGH. PIN 9 on the Uno, Pin 11 on the Mega
	pinMode(clk, OUTPUT);

	// generate a 1mhz clock pulse on Timer1 pins
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 7;
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10);
}

void MOS6581::transfer(byte address, byte value){

	// turn off shift register output..
	digitalWrite(ss, LOW);
	// transfer the two bytes of data
	SPI.transfer(address);
	SPI.transfer(value);
	// turn on the shift register output
	digitalWrite(ss, HIGH);
	// wait a small amount of time for the shift register latch data in
	delayMicroseconds(2);
	// enable write mode on the SID
	digitalWrite(cs, LOW);
	// 2 microseconds should be enough for a single clock pulse to get through
	delayMicroseconds(2);
	// disable write mode on the SID
	digitalWrite(cs, HIGH);

}

void MOS6581::voiceFrequency(byte lowAddress, byte highAddress, word frequency){
	byte low = lowByte(frequency);
	byte high = highByte(frequency);
	transfer(lowAddress, low);
	transfer(highAddress, high);

}

void MOS6581::voicePulseWidth(byte lowAddress, byte highAddress, word frequency){
	byte low = lowByte(frequency);
	byte high = highByte(frequency) & B00001111;
	transfer(lowAddress, low);
	transfer(highAddress, high);
}

void MOS6581::setFrequency(byte voice, word frequency){
	if(voice == 0){
		voiceFrequency(SID_V1_FL, SID_V1_FH, frequency);
	}else if(voice == 1){
		voiceFrequency(SID_V2_FL, SID_V2_FH, frequency);
	}else if(voice == 2){
		voiceFrequency(SID_V3_FL, SID_V3_FH, frequency);
	}
}

void MOS6581::setPulseWidth(byte voice, word frequency){
	if(voice == 0){
		voicePulseWidth(SID_V1_PWL, SID_V1_PWH, frequency);
	}else if(voice == 1){
		voicePulseWidth(SID_V2_PWL, SID_V2_PWH, frequency);
	}else if(voice == 2){
		voicePulseWidth(SID_V3_PWL, SID_V3_PWH, frequency);
	}
}

void MOS6581::filterFrequency(word frequency){

	// shift the number along 5 bits to get the high frequency
	byte low = lowByte(frequency) & B00000111;
	byte high = highByte(frequency << 5);
	// shift the number back 
	transfer(SID_FL_FL, low);
	transfer(SID_FL_FH, high);
}

void MOS6581::filterResonance(byte frequency){

	filter_register &= B00001111;

	filter_register |= (frequency << 4);

	transfer(SID_FL_RES_CT, filter_register);
	// shift the number back 
}

void MOS6581::setFilter(byte voice, boolean on){
	if(voice == 0){
		if(on){
			filter_register |= SID_FILT_VOICE1;
		}else{
			filter_register &= ~SID_FILT_VOICE1;
		}
	}else if(voice == 1){
		if(on){
			filter_register |= SID_FILT_VOICE2;
		}else{
			filter_register &= ~SID_FILT_VOICE2;
		}
	}else if(voice == 2){
		if(on){
			filter_register |= SID_FILT_VOICE3;
		}else{
			filter_register &= ~SID_FILT_VOICE3;
		}
	}
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::setVoice(byte voice, boolean on){
	if(voice == 0){
		if(on){
			voice1_register |= B00000001;
		}else{
			voice1_register &= B11111110;
		}
		transfer(SID_V1_CT, voice1_register);
	}else if(voice == 1){
		if(on){
			voice2_register |= B00000001;
		}else{
			voice2_register &= B11111110;
		}
		transfer(SID_V2_CT, voice2_register);
	}else if(voice == 2){
		if(on){
			voice3_register |= B00000001;
		}else{
			voice3_register &= B11111110;
		}
		transfer(SID_V3_CT, voice3_register);
	}

}

void MOS6581::setMode(byte voice, byte mode){

	if(voice == 0){
		voice1_register &= B00000001;
		voice1_register |= mode;
		transfer(SID_V1_CT, voice1_register);
	}else if(voice == 1){
		voice2_register &= B00000001;
		voice2_register |= mode;
		transfer(SID_V2_CT, voice2_register);
	}else if(voice == 2){
		voice3_register &= B00000001;
		voice3_register |= mode;
		transfer(SID_V3_CT, voice3_register);
	}else{
	 
	}
}

void MOS6581::setADEnvelope(byte voice, byte attack, byte decay){
	if(voice == 0){
		transfer(SID_V1_AD,(decay & B00001111) | (attack << 4));
	}else if(voice == 1){
		transfer(SID_V2_AD,(decay & B00001111) | (attack << 4));
	}else if(voice == 2){
		transfer(SID_V3_AD,(decay & B00001111) | (attack << 4));
	}
}

void MOS6581::setSREnvelope(byte voice, byte sustain, byte release){
	if(voice == 0){
		transfer(SID_V1_SR,(release & B00001111) | (sustain << 4));
	}else if(voice == 1){
		transfer(SID_V2_SR,(release & B00001111) | (sustain << 4));
	}else if(voice == 2){
		transfer(SID_V3_SR,(release & B00001111) | (sustain << 4));
	}
}

void MOS6581::volume(byte level){

	mode_register &= B11110000;
	mode_register |= (level & B00001111);

	transfer(SID_FL_MD_VL, mode_register);
}

void MOS6581::setFilterMode(byte mode){

	mode_register &= B00001111;
	mode_register |= mode;

	transfer(SID_FL_MD_VL, mode_register);
}
