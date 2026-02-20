#define RedLed 9
#define GreenLed 8
#define TrigPin 3
#define EchoPin 2

float position() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  float duration = pulseIn(EchoPin, HIGH);
  return (duration * 0.034)/2;
}

void setup()
{
  Serial.begin(9600);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void loop()
{
  float int_time = millis();
  float pos1 = position();
  delay(1000);
  float pos2 = position();
  float fin_time = millis();
  float total_time = (fin_time - int_time)/1000.0;
  float speed = abs(pos1-pos2)/total_time; // average speed
  Serial.print("Average Speed ");
  Serial.println(speed);
  if (pos1>0 && pos2>0) {
  	if (speed >25) { // speed limit of 25cm/s
    	digitalWrite(RedLed, HIGH);
    	digitalWrite(GreenLed, LOW);
  	}
  	else if (speed > 2.0) {
    	digitalWrite(RedLed, LOW);
    	digitalWrite(GreenLed, HIGH);
  	}
  	else {
    	digitalWrite(RedLed, LOW);
    	digitalWrite(GreenLed, LOW);
  	}
  }
  else {
    Serial.println("error");
  }
  delay(50);
}