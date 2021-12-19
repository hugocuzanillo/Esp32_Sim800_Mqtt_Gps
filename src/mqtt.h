#include <PubSubClient.h>

int count = 0;

PubSubClient mqtt(client);

void publish(String message, String subTopic)
{

    if (message != "")
    {
        String toSend = message;
        int m = toSend.length();
        char msg[m + 1];
        strcpy(msg, toSend.c_str());

        String userIddId = JSONDocument["topic"];
        String variable = JSONDocument["variables"][0]["variable"];
        String topicAux = userIddId + variable + subTopic;
        int n = topicAux.length();
        char topic[n + 1];
        strcpy(topic, topicAux.c_str());
        mqtt.publish(topic, msg);
    }
}

void mqttConnect()
{
    String clientId = CLIENT_ID;
    clientId += String(random(0xffff), HEX);
    const char *user = JSONDocument["username"];
    const char *pass = JSONDocument["password"];
    
    if (modem.isGprsConnected())
    {
        Serial.print("Connecting to mqtt...");
        if (!mqtt.connect(clientId.c_str(), user, pass))
        {
            Serial.println("failed");
            count++;
            delay(100);
        }
        else
        {
            count = 0;
            Serial.println("success");
            // Nos suscribimos
            // mqtt.subscribe(topic);
            mqtt.loop();
        }
    }
    else
    {
        Serial.print(F("Reconnecting to "));
        Serial.print(F(apn));
        Serial.print(F("... "));

        if (!modem.gprsConnect(apn, gprsUser, gprsPass))
        {
            Serial.println(" fail");
            delay(100);
        }
        else
        {
            Serial.println("success");
        }

        Serial.println("Connecting to mqtt...");
        if (!mqtt.connect(clientId.c_str(), user, pass))
        {
            Serial.println("failed");
            delay(100);
            count++;
        }
        else
        {
            count = 0;
            Serial.println("success");
            // Nos suscribimos
            // mqtt.subscribe(topic);
            mqtt.loop();
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String incoming = "";

    for (unsigned int i = 0; i < length; i++)
    {
        incoming += (char)payload[i];
    }

    incoming.trim();

    Serial.print("Mensaje \"");
    Serial.print(incoming);
    Serial.print("\", "); 
    Serial.print("recibido con el topico \"");
    Serial.print(topic);
    Serial.print("\" ");
}

void setupMqtt()
{
    const char *broker = MQTT_SERVER;
    const int port = MQTT_PORT;
    mqtt.setServer(broker, port);
    mqtt.setCallback(callback);
}

bool getMqttCredentials()
{
    Serial.println("making POST request");
    String contentType = "application/x-www-form-urlencoded";
    String toSend = "dId=" + dId + "&password=" + webhook_pass;

    http.post(WEBHOOK_ENDPOINT, contentType, toSend);

    // read the status code and body of the response
    int statusCode = http.responseStatusCode();

    if (statusCode < 0)
    {
        Serial.println("Error sending request");
        http.stop();
        return false;
    }
    if (statusCode != 200)
    {
        Serial.println("Error in response");
        http.stop();
        return false;
    }
    if (statusCode == 200)
    {
        String response = http.responseBody();

        deserializeJson(JSONDocument, response);

        Serial.print("Status code: ");
        Serial.println(statusCode);
        Serial.print("Response: ");
        Serial.println(response);
        http.stop();
        return true;
    }
    http.stop();
    return false;
}