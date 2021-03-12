

InOut::InOut(char initName[], char *initTopic[], char *pinSelect[])
// : name(initName),
//   topic(initTopic),
//   pinNumber(pinSelect)
{
  name = initName;
}

// second initializer array must be bounded in order to work.
//[row for a node][column for data in each row A2]
char *inputNode[][8] = {
    {"pks/station/pressing/digester", (char)A0, (char)0},
    {"pks/station/bunch_reception/sterilizer_a", (char)A1, (char)0}

};

void checkNode(char *refNode[])
{
  Serial.println(refNode[1]);
}

// void initNodes(char *refNode[])
// {
//   // now only coloumn select
//   pinMode((int)refNode[1], INPUT);
// }

void setup()
{
  pinMode((int)inputNode[0][1], INPUT);
  Serial.begin(9600);
}

void loop()
{
  int value = digitalRead((int)inputNode[0][1]);
  Serial.println("value:");
  checkNode(inputNode[0]);
  delay(2500);
}