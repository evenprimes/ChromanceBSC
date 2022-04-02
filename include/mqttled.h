

#ifndef MQTTLED_H_
#define MQTTLED_H_ 1

// #define MQTTLED_DEBUG 1

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ledhelpers.h"
#include "localsecrets.h"

// These need to be defined somewhere!
// const char *ssid = "localssid";
// const char *password = "wifipassword";

const char *mqtt_server = "192.168.1.60";
const uint16_t mqtt_port = 1883;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
#ifdef MQTTLED_DEBUG
    Serial.print("Message arrived on Topic: ");
    Serial.println(topic);
#endif

    String message = "";
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

#ifdef MQTTLED_DEBUG
    Serial.print("Message: ");
    Serial.println(message);
#endif

    String s_topic = topic;
    if (s_topic.endsWith("mode"))
    {
        g_mode = message.toInt();
    }
    else if (s_topic.endsWith("pattern"))
    {
        g_pattern = message.toInt();
    }
    else if (s_topic.endsWith("palette"))
    {
        g_palette = message.toInt();
    }

    // String str_msg = String(message);
    // if (str_msg.equals("ON"))
    // {
    //     digitalWrite(RELAY, HIGH);
    //     digitalWrite(LED, HIGH);
    // }
    // else if (str_msg.equals("OFF"))
    // {
    //     digitalWrite(RELAY, LOW);
    //     digitalWrite(LED, LOW);
    // }
}

void wifi_init()
{
    int tries = 0;

    WiFi.mode(WIFI_STA);
#ifdef MQTTLED_DEBUG
    Serial.print("Hostname: ");
    Serial.println(WiFi.hostname());
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
#endif

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
#ifdef MQTTLED_DEBUG
        WiFi.printDiag(Serial);
        Serial.println();
        tries++;
#endif
        delay(1000);
    }

#ifdef MQTTLED_DEBUG
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("WiFi connected (%d tries)\n", tries);
        Serial.print("     IP: ");
        Serial.println(WiFi.localIP());
        Serial.print(" subnet: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("gateway: ");
        Serial.println(WiFi.gatewayIP());
    }
#endif

    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(mqtt_callback);
    mqtt_client.connect("LEDSculpture", NULL, NULL);
    mqtt_client.subscribe("ledsculpture/mode");
    mqtt_client.subscribe("ledsculpture/pattern");
    mqtt_client.subscribe("ledsculpture/palette");
}

#endif
