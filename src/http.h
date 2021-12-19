#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

String resource = WEBHOOK_ENDPOINT;
const char server[] = WEBHOOK_SERVER;
const int port = WEBHOOK_PORT;

String dId = "12345";
String webhook_pass = "QH8U4HEKPZ";

HttpClient http(client, server, port);
static DynamicJsonDocument JSONDocument(2048);

