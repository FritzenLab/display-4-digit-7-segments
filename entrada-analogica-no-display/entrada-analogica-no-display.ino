/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */

 int analogInPin = A1;
 int analogReading;
 int thousands;
 int hundreds;
 int tens;
 int ones;
 
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[4];
int dados;
int tempoanteriordisplay = 0; // controls display timing information
int tempoanteriorleituras = 0; // controls analog reading timing

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment
  
  /*dataArray[6] = 0b10000010; // 6
  dataArray[7] = 0b01111000; // 7 com ponto decimal
  dataArray[8] = 0b10000000; // 8
  dataArray[4] = 0b10011001; // 4
  dataArray[5] = 0b00010010; // 5 com ponto decimal
  dataArray[9] = 0b00010000; // 9 com ponto decimal
  dataArray[10] = 0b11000110; // C */
  

}

void loop() {

  if (millis() - tempoanteriorleituras >= 500){
    tempoanteriorleituras = millis();
    
  //analogReading = analogRead(analogInPin);
  analogReading = 5555;
  thousands = (analogReading / 1000);
  hundreds = ((analogReading / 100) % 10);
  tens = ((analogReading / 100) % 10);
  ones = (analogReading % 10);

  /*
  dataArray[0] = thousands;
  dataArray[1] = hundreds;
  dataArray[2] = tens;
  dataArray[3] = ones;
  */
  dataArray[1] = 0b11000000;
  dataArray[2] = 0b11111001;
  dataArray[3] = 0b00100100;
  dataArray[0] = 0b00110000;
  
  Serial.print("leitura= ");
  Serial.print(analogReading);
  Serial.print("  -  ");
  Serial.print("Milhar= ");
  Serial.print(thousands);
  Serial.print(", Centena= ");
  Serial.print(hundreds);
  Serial.print(", Dezena= ");
  Serial.print(tens);
  Serial.print(", Unidade= ");
  Serial.println(ones); 
 
  } else{
    
  }
  
  if (millis() - tempoanteriordisplay >= 10){
    tempoanteriordisplay = millis();
    
    for (int j = 0; j < 4; j++) {
      
      if (j == 0) {  //0,2,6 // 1,3,7 //4,8 // 5,9 
      analogWrite(5, 125);
      analogWrite(6, 0);
      analogWrite(9, 0);
      analogWrite(10, 0);
    }
    if (j == 1) {
      analogWrite(5, 0);
      analogWrite(6, 125);
      analogWrite(9, 0);
      analogWrite(10, 0);
    }
    if (j == 2) {
      analogWrite(5, 0);
      analogWrite(6, 0);
      analogWrite(9, 125);
      analogWrite(10, 0);
    }
    if (j == 3) {
      analogWrite(5, 0);
      analogWrite(6, 0);
      analogWrite(9, 0);
      analogWrite(10, 125);
    }
    //load the light sequence you want from array
    data = dataArray[j];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(50);
    
    
  }
  } else{
    // DO nothing while the entering function time has not been reached yet
  }
 
}



// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
