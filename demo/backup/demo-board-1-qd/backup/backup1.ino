#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int ledPin = 9;
int buttonPin = 8;
int buttonState = 0;

byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 86);
IPAddress server(192, 168, 0, 3);

void callback(char *topic, byte *payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

int breakLoop = A0;

// arrays containing [topic, pin number], cast it with (int)
char *ffbScrapperConv[] = {"pks/station/bunch_reception/ffb_scrapper_conveyor", (char)A0};
char *sterilizerA[] = {"pks/station/bunch_reception/sterilizer_a", (char)A1};
char *sterilizerB[] = {"pks/station/bunch_reception/sterilizer_b", (char)A2};
char *tresherA[] = {"pks/station/treshing/tresher_a", (char)A3};
char *tresherB[] = {"pks/station/treshing/tresher_b", (char)A4};
char *eBConveyor[] = {"pks/station/empty_bunch/conveyor", (char)A5};
char *digester[] = {"pks/station/pressing/digester", (char)A6};
char *screwPressA[] = {"pks/station/pressing/screw_press_a", (char)A7};
char *screwPressB[] = {"pks/station/pressing/screw_press_b", (char)2};

void callback(char *topic, byte *payload, unsigned int length)
{

  byte *p = (byte *)malloc(length);

  memcpy(p, payload, length);

  if (*p == '1')
  {
    client.publish("outTopic", "led Turned on", length);
    digitalWrite(ledPin, HIGH);
  }
  else if (*p == '0')
  {
    client.publish("outTopic", "led Turned off", length);
    digitalWrite(ledPin, LOW);
  }

  free(p);
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode((int)ffbScrapperConv[1], INPUT);

  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient"))
  {
    client.publish("outTopic", "hello world");
    client.subscribe("inTopic");
  }
}

void loop()
{
  client.loop();
  // (pinCall()) ?: return : continue; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<PADAHAL SYNTAX BENER??
  // (2 > 3) ?: 3;<<< jalan
}

void pinCall()
{

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    if (breakLoop >= 1)
    {
      return true;
    }
    client.publish("outTopic", "on");
    breakLoop = 1;
  }

  else
  {
    if (breakLoop == 0)
    {
      return true;
    }
    client.publish("outTopic", "off");
    breakLoop = 0;
  }
}