/* DTMF encoder (Dual Tone Generator) for a Phone Dialer
 *  Created by David Dubins, May 13th, 2016.
 *  Released into the public domain.
 *
 * SETUP:
 * - Connect Pins 12 and 13 to the + speaker terminal, each through their own 240 Ohm resistor
 * - Connect a 4.7 uF capacitor between the + and - terminals of the speaker
 * - Connect speaker GND to Arduino GND
 * - Connect a momentary switch to Pin 8, and the other side of the switch to GND
 */
 
const byte tone1Pin=12; // pin for tone 1
const byte tone2Pin=13; // pin for tone 2
byte PhoneNumber[]={8,6,7,5,3,0,9}; // for special characters: 10=*, 11=#, 12=1sec delay
byte PhoneNumberLength = 7;  // adjust to length of phone number
const byte buttonPin=8; // for momentary switch
 
// frequencies adopted from: https://en.wikipedia.org/wiki/Dual-tone_multi-frequency_signaling
int DTMF[13][2]={
  {941,1336}, // frequencies for touch tone 0
  {697,1209}, // frequencies for touch tone 1
  {697,1336}, // frequencies for touch tone 2
  {697,1477}, // frequencies for touch tone 3
  {770,1209}, // frequencies for touch tone 4
  {770,1336}, // frequencies for touch tone 5
  {770,1477}, // frequencies for touch tone 6
  {852,1209}, // frequencies for touch tone 7
  {852,1336}, // frequencies for touch tone 8
  {852,1477}, // frequencies for touch tone 9
  {941,1209}, // frequencies for touch tone *
  {941,1477}, // frequencies for touch tone #
  {0,0} // pause
};
 
void setup()
{  
  pinMode(tone1Pin,OUTPUT); // Output for Tone 1
  pinMode(tone2Pin,OUTPUT); // Output for Tone 2
  pinMode(buttonPin,INPUT_PULLUP); // Button
}
 
void loop()
{
  if(digitalRead(buttonPin)==LOW){  // If the button is pushed
    dialNumber(PhoneNumber,PhoneNumberLength);  // Dial the number
  }
}
 
void playDTMF(byte digit, byte duration){
  boolean tone1state=false;
  boolean tone2state=false;
  int tone1delay=(500000/DTMF[digit][0])-10; // calculate delay (in microseconds) for tone 1 (half of the period of one cycle). 10 is a fudge factor to raise the frequency due to sluggish timing.
  int tone2delay=(500000/DTMF[digit][1])-10; // calculate delay (in microseconds) for tone 2 (half of the period of one cycle). 10 is a fudge factor to raise the frequency due to sluggish timing.
  unsigned long tone1timer=micros();
  unsigned long tone2timer=micros();
  unsigned long timer=millis(); // for timing duration of a single tone
  if(digit==12){
    delay(1000); // one second delay if digit is 12
  } else {
    while(millis()-timer<duration){
      if(micros()-tone1timer>tone1delay){
        tone1timer=micros(); // reset the timer
        tone1state=!tone1state; // toggle tone1state
        digitalWrite(tone1Pin, tone1state);
      }
      if(micros()-tone2timer>tone2delay){
        tone2timer=micros(); // reset the timer
        tone2state=!tone2state; // toggle tone2state
        digitalWrite(tone2Pin, tone2state);
      }
    }
    digitalWrite(tone1Pin,LOW);
    digitalWrite(tone2Pin,LOW);
  }
}
 
void dialNumber(byte number[],byte len){
  for(int i=0;i<len;i++){
    playDTMF(number[i], 100);  // 100 msec duration of tone
    delay(100); // 100 msec pause between tones
  }
}
