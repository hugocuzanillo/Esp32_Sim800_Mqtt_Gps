#include <Arduino.h>

#include "Colors.h"
#include "gprs.h"
#include "http.h"
#include "mqtt.h"
#include "gps.h"
#include "env.h"

void setup()
{
  Serial.begin(921600);
  delay(10);

  setupGps();

  setupModem();

  connectModem();

  setupMqtt();
}

void loop()
{

  if (!mqtt.connected())
  {
    if (!getMqttCredentials())
    {
      delay(1000);
      ESP.restart();
    }

    mqttConnect();
    if (count > 10)
    {
      ESP.restart();
    }
  }
  else
  {
    gpsData();

    long lastMsg = 1000;
    int lapsused = JSONDocument["variables"][3]["variableSendFreq"];
    lapsused *= 100;
    long now = millis();

    if (now - lastMsg > lapsused)
    {
      lastMsg = now;
      publish(lat_lng_srt, "/sdata");
    }
  }
  mqtt.loop();
}