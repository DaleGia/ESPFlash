/*
ESPFlashString.h

Copyright (c) 2020 Dale Giancono. All rights reserved..
This file defines the ESPFlashString class. It provides a user friendly
way to store strings in SPIFFs.

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

#ifndef ESPFLASHSTRING_H_
#define ESPFLASHSTRING_H_

#include "ESPFlash.h"

class ESPFlashString
{
  public:
    ESPFlashString(const char* fileName, String defaultString = String());
    ESPFlashString(const char* fileName, const char* defaultString);
    void set(String string);
    void set(const char* string);
    void reset(void);
    String get(void);
  private:
    String defaultString;
    ESPFlash<char> espFlash;
};

ESPFlashString::ESPFlashString(const char* fileName, String defaultString)
{
  this->defaultString = defaultString;
  this->espFlash.setFileName(fileName);
  if(!SPIFFS.exists(fileName))
  {
    set(this->defaultString);
  }
}

ESPFlashString::ESPFlashString(const char* fileName, const char* defaultString)
{
  this->defaultString = defaultString;
  this->espFlash.setFileName(fileName);
  if(!SPIFFS.exists(fileName))
  {
    set(this->defaultString);
  }
}

void ESPFlashString::set(String string)
{
  espFlash.setElements(string.c_str(), string.length()+1);
}

void ESPFlashString::set(const char* string)
{
  espFlash.setElements(string, strlen(string)+1);
}

void ESPFlashString::reset(void)
{
  set(this->defaultString);
  return;
}

String ESPFlashString::get(void)
{
  char string[espFlash.length()];
  espFlash.getFrontElements(string, espFlash.length());
  return String(string);
}

#endif /*ESPFLASHSTRING_H_*/
