/*
  ssidStorage.ino

  Copyright (c) 2020 Dale Giancono. All rights reserved..
  This file is a an application to demonstrte the abilities of the 
  ESPFlash Library. It uses an ESPFlashString instance to store
  the value of an SSID. An AsyncWebServer provides a way to change the
  ssid value with a browser.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "ESPFlashString.h"

#define DEFAULT_SSID                  "ESPFlashString Test"
#define SSID_FILEPATH                 "/ssid"

static const char _SSIDEDIT[] = "<html><body><form action=\"/ssidedit\">New SSID: <input type=\"text\" value=\"\" name=\"SSID\"><input type=\"submit\" name=\"submit\"></form></html></body>";

/* Create webserver instance for serving the StringTemplate example. */
AsyncWebServer server(80);

void setup() 
{  
  ESPFlashString ssid(SSID_FILEPATH, DEFAULT_SSID);
  Serial.begin(115200);
  /* Configure access point with static IP address */
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(
    IPAddress(192, 168, 4, 1),
    IPAddress(192, 168, 4, 1),
    IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid.get());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/html", _SSIDEDIT);
  });

  server.on("/ssidedit", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebParameter* p = request->getParam("SSID");
    if(p->value() != "")
    {
      ESPFlashString ssid(SSID_FILEPATH);
      ssid.set(p->value());
      WiFi.softAPdisconnect(true);
      WiFi.softAP(ssid.get());  
    }
    else
    {
      request->redirect("/");
    }
  });

  server.begin();
}

void loop() 
{
}
