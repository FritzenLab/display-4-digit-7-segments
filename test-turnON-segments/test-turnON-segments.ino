/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[11];
int dados;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment
  dataArray[0] = 0b00110000; // 3 com ponto decimal 
  dataArray[1] = 0b11000000; // 0
  dataArray[2] = 0b01111001; // 1 com ponto decimal
  dataArray[3] = 0b10100100; // 2
  dataArray[4] = 0b10011001; // 4
  dataArray[5] = 0b00010010; // 5 com ponto decimal
  dataArray[6] = 0b10000010; // 6
  dataArray[7] = 0b01111000; // 7 com ponto decimal
  dataArray[8] = 0b10000000; // 8
  dataArray[9] = 0b00010000; // 9 com ponto decimal
  dataArray[10] = 0b11000110; // C 
  

}

void loop() {

  for (int j = 0; j < 4; j++) {
    if (dados < 11){
      dados= dados +1;
    } else{
      dados= 0;
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
    delay(5);
    if (j == 0) {  //0,2,6 // 1,3,7 //4,8 // 5,9 
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
    }
    if (j == 1) {
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
    }
    if (j == 2) {
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(13, LOW);
    }
    if (j == 3) {
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(13, HIGH);
    }
    
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
