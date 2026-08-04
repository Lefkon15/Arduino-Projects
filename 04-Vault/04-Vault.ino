  /*
  few words about the project:
  it was originally supposed to be a much more complicated project around the 1602 display
  but because of the weird wiring, I couldn't get it to work (no I2C pack so pins couldn't make good contact without soldering)
  So, to salvage it, I replaced it with the 4 digit 7 segment display with the 74HC595 shift register
  However, the input method of the IR receiver with the remote proved to be too unrealiable and I couldn't get it to consustently work
  Upon research, I figured that this is because the specific IR receiver (VS1838, I think) is of the lower end and it
  probably picks up too much interference (it wouldn't even work on the daylight realiably)

   
  */
  
  
  #define DECODE_NEC
  #include <IRremote.hpp>

  #define dataPin 11
  #define clockPin 12
  #define latchPin 8
  #define D1 3
  #define D2 4
  #define D3 5
  #define D4 6
  #define receiver 2

  byte digits[10] = {
    B00000011,
    B10011111,
    B00100101,
    B00001101,
    B10011001,
    B01001001,
    B01000001,
    B00011111,
    B00000001,
    B00001001
  };

  int currentDigit=0;
  unsigned long lastRefresh = 0;
  unsigned long lastButtonTime = 0;

  long secret;
  const int digitPins[4] = {D1, D2, D3, D4};
  int display[4] = {0, 0, 0, 0};

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
  Serial.println("You won!!");
  secret = random(1000, 10000);
  for(int i=0; i<4; i++) display[i] = 0;
}

  void input() {
    uint16_t cmd = IrReceiver.decodedIRData.command;
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;

    if (millis() - lastButtonTime < 400) { return;}
    lastButtonTime = millis();

    int pressed = -1;
    switch(cmd) {
      case 22: { pressed=0; break;}
      case 12: { pressed=1; break;}
      case 24: { pressed=2; break;}
      case 94: { pressed=3; break;}
      case 8: { pressed=4; break;}
      case 28: { pressed=5; break;}
      case 90: { pressed=6; break;}
      case 66: { pressed=7; break;}
      case 82: { pressed=8; break;}
      case 74: { pressed=9; break;}
      case 9: {
        long guess = 1000*(long)display[0]+100*(long)display[1]+10*(long)display[2]+display[3];
        Serial.print("your guess:"); Serial.println(guess);
        if (guess==secret) winning();
        else Serial.println(guess > secret ? "LOWER" : "HIGHER");
        return;  
      }
    }
    if (pressed != -1) updateDis(pressed);
  }


  void setup() {
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    IrReceiver.begin(receiver, DISABLE_LED_FEEDBACK);
    Serial.begin(115200);
    randomSeed(analogRead(0));
    secret = random(1000, 10000);
    Serial.println("Game started, guess the 4-digit number");
    
  }

  void loop() {
    if (millis() - lastRefresh >=5) {
      lastRefresh = millis();
      refresh();
    }
    if (IrReceiver.decode()) {
      input();
      IrReceiver.resume();
    }
  }
