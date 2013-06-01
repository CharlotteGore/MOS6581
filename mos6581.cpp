#include "mos6581.h"
#include <SPI.h>
#include "sid_registers.h"

void print_binary(int v, int num_places)
{
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << num_places-1))
        {
             Serial.print("1");
        }
        else
        {
             Serial.print("0");
        }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}

MOS6581::MOS6581(void){
	ss = 7;
	cs = 4;
	clk = 9;
	initialise();
	startClock();
}

MOS6581::MOS6581(byte shiftPin, byte sidPin, byte clockPin){
	ss = shiftPin;
	cs = sidPin;
	clk = clockPin;
	initialise();
	startClock();
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

	SPI.begin();

#ifdef SID_DEBUG
	Serial.begin(57600);
#endif

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
	delayMicroseconds(10);
	// enable write mode on the SID
	digitalWrite(cs, LOW);
	// wait a small amount of time for the SID to pick up the data
	delayMicroseconds(10);
	// disable write mode on the SID
	digitalWrite(cs, HIGH);

#ifdef SID_DEBUG
	print_binary(address, 5);
	Serial.print("\t");
	Serial.print(address, DEC);
	Serial.print("\t");
	print_binary(value, 8);
	Serial.print("\t");
	Serial.print(value, DEC);
	Serial.println("");
#endif

#ifdef SID_DEBUG
	delay(1000);
#endif
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

void MOS6581::voiceOneFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice one frequency");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voiceFrequency(SID_V1_FL, SID_V1_FH, frequency);
}

void MOS6581::voiceTwoFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice two frequency");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voiceFrequency(SID_V2_FL, SID_V2_FH, frequency);
}

void MOS6581::voiceThreeFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice three frequency");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voiceFrequency(SID_V3_FL, SID_V3_FH, frequency);
}

void MOS6581::voiceOnePulseWidthFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice one pulse width");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voicePulseWidth(SID_V1_PWL, SID_V1_PWH, frequency);
}

void MOS6581::voiceTwoPulseWidthFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice two pulse width");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voicePulseWidth(SID_V2_PWL, SID_V2_PWH, frequency);
}

void MOS6581::voiceThreePulseWidthFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Voice three pulse width");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	voicePulseWidth(SID_V3_PWL, SID_V3_PWH, frequency);
}

void MOS6581::filterFrequency(word frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Filter frequency");
	Serial.print("\t");
	print_binary(frequency, 16);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif
	// shift the number along 5 bits to get the high frequency
	byte low = lowByte(frequency) & B00000111;
	byte high = highByte(frequency << 5);
	// shift the number back 
	transfer(SID_FL_FL, low);
	transfer(SID_FL_FH, high);
}

void MOS6581::filterResonance(byte frequency){

#ifdef SID_DEBUG
	Serial.println("");
	Serial.print("Filter resonance");
	Serial.print("\t");
	print_binary(frequency, 8);
	Serial.print("\t");
	Serial.print(frequency, DEC);
	Serial.println("");
#endif

	filter_register &= B00001111;

	filter_register |= (frequency << 4);

	transfer(SID_FL_RES_CT, filter_register);
	// shift the number back 
}

void MOS6581::voiceOneFilterOn(){

#ifdef SID_DEBUG
	Serial.println("Voice one filter on");
#endif

	filter_register |= SID_FILT_VOICE1;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceTwoFilterOn(){

#ifdef SID_DEBUG
	Serial.println("Voice two filter on");
#endif

	filter_register |= SID_FILT_VOICE2;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceThreeFilterOn(){

#ifdef SID_DEBUG
	Serial.println("Voice three filter on");
#endif

	filter_register |= SID_FILT_VOICE3;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceOneFilterOff(){

#ifdef SID_DEBUG
	Serial.println("Voice one filter off");
#endif

	filter_register &= ~SID_FILT_VOICE1;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceTwoFilterOff(){

#ifdef SID_DEBUG
	Serial.println("Voice two filter off");
#endif

	filter_register &= ~SID_FILT_VOICE2;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceThreeFilterOff(){

#ifdef SID_DEBUG
	Serial.println("Voice three filter off");
#endif

	filter_register &= ~SID_FILT_VOICE3;
	transfer(SID_FL_RES_CT, filter_register);
}

void MOS6581::voiceOneMode(byte mode){

#ifdef SID_DEBUG
	Serial.println("Voice one mode");
#endif

	voice1_register &= B00000001;
	voice1_register |= mode;
	transfer(SID_V1_CT, voice1_register);

}

void MOS6581::voiceTwoMode(byte mode){

#ifdef SID_DEBUG
	Serial.println("Voice two mode");
#endif

	voice2_register &= B00000001;
	voice2_register |= mode;
	transfer(SID_V2_CT, voice2_register);

}

void MOS6581::voiceThreeMode(byte mode){

#ifdef SID_DEBUG
	Serial.println("Voice three mode");
#endif

	voice3_register &= B00000001;
	voice3_register |= mode;
	transfer(SID_V3_CT, voice3_register);

}

void MOS6581::voiceOneEnvelope(byte attack, byte decay, byte sustain, byte release){

#ifdef SID_DEBUG
	Serial.println("Voice one attack/decay");
#endif

	transfer(SID_V1_AD,(decay & B00001111) | (attack << 4));

#ifdef SID_DEBUG
	Serial.println("Voice one sustain/release");
#endif

	transfer(SID_V1_SR,(release & B00001111) | (sustain << 4));

}


void MOS6581::voiceTwoEnvelope(byte attack, byte decay, byte sustain, byte release){

#ifdef SID_DEBUG
	Serial.println("Voice two attack/decay");
#endif

	transfer(SID_V2_AD,(decay & B00001111) | (attack << 4));

#ifdef SID_DEBUG
	Serial.println("Voice two sustain/release");
#endif

	transfer(SID_V2_SR,(release & B00001111) | (sustain << 4));

}

void MOS6581::voiceThreeEnvelope(byte attack, byte decay, byte sustain, byte release){

#ifdef SID_DEBUG
	Serial.println("Voice three attack/decay");
#endif

	transfer(SID_V3_AD,(decay & B00001111) | (attack << 4));

#ifdef SID_DEBUG
	Serial.println("Voice three sustain/release");
#endif

	transfer(SID_V3_SR,(release & B00001111) | (sustain << 4));

}

void MOS6581::volume(byte level){

#ifdef SID_DEBUG
	Serial.println("Set volume");
#endif

	mode_register &= B11110000;
	mode_register |= (level & B00001111);

	transfer(SID_FL_MD_VL, mode_register);
}

void MOS6581::filterBP(){

#ifdef SID_DEBUG
	Serial.println("Setting filter band pass mode");
#endif

	mode_register &= B00001111;
	mode_register |= SID_FILT_BP;

	transfer(SID_FL_MD_VL, mode_register);
}

void MOS6581::filterLP(){

#ifdef SID_DEBUG
	Serial.println("Setting filter low pass mode");
#endif

	mode_register &= B00001111;
	mode_register |= SID_FILT_LP;

	transfer(SID_FL_MD_VL, mode_register);
}

void MOS6581::filterHP(){

#ifdef SID_DEBUG
	Serial.println("Setting filter high pass mode");
#endif

	mode_register &= B00001111;
	mode_register |= SID_FILT_HP;

	transfer(SID_FL_MD_VL, mode_register);
}

void MOS6581::filterNotch(){

#ifdef SID_DEBUG
	Serial.println("Setting filter notch mode");
#endif

	mode_register &= B00001111;
	mode_register |= (SID_FILT_HP | SID_FILT_LP);

	transfer(SID_FL_MD_VL, mode_register);
}

