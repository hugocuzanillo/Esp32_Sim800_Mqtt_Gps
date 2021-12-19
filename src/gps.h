#include <TinyGPS++.h>

#define SerialGPS Serial2

// GPS details

double latitude = 0;
double longitude = 0;
int speed = 0;
String speed_str = "";
String lat_lng_srt = "";

static const int RXPin = 34, TXPin = 35;

TinyGPSPlus gps;

void setupGps()
{
  SerialGPS.begin(9600, SERIAL_8N1, RXPin, TXPin);
  delay(10);
}

void gpsData()
{
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        DynamicJsonDocument JSONDocument_2(2048);
        JSONDocument_2["lat"] = gps.location.lat();
        JSONDocument_2["lng"] = gps.location.lng();
        lat_lng_srt = "";
        serializeJson(JSONDocument_2, lat_lng_srt);
        speed = round(gps.speed.kmph());
        speed_str = String(speed);
      }
    }
}
