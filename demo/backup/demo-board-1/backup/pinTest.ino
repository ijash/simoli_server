
//use pull down/up resistor
int pushPin = A0;

void setup()
{
  pinMode(pushPin, INPUT);
  Serial.begin(9600);
}
void loop()
{
  int value = digitalRead(pushPin);
  Serial.println("value:");
  Serial.println(value);
  delay(100);
}