#include "pitches.h"  //add note library
#include <LiquidCrystal.h> // for LCD monitor
#include "IRremote.h" // for IR remote control
#include <TimerFreeTone.h> // an extra library since the tone timer conflict with the IRremote timer

#define TONE_PIN 2 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).
int buzzerPin = 2; // buzzer
const int gPin = 3;  // green LED
const int yPin = 4;  // yellow LED
const int rPin = 5;  // red LED
int receiver = 6; // Signal Pin of IR receiver to Arduino Digital Pin 6
int buttonPin = 13; // Door bell button

int ledState = 0;
int ledOn = false;

//notes in the melody
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int melody2[] = {NOTE_C4, NOTE_G3};
//note durations. 4=quarter note / 8=eighth note
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4}; 
int noteDurations2[] = {4,4}; 

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/*-----( Declare IR objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void setup() {
  //make the button's pin input
  pinMode(buttonPin, INPUT);
  pinMode(rPin, OUTPUT);
  pinMode(yPin, OUTPUT);
  pinMode(gPin, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {

  //read the input pin
  int buttonState = digitalRead(buttonPin);
  //if the door bell(button) is pressed
  if (buttonState == 1) {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    delay(500);
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print("Please wait        ");
    //iterate over the notes of the melody
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
      // blink the three LEDs in sequence
      if (thisNote % 3 == 0) {
        digitalWrite(rPin, HIGH);
        digitalWrite(yPin, LOW);
        digitalWrite(gPin, LOW);
      }
      else if (thisNote % 3 == 1) {
        digitalWrite(rPin, LOW);
        digitalWrite(yPin, HIGH);
        digitalWrite(gPin, LOW);
      }
      else if (thisNote % 3 == 2) {
        digitalWrite(rPin, LOW);
        digitalWrite(yPin, LOW);
        digitalWrite(gPin, HIGH);
      }
      //to calculate the note duration, take one second. Divided by the note type
      int noteDuration = 1000 / noteDurations [thisNote];
      TimerFreeTone(buzzerPin, melody [thisNote], noteDuration);
      //to distinguish the notes, set a minimum time between them
      //the note's duration +30%, then change to 1.30
      int pauseBetweenNotes = noteDuration * 1.00;
      delay(pauseBetweenNotes);
    }
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                  "); // remove the second row output on LCD
  }

  // for IR receiver
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    switch (results.value)
    {
      case 0xFF6897:
        lcd.clear();
        lcd.print("Press button"); // "0"
        for (int thisNote = 0; thisNote < 2; thisNote++)
        {
          int noteDuration = 1000 / noteDurations2 [thisNote];
          TimerFreeTone(buzzerPin, melody2 [thisNote], noteDuration);
          delay(noteDuration);
        }
        break;
      case 0xFF30CF:
        lcd.clear();
        lcd.print("Working >0<"); // "1"
        for (int thisNote = 0; thisNote < 2; thisNote++)
        {
          int noteDuration = 1000 / noteDurations2 [thisNote];
          TimerFreeTone(buzzerPin, melody2 [thisNote], noteDuration);
          delay(noteDuration);
        }
        break;
      case 0xFF18E7:
        lcd.clear();
        lcd.print("Busy! )0("); // "2"
        for (int thisNote = 0; thisNote < 2; thisNote++)
        {
          int noteDuration = 1000 / noteDurations2 [thisNote];
          TimerFreeTone(buzzerPin, melody2 [thisNote], noteDuration);
          delay(noteDuration);
        }
        break;
      case 0xFF7A85:
        lcd.clear();
        lcd.print("Sleeping zZZ"); // "3"
        for (int thisNote = 0; thisNote < 2; thisNote++)
        {
          int noteDuration = 1000 / noteDurations2 [thisNote];
          TimerFreeTone(buzzerPin, melody2 [thisNote], noteDuration);
          delay(noteDuration);
        }
        break;
      case 0xFF9867:
        for (int thisNote = 0; thisNote < 2; thisNote++)
        {
          int noteDuration = 1000 / noteDurations2 [thisNote];
          TimerFreeTone(buzzerPin, melody2 [thisNote], noteDuration);
          delay(noteDuration);
        }
        lcd.setCursor(0, 1);
        lcd.print("Please come in"); // "EQ"
        digitalWrite(gPin, HIGH); // green LED on
        delay(3000);
        digitalWrite(gPin, LOW); // green LED off
        lcd.setCursor(0, 1);
        lcd.print("                  ");
        break;
    }
    irrecv.resume(); // receive the next value
  }
}

///*-----( IR remote example )-----*/
//void translateIR() // takes action based on IR code received
//
//// describing Remote IR codes
//
//{
//
//  switch(results.value)
//
//  {
////  case 0xFFA25D:
////  {
////    lcd.setCursor(0, 1);
////    // print the number of seconds since reset:
////    lcd.print("Please wait");
////    break;
////  }
//  case 0xFFA25D: Serial.println("POWER"); break;
//  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
//  case 0xFF629D: Serial.println("VOL+"); break;
//  case 0xFF22DD: Serial.println("FAST BACK");    break;
//  case 0xFF02FD: Serial.println("PAUSE");    break;
//  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
//  case 0xFFE01F: Serial.println("DOWN");    break;
//  case 0xFFA857: Serial.println("VOL-");    break;
//  case 0xFF906F: Serial.println("UP");    break;
//  case 0xFF9867: Serial.println("EQ");    break;
//  case 0xFFB04F: Serial.println("ST/REPT");    break;
//  case 0xFF6897: Serial.println("0");    break;
//  case 0xFF30CF: Serial.println("1");    break;
//  case 0xFF18E7: Serial.println("2");    break;
//  case 0xFF7A85: Serial.println("3");    break;
//  case 0xFF10EF: Serial.println("4");    break;
//  case 0xFF38C7: Serial.println("5");    break;
//  case 0xFF5AA5: Serial.println("6");    break;
//  case 0xFF42BD: Serial.println("7");    break;
//  case 0xFF4AB5: Serial.println("8");    break;
//  case 0xFF52AD: Serial.println("9");    break;
//  case 0xFFFFFFFF: Serial.println(" REPEAT");break;
//
//  default:
//    Serial.println(" other button   ");
//
//  }// End Case
//
//  delay(500); // Do not get immediate repeat
//
//
//} //END translateIR
