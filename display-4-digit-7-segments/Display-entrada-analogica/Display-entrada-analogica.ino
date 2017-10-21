// Timing (tasks) management

unsigned long timet;
unsigned long tempoativo1inicio;
unsigned long tempoativo2inicio;
unsigned long tempoativo3inicio;
unsigned long tempoativo1final;
unsigned long tempoativo2final;
unsigned long tempoativo3final;
unsigned long tempototal1;
unsigned long tempototal2;
unsigned long tempototal3;

unsigned long previousTime;
unsigned long previousTime2;
unsigned long previousTime3;
boolean enterFunction = true;
boolean enterFunction2 = true;
boolean enterFunction3 = true;
//-----------------------


int Potenciometro= A0;
int leituraPotenciometro = 0; 
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[11];

int posicao1= 0;
int posicao2= 2;
int posicao3= 4;
int posicao4= 6;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment
  
  dataArray[0] = 0b11000000; // 0
  dataArray[1] = 0b11111001; // 1 com ponto decimal
  dataArray[2] = 0b10100100; // 2
  dataArray[3] = 0b10110000; // 3 com ponto decimal 
  dataArray[4] = 0b10011001; // 4
  dataArray[5] = 0b10010010; // 5 com ponto decimal
  dataArray[6] = 0b10000010; // 6
  dataArray[7] = 0b11111000; // 7 com ponto decimal
  dataArray[8] = 0b10000000; // 8
  dataArray[9] = 0b10010000; // 9 com ponto decimal
  dataArray[10] = 0b11000110; // C 
  

}

void loop() {

timet = micros();
  if (enterFunction == true){
    previousTime= timet;
    //Serial.println(previousTime); // for debugging

    // Write your first code below 
//-----------------------

  leituraPotenciometro = analogRead(Potenciometro);
  posicao4 = leituraPotenciometro % 10;
  posicao3 = (leituraPotenciometro / 10) % 10;
  posicao2 = (leituraPotenciometro / 100) % 10;
  posicao1 = (leituraPotenciometro / 1000) % 10;
  
  Serial.print(leituraPotenciometro);
  Serial.print(", ");
  Serial.print(posicao1);
  Serial.print(", ");
  Serial.print(posicao2);
  Serial.print(", ");
  Serial.print(posicao3);
  Serial.print(", ");
  Serial.println(posicao4);
  
//-----------------------
// End of your first code
} 

// Your second code (different timing) starts here

  if (enterFunction2 == true){
    previousTime2= timet;
    //Serial.println(previousTime2); // for debugging

    // Write your second code below 
//-----------------------

  for (int j = 0; j < 4; j++) {
    

    if (j == 1) {  //0,2,6 // 1,3,7 //4,8 // 5,9 
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
    }
    if (j == 2) {
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
    }
    if (j == 3) {
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(13, LOW);
    }
    if (j == 0) {
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(13, HIGH);
    }

    switch(j){
      case 1:
      {
        data = dataArray[posicao1];
        break;
      }
      case 2:
      {
        data = dataArray[posicao2];
        break;
      }
      case 3:
      {
        data = dataArray[posicao3];
        break;
      }
      case 0:
      {
        data = dataArray[posicao4];
        break;
      }
      
    }
    //load the light sequence you want from array
    
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(5);

    //-----------------------
    // End of your second code    
    }   

    
  }

  // The DELAY time is adjusted in the constant below >> 
  if (timet - previousTime < 999990){ // 1 million microsencods= 1 second delay
    /* I have actually used 0.999990 seconds, in a trial to compensate the time that
       this IF function takes to be executed. this is really a point that
       need improvement in my code */   
    enterFunction= false;
  } 
  else {
    enterFunction= true;
  }

  if (timet - previousTime2 < 10){ // 500,000 microsencods= 0.5 seconds delay
      enterFunction2= false;
  } 
  else {
    enterFunction2= true;
  }
  
    // The DELAY time is adjusted in the constant below >> 
  if (timet - previousTime3 < 29990){ // 30,000 microsencods= 30 milisecond delay
      enterFunction3= false;
  } 
  else {
    enterFunction3= true;
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
