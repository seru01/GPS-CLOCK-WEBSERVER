/*************************************
 * @file Main.cpp
 * @authors Reynald Bartiana
 * @version 0.1.0
 * @date Created On 07/11/2024
 * @date Last Modified On 07/11/2024
 * @copyright Copyright (c) 2024
 *************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

IPAddress IP(10, 0, 0, 1);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial GPS(1);

// SETUP
void setup() {
    GPS.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin); 
    Serial.begin(115200);
    
    WiFi.mode(WIFI_AP);
    String ssid = "GPSCLOCK:" + WiFi.macAddress(); 
    const char* password = "1234567890";

    WiFi.softAPConfig(IP, gateway, subnet);
    WiFi.softAP(ssid.c_str(), password);

    // Initialize SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
         request->send(SPIFFS, "/index.html", "text/html");
    });

    // BEGIN SERVER
    server.begin();
    Serial.println("Web Server Begins ...");
}

// LOOP
void loop() {
    while (GPS.available()) {
        gps.encode(GPS.read());

        if (gps.time.isUpdated()) {
            // Extract time in hh:mm:ss format
            char timeBuffer[10];
            sprintf(timeBuffer, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
            Serial.print("GPS TIME: ");
            Serial.println(timeBuffer);
        }
    }
}
