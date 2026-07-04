//Tinkercad Schematic: https://www.tinkercad.com/things/7qOcg4XDwK0-traffic-lights?sharecode=fzNKd56A5XxppdHCA9GvkYYm9xBVuXa8MfA5d3dI7Y8

#define Red 10
#define Orange 9
#define Green 8
void setup()
{
  pinMode(Red, OUTPUT);
  pinMode(Orange, OUTPUT);
  pinMode(Green, OUTPUT);
}

void loop()
{
  digitalWrite(Green, LOW);
  digitalWrite(Red, HIGH);
  delay(7000);
  digitalWrite(Red, LOW);
  digitalWrite(Orange, HIGH);
  delay(1000);
  digitalWrite(Orange, LOW);
  digitalWrite(Green, HIGH);
  delay(7000);
}