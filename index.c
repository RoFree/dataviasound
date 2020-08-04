#include dtmf1.h
#include dtmf2.h
#include qrcode.h
#include <Goertzel.h>
int raw
int samp_rate= //Sample Rate
int sendbutton = INSERTPINBUTTONTOSENDISCONNECTEDTO
int micPin = PINMICROPHONECONNECTEDTO
int val
int threshold= //threshold for dtmf decoder
int id;
int val2 = 0;
int myID= INSERTYOUR4DIGITIDHERE
int othersID["insert max number of participants"]
int buttonqr = INSERTPINTHATBUTTONTOGENQRIS
void PrintQRCode(const char * url){
  QRCode qrcode;

  const int ps = 3; //pixels / square
  
  uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
  qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

 
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      //If pixel is on, we draw a ps x ps black square
      if(qrcode_getModule(&qrcode, x, y)){
        for(int xi = x*ps + 2; xi < x*ps + ps + 2; xi++){
          for(int yi= y*ps + 2; yi < y*ps + ps + 2; yi++){
            ///code to display on screen of choice goes here using xi, yi, EPD_BLACK
          }
        }
      }
    }
  }
}
bool detect_tone(float freq){

Goertzel goertzel = Goertzel(freq, N, sampling_freq);        //initialize library function with the given sampling frequency no of samples and target freq
  goertzel.sample(sensorPin);                               //Will take n samples          
  float magnitude = goertzel.detect();                      //check them for target_freq

 if(magnitude>threshold){                                   //if you're getting false hits or no hits adjust the threshold
  digitalWrite(13,HIGH);                                    //blink led on 13 if a pulse is detected
  delay(250);
  digitalWrite(13,LOW);
  Serial.print(freq);
  Serial.print("\n");
  return freq;
 }
  else
    return false;
}
void detect_digit(float *spectrum) {
  float avg_row = avg(spectrum, 4);
  float avg_col = avg(&spectrum[4], 4);
  int8_t row = get_single_index_above_threshold(spectrum, 4, avg_row);
  int8_t col = get_single_index_above_threshold(&spectrum[4], 4, avg_col);
  
  if (row != -1 && col != -1 && avg_col > 200) {
    detected_digit.digit = pgm_read_byte(&(table[row][col]));
    detected_digit.index = pgm_read_byte(&(char_indexes[row][col]));
  } else {
    detected_digit.digit = 0;
  }
}
void setup()
{

pinMode(buttonqr, INPUT);
pinMode(sendbutton, INPUT);

}
void loop()
{
  val = digitalRead(buttonqr);
  val2 = digitalRead(sendbutton);
  if (val == HIGH) {         // check if the input is HIGH (button released)
    PrintQRCode(othersID)  // make QR
  }
  if (val2 == High){
    dialNumber2(myID, 4);
  }
  if (micPin > threshold){
    
    raw = detect_tone(samp_rate)
    id = id + 1;
    othersID[id] = detect_digit(raw);
  
  }

}
