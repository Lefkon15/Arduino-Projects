// C++ code
//
// -------------------------------------------------
// Copyright (c) 2024 HiBit <https://www.hibit.dev>
// ------------------------------------------------- for the winning music
//tinkercad schematic https://www.tinkercad.com/things/aptvVMWeCn0-simon-says?sharecode=SqxM4pFtRtztcZ5iLi1MOxlCsY_Q4xP1yrEbKdD4794
#include "pitches.h"
#define RedPin 7
#define BluePin 6
#define GreenPin 13
#define RedIn 10
#define BlueIn 9
#define GreenIn 8
int memory[50];


#define BUZZER_PIN 11

int melody[] = {
  NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
  NOTE_D5, REST, NOTE_E5, REST, NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST,
  NOTE_G5, NOTE_F5, NOTE_E5, REST,

  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST
};

int durations[] = {
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2, 2,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 4, 8, 8, 8, 8, 8, 1, 4,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 3, 8, 8, 8, 8, 8, 1, 4,

  2, 6, 2, 6, 4, 4, 2, 6, 2, 3,
  8, 8, 8, 8,

  2, 6, 2, 6, 2, 1
};


void setup()
{
  Serial.begin(9600);
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(RedIn, INPUT);
  pinMode(BlueIn, INPUT);
  pinMode(GreenIn, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
    randomSeed(analogRead(0)); 
}

void StartGame(int round) {
    digitalWrite(RedPin, LOW);
    digitalWrite(BluePin, LOW);
    digitalWrite(GreenPin, LOW);
    for (int i=0; i<round; i++) {
      delay(500);
      int currLed = random(1,4);
      memory[i] = currLed;
      Serial.println(currLed);
      if (currLed == 1) digitalWrite(RedPin, HIGH);
      if (currLed == 2) digitalWrite(BluePin, HIGH);
      if (currLed == 3) digitalWrite(GreenPin, HIGH);
      delay(500);
      digitalWrite(RedPin, LOW);
      digitalWrite(BluePin, LOW);
      digitalWrite(GreenPin, LOW);
    }
}

int ButtonPress() {
  while (true) {
    if (digitalRead(RedIn) == HIGH) {
      digitalWrite(RedPin, HIGH);
      while(digitalRead(RedIn) == HIGH) { delay(10); } 
      digitalWrite(RedPin, LOW);
      Serial.println("Pressed: RED");
      return 1;
    }
    if (digitalRead(BlueIn) == HIGH) {
      digitalWrite(BluePin, HIGH);
      while(digitalRead(BlueIn) == HIGH) { delay(10); } 
      digitalWrite(BluePin, LOW);
      Serial.println("Pressed: BLUE");
      return 2;
    }
    if (digitalRead(GreenIn) == HIGH) {
      digitalWrite(GreenPin, HIGH);
      while(digitalRead(GreenIn) == HIGH) { delay(10); } 
      digitalWrite(GreenPin, LOW);
      Serial.println("Pressed: GREEN");
      return 3;
    }
  }
}
      
      

bool Play(int round) {
  for (int i=0; i<round; i++) {
   int curr = ButtonPress();
   if (memory[i] != curr) return true;
	}
 return false;
}
  
 void Winning() {
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
 }
void Lossing() {
  tone(BUZZER_PIN, 1000);
  digitalWrite(RedPin, HIGH);
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(RedPin, LOW);
  Serial.println("loss");
}
    


void loop()
{
  digitalWrite(RedPin, LOW);
  digitalWrite(BluePin, LOW);
  digitalWrite(GreenPin, LOW);
  int round = 3;
  bool lose = false;
  while (!lose) {
    StartGame(round);
    lose = Play(round);
    if (lose) {
      Lossing();
      delay(700);
     }
    else {
      round++;
      if (round == 6) {
        Winning();
        delay(700);
        break;
      }
          else { delay(1000); }
     }
    }
        
}

