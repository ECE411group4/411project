#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 2
#define NUMPIXELS 6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 12, d5 = 11, d6 = 10, d7 = 9; //PCB
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float LightRead = 0;
int Samples = 500;
int MicRead[500];
int PeakArray[500];
int PixR;
int PixG;
int PixB;
int LightValue; //Used for dimming.
float Lux;
float Peak;
unsigned long SoundSum = 0;
float SoundAvg = 0;
float DecibelNow;
float DecibelMax;
float dBLocalMax;
bool Reset = false;
void setup() {
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(3,INPUT); //Reset Level Button
}

void loop() {
  Peak = 0; //Reset Peak dB value.
  LightRead = 0;
 //Read Mic-------------------------------------------------------------------------
for(int j=0;j<=Samples;j++){ //Capture sound from mic and store in array MicRead
  MicRead[j] = analogRead(A1);
 if(MicRead[j]<512){
    MicRead[j]=512-MicRead[j]; //Rectify the Mic output, and remove DC offset.
  }
  else{
    MicRead[j]=MicRead[j]-512; //Remove DC offset if greater than 512. Data should now range from 0 to 512 and be rectified.
  }
}
//----------------------------------------------------------------------------------



for (int k = 0; k<=Samples; k++){

if(Peak < MicRead[k]){
 Peak = MicRead[k];
}

}
dBLocalMax = Peak/512*72;
if(dBLocalMax > DecibelMax){
  DecibelMax = dBLocalMax;
}


//Light Sensor ---------------------------------------------------------------------
  //Lux = analogRead(A2); //Because the light sensor reads from 0-1000 Lux, the raw analog output value is actually the correct scale. Therefore it needs no calibration.
  for (int i=0; i<(Samples/3); i++){
    LightRead = LightRead + analogRead(A2);
  }
  
  Lux = LightRead/(Samples/3);


//Max Value Reset Button------------------------------------------------------------

  Reset = digitalRead(3);
  if (Reset == 0){
  DecibelMax = 0;
  DecibelNow = 0;
  delay(500); //Avoid sound of button press affecting dB reading
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("dB Max ");
  lcd.print(DecibelMax);
  lcd.setCursor(0, 1);  
  lcd.print("Lux ");
  lcd.print(Lux);


  //Neo Pixels-------------------------------------------------------------------
//Autodim---------
if (Lux<300){
LightValue = 50;
}
if (Lux>=300&&Lux<700){
LightValue = 100;
}
if (Lux>=700){
LightValue = 200;
}
//----------------
if(Peak<170){ //Low sound level, set to green.
  PixR = 0;
  PixG = LightValue;
  PixB = 0;
}

if(Peak>170&&Peak<340){ //Medium sound level, set to yellow.
  PixR = LightValue;
  PixG = LightValue;
  PixB = 0;
}

if(Peak>340){ //High sound level, set to red.
  PixR = LightValue;
  PixG = 0;
  PixB = 0;
}
if(Peak>64){ 
  pixels.setPixelColor(0, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(0, pixels.Color(0,0,0));
}
if(Peak>64*2){ 
  pixels.setPixelColor(1, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(1, pixels.Color(0,0,0));
}
if(Peak>64*3){ 
  pixels.setPixelColor(2, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(2, pixels.Color(0,0,0));
}
if(Peak>64*4){ 
  pixels.setPixelColor(3, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(3, pixels.Color(0,0,0));
}
if(Peak>64*5){ 
  pixels.setPixelColor(4, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(4, pixels.Color(0,0,0));
}
if(Peak>64*6){ 
  pixels.setPixelColor(5, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(5, pixels.Color(0,0,0));
}
if(Peak>64*6){ 
  pixels.setPixelColor(6, pixels.Color(PixR,PixG,PixB));
}
else{
  pixels.setPixelColor(6, pixels.Color(0,0,0));
}
pixels.show();






}