#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int ledPin = A0;
int buttonState = 0;

byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 186);

IPAddress server(66, 96, 231, 241);

void callback(char *topic, byte *payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

int breakLoop = 0;

// arrays containing [topic, pin number], cast it with (int)
char *ffbScrapperConv[] = {"pks/bunch_reception/ffb_scrapper_conveyor", (char)9};
char *sterilizer1[] = {"pks/bunch_reception/sterilizer_1", (char)8};
char *sterilizer2[] = {"pks/bunch_reception/sterilizer_2", (char)7};
char *tresher1[] = {"pks/threshing/thresher_1", (char)6};
char *tresher2[] = {"pks/threshing/thresher_2", (char)5};
char *eBConveyor[] = {"pks/empty_bunch/conveyor", (char)A6};
char *digester[] = {"pks/pressing/digester", (char)4};
char *screwPress1[] = {"pks/pressing/screw_press_1", (char)3};
char *screwPress2[] = {"pks/pressing/screw_press_2", (char)2};

void callback(char *topic, byte *payload, unsigned int length)
{

  byte *p = (byte *)malloc(length);

  memcpy(p, payload, length);

  if (*p == '1')
  {
    client.publish("pks/alarm/1", "Alarm on", length);
    digitalWrite(ledPin, HIGH);
  }
  else if (*p == '0')
  {
    client.publish("pks/alarm/1", "Alarm off", length);
    digitalWrite(ledPin, LOW);
  }

  free(p);
}

int state_ffbScrapperConv = 0;
int state_sterilizer1 = 0;
int state_sterilizer2 = 0;
int state_tresher1 = 0;
int state_tresher2 = 0;
int state_eBConveyor = 0;
int state_digester = 0;
int state_screwPress1 = 0;
int state_screwPress2 = 0;

int brkLoop_ffbScrapperConv = 0;
int brkLoop_sterilizer1 = 0;
int brkLoop_sterilizer2 = 0;
int brkLoop_tresher1 = 0;
int brkLoop_tresher2 = 0;
int brkLoop_eBConveyor = 0;
int brkLoop_digester = 0;
int brkLoop_screwPress1 = 0;
int brkLoop_screwPress2 = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);

  pinMode((int)ffbScrapperConv[1], INPUT);
  pinMode((int)sterilizer1[1], INPUT);
  pinMode((int)sterilizer2[1], INPUT);
  pinMode((int)tresher1[1], INPUT);
  pinMode((int)tresher2[1], INPUT);
  pinMode((int)eBConveyor[1], INPUT);
  pinMode((int)digester[1], INPUT);
  pinMode((int)screwPress1[1], INPUT);
  pinMode((int)screwPress2[1], INPUT);

  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient"))
  {
    client.publish("pks/info", "arduino is ready...");
    client.subscribe("pks/alarm/1");
  }
}

void loop()
{
  client.loop();
  //===============================================================
  state_ffbScrapperConv = digitalRead((int)ffbScrapperConv[1]);
  if (state_ffbScrapperConv == HIGH)
  {
    if (brkLoop_ffbScrapperConv >= 1)
    {
    }
    else
    {
      client.publish(ffbScrapperConv[0], "1");
      brkLoop_ffbScrapperConv = 1;
    }
  }

  else
  {
    if (brkLoop_ffbScrapperConv == 0)
    {
    }
    else
    {
      client.publish(ffbScrapperConv[0], "0");
      brkLoop_ffbScrapperConv = 0;
    }
  }
  //===============================================================
  state_sterilizer1 = digitalRead((int)sterilizer1[1]);
  if (state_sterilizer1 == HIGH)
  {
    if (brkLoop_sterilizer1 >= 1)
    {
    }
    else
    {
      client.publish(sterilizer1[0], "1");
      brkLoop_sterilizer1 = 1;
    }
  }

  else
  {
    if (brkLoop_sterilizer1 == 0)
    {
    }
    else
    {
      client.publish(sterilizer1[0], "0");
      brkLoop_sterilizer1 = 0;
    }
  }
  //===============================================================
  state_sterilizer2 = digitalRead((int)sterilizer2[1]);
  if (state_sterilizer2 == HIGH)
  {
    if (brkLoop_sterilizer2 >= 1)
    {
    }
    else
    {
      client.publish(sterilizer2[0], "1");
      brkLoop_sterilizer2 = 1;
    }
  }

  else
  {
    if (brkLoop_sterilizer2 == 0)
    {
    }
    else
    {
      client.publish(sterilizer2[0], "0");
      brkLoop_sterilizer2 = 0;
    }
  }
  //===============================================================
  state_tresher1 = digitalRead((int)tresher1[1]);
  if (state_tresher1 == HIGH)
  {
    if (brkLoop_tresher1 >= 1)
    {
    }
    else
    {
      client.publish(tresher1[0], "1");
      brkLoop_tresher1 = 1;
    }
  }

  else
  {
    if (brkLoop_tresher1 == 0)
    {
    }
    else
    {
      client.publish(tresher1[0], "0");
      brkLoop_tresher1 = 0;
    }
  }
  //===============================================================
  state_tresher2 = digitalRead((int)tresher2[1]);
  if (state_tresher2 == HIGH)
  {
    if (brkLoop_tresher2 >= 1)
    {
    }
    else
    {
      client.publish(tresher2[0], "1");
      brkLoop_tresher2 = 1;
    }
  }

  else
  {
    if (brkLoop_tresher2 == 0)
    {
    }
    else
    {
      client.publish(tresher2[0], "0");
      brkLoop_tresher2 = 0;
    }
  }
  //===============================================================
  state_eBConveyor = digitalRead((int)eBConveyor[1]);
  if (state_eBConveyor == HIGH)
  {
    if (brkLoop_eBConveyor >= 1)
    {
    }
    else
    {
      client.publish(eBConveyor[0], "1");
      brkLoop_eBConveyor = 1;
    }
  }

  else
  {
    if (brkLoop_eBConveyor == 0)
    {
    }
    else
    {
      client.publish(eBConveyor[0], "0");
      brkLoop_eBConveyor = 0;
    }
  }
  //===============================================================
  state_digester = digitalRead((int)digester[1]);
  if (state_digester == HIGH)
  {
    if (brkLoop_digester >= 1)
    {
    }
    else
    {
      client.publish(digester[0], "1");
      brkLoop_digester = 1;
    }
  }

  else
  {
    if (brkLoop_digester == 0)
    {
    }
    else
    {
      client.publish(digester[0], "0");
      brkLoop_digester = 0;
    }
  }
  //===============================================================
  state_screwPress1 = digitalRead((int)screwPress1[1]);
  if (state_screwPress1 == HIGH)
  {
    if (brkLoop_screwPress1 >= 1)
    {
    }
    else
    {
      client.publish(screwPress1[0], "1");
      brkLoop_screwPress1 = 1;
    }
  }

  else
  {
    if (brkLoop_screwPress1 == 0)
    {
    }
    else
    {
      client.publish(screwPress1[0], "0");
      brkLoop_screwPress1 = 0;
    }
  }
  //===============================================================

  state_screwPress2 = digitalRead((int)screwPress2[1]);
  if (state_screwPress2 == HIGH)
  {
    if (brkLoop_screwPress2 >= 1)
    {
    }
    else
    {
      client.publish(screwPress2[0], "1");
      brkLoop_screwPress2 = 1;
    }
  }

  else
  {
    if (brkLoop_screwPress2 == 0)
    {
    }
    else
    {
      client.publish(screwPress2[0], "0");
      brkLoop_screwPress2 = 0;
    }
  }
  //===============================================================
}
