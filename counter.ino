/*
 * Blurtooth Frequency Counter
 * Created: 12/08/2017 23:34:47
 *  Author: moty22.co.uk
*/


  unsigned char overF=0;
  unsigned int freq=0;

// the setup function runs once when you press reset or power the board
void setup() {
        // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    
          //1 Hz timer
    OCR0A = 249;
    TCCR0A = _BV(WGM00) | _BV(WGM01) | _BV(COM0A0); //   
    TCCR0B = _BV(WGM02) | _BV(CS02) | _BV(CS01);  //  PWM mode, input T0 pin D4
    pinMode(6, OUTPUT);
    
      //250 Hz - timer2
    OCR2A =249;
    OCR2B = 125;  
    TCCR2A = _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);  //output B in phase, fast PWM mode
    TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21); // set prescaler to 256 and start the timer
    pinMode(3, OUTPUT);
    
        //  counter input T1 pin D5
    OCR1A = 32767;   //32768 counts
    TCCR1A = _BV(WGM10) | _BV(WGM11) | _BV(COM1A0); //   
    TCCR1B =_BV(WGM12) | _BV(WGM13) | _BV(CS12) | _BV(CS11); //input pin D5
    pinMode(9, OUTPUT);
    
}

  
  // the loop function runs over and over again forever
void loop() {

    while(digitalRead(6)){} //wait for port low 
    while(!digitalRead(6)){} //wait for port high
 
    TIFR1 = _BV(OCF1A);    //reset int
    OCR1A = 65535;  //32767
    TCNT1=0;
    overF=0;
    while(digitalRead(6)){
    if(TIFR1 & (1<<OCF1A)) {++overF; TIFR1 = _BV(OCF1A);}   //overflow 
    }
    freq = TCNT1 ; 

    Serial.write(highByte(analogRead(A1)));  //send low byte of 10 bits analogue read
    Serial.write(lowByte(analogRead(A1))); //send high byte of 10 bits analogue read
    //counter bytes
    Serial.write(overF);
    Serial.write(highByte(freq));
    Serial.write(lowByte(freq));

}

