/*
    Author: Alejandro Palacios (pamanes@gmail.com)
    Creation Date: Jan. 10, 2015
    Purpose: 
    
    This is a clock program that pushes the time
    to Shift Registers in order to light up Nixie Tubes.
	
	H   H : M   M : S   S  (Nixies)
	|   |   |   |   |   |   (Rusian Driver has 10 output pins, each lighting up a number)
    D   D   D   D   D   D   (Russian Driver K155NA1)
	  \/      \/      \/    (4 pins to each Russian Driver)
	  SF3     SF2     SF1   (each shift register has 8 output pins - 74HC595)
	  
    There are three Shift Registers, each controls two nixie tubes
*/

#include "common.h"
//define shifts pins for seconds
const int sDataPin = 4;
const int sLatchPin = 5;
const int sClockPin = 6;
//define shifts pins for minutes
const int mDataPin = 7;
const int mLatchPin = 8;
const int mClockPin = 9;
//define shifts pins for hours
const int hDataPin = 10;
const int hLatchPin = 11;
constint hClockPin = 12;
//pins for buttons
const int hButtonPin = 3;
const int mButtonPin = 13;
//buttons state
int hButtonState = 0;
int mButtonState = 0;
//flag when button released
qboolean hFlag = qtrue;
qboolean mFlag = qtrue;
//wat a sec each cycle
const int interval = 1000;
//keep track o ftime
unsigned long previousMillis = 0;
//this is to handle when millis resets
const unsigned long MAX = 4294967295;
//byte pushed out to shift registers
byte sLed = 0;
byte mLed = 0;
byte hLed = 0;

//clock variables, init at 12:00:00
int h = 12;
int m = 0;
int s = 0;

//define pins direction
void setup()
{
	//BUTTONS
	pinMode(hButtonPin, INPUT);
	pinMode(mButtonPin, INPUT);

	//SHIFTS
	pinMode(sLatchPin, OUTPUT);
	pinMode(sDataPin, OUTPUT);
	pinMode(sClockPin, OUTPUT);

	pinMode(mLatchPin, OUTPUT);
	pinMode(mDataPin, OUTPUT);
	pinMode(mClockPin, OUTPUT);

	pinMode(hLatchPin, OUTPUT);
	pinMode(hDataPin, OUTPUT);
	pinMode(hClockPin, OUTPUT);
}

//add either hour, minute or second
//and call ShowClock to refresh nixies
void clock(qboolean hAdd, qboolean mAdd, qboolean sAdd)
{
	if(hAdd == qtrue)
		h = h + 1;
	if(mAdd == qtrue)
		m = m + 1;
	if(sAdd == qtrue)
		s = s + 1;

	if(s>59){m = m + 1;s = 0;}
	if(m>59){h = h + 1;m = 0;}
	if(h>12){h = 1;m = 0;s = 0;}

	ShowClock();
}

//display to nixies 
void ShowClock()
{
	//each shift register controls two nixies
	//split the value into two , then shift bits
	//of first digit and sum then update shifts
	sLed = (s % 10) + ((s / 10) << 4);
	mLed = (m % 10) + ((m / 10) << 4);
	hLed = (h % 10) + ((h / 10) << 4);

    UpdateShiftRegister();
}

void loop()
{
    // Get snapshot of time
    unsigned long currentMillis = millis();
    
    hButtonState = digitalRead(hButtonPin);//pin 3
    mButtonState = digitalRead(mButtonPin);//pin 13
    //flag for only processing one time when pressed
    if(!hFlag && hButtonState == LOW)
        hFlag = qtrue;
    if(!mFlag && mButtonState == LOW)
        mFlag = qtrue;
	//Note: They will be high and low randomly if not connected!
	
	//if hour button was pushed add hour
    if (hButtonState == HIGH && hFlag)
    {
        //add hour
        hFlag = qfalse;
        clock(qtrue, qfalse, qfalse);
    }
    //if minute button was pushed
    if (mButtonState == HIGH && mFlag)
    {
        //add minute  
        mFlag = qfalse;
        clock(qfalse, qtrue, qfalse);
    }

    //handle reset to 0 when max unsigned long is reached (after 49 days)
    if(((unsigned long)(currentMillis - previousMillis) >= interval) || 
	((currentMillis < previousMillis) && ((unsigned long)(MAX - (previousMillis-1) + currentMillis) >= interval)))
    {
        // It's time to do something!
        clock(qfalse, qfalse, qtrue);
        previousMillis = currentMillis;
    } 
}
//shift out
void UpdateShiftRegister()
{
    digitalWrite(sLatchPin, LOW);
    shiftOut(sDataPin, sClockPin, LSBFIRST, sLed);
    digitalWrite(sLatchPin, HIGH);

    digitalWrite(mLatchPin, LOW);
    shiftOut(mDataPin, mClockPin, LSBFIRST, mLed);
    digitalWrite(mLatchPin, HIGH);

    digitalWrite(hLatchPin, LOW);
    shiftOut(hDataPin, hClockPin, LSBFIRST, hLed);
    digitalWrite(hLatchPin, HIGH);
}
