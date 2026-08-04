/*
yt demo: https://youtube.com/shorts/q8tIRSl5bwY?feature=share
Sooo, it worked with a potentiometer and a switch
*/
  #include "pitches.h"

  #define dataPin 11
  #define clockPin 12
  #define latchPin 8
  #define D1 3
  #define D2 4
  #define D3 5
  #define D4 6
  #define pot A1
  #define button 2
  #define BUZZER_PIN 9

  byte digits[10] = {
    B00000011, B10011111, B00100101, B00001101,
    B10011001, B01001001, B01000001, B00011111,
    B00000001, B00001001
  };

  // -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------



int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6
};

int durations[] = {
  8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4
};




void song()
{
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

  int currentDigit=0;
  unsigned long lastRefresh = 0;

  long secret;
  const int digitPins[4] = {D1, D2, D3, D4};
  int display[4] = {0, 0, 0, 0};
  int secr[4];

  void updateDis(int digit) {
    //Serial.print("Pressed: "); Serial.println(digit);
    display[0] = display[1];
    display[1] = display[2];
    display[2] = display[3];
    display[3] = digit;
  }
  void refresh() {
    for(int i=0; i<4; i++) digitalWrite(digitPins[i], LOW);
    currentDigit = (currentDigit+1)%4;
    int number = display[currentDigit];
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, digits[number]);
    digitalWrite(latchPin, HIGH);
    digitalWrite(digitPins[currentDigit], HIGH);
  }

  void winning() {
  for(int i=0; i<4; i++) digitalWrite(digitPins[i], LOW);
  Serial.println("Κέρδισες!!");
  song();
  secret = random(1000, 10000);
  for(int i=0; i<4; i++) display[i] = 0;
}

int selected = 0;
void input() {
  long value = analogRead(pot);
  long digit = map(value, 0, 1023, 0, 9);
  display[selected] = digit;
  if (digitalRead(button) == HIGH){
    if (selected == 3) {
      int correct=0;
      long guess = 1000*(long)display[0]+100*(long)display[1]+10*(long)display[2]+display[3];
      Serial.print("Η μαντεψιά σου: "); Serial.println(guess);
      if (guess == secret) winning();
      else {
        for (int i=0; i<4; i++) {
          if (display[i] == secr[i]) correct++;
        }
        Serial.print("έχεις σωστά "); Serial.print(correct); Serial.println(" ψηφία");
        Serial.println(guess > secret ? "Χαμηλότερα" : "Ψηλότερα");
      }
    }
    selected = (selected+1)%4;
    delay(500);
  }

}

  void setup() {
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(pot, INPUT);
    pinMode(button, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    for (int i=0; i<4; i++) {
      pinMode(digitPins[i], OUTPUT);
    }
    Serial.begin(115200);
    randomSeed(analogRead(0));
    secret = random(1000, 10000);
    Serial.println(secret);
    secr[0] = secret/1000;
    secr[1] = (secret/100)%10;
    secr[2] = (secret/10)%10;
    secr[3] = secret%10;
    Serial.println("Game started, guess the 4-digit number");
  }

  void loop() {
    if (millis() - lastRefresh >= 5) {
      lastRefresh = millis();
      refresh();
    }
    input();   
  }
