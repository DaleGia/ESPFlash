/*
ESPFlashCounter.h

Copyright (c) 2020 Dale Giancono. All rights reserved..
This file defines the ESPFlashCounter class. It can be used as a way
of keeping count of something, with the counter value stored in flash 
using SPIFFS. It's value can be set to any integer value, or the values
can incremented and decremented. It can be reset to initial starting value
(or 0 if that is not defined), and can be retreived in String or int form.
It is particularly useful if I counter value is required through power 
cycles.

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
#ifndef ESPFLASHCOUNTER_H_
#define ESPFLASHCOUNTER_H_

#include "ESPFlash.h"

class ESPFlashCounter
{
  public:
    ESPFlashCounter(const char* name, int initialValue = 0);
    void decrement(void);    
    void increment(void);
    void set(int value);
    void reset(void);
    int get(void);
  private:
    int initialValue;
    ESPFlash<int> espFlash;
};

ESPFlashCounter::ESPFlashCounter(const char* fileName, int initialValue)
{
  this->initialValue = initialValue;
  this->espFlash.setFileName(fileName);
  if(!SPIFFS.exists(fileName))
  {
    set(this->initialValue);
  }
}

void ESPFlashCounter::decrement(void)
{
  int value;
  value = espFlash.get();
  espFlash.set(--value);
  return;
}
void ESPFlashCounter::increment(void)
{
  int value;
  value = espFlash.get();
  espFlash.set(++value);
  return;
}
void ESPFlashCounter::set(int value)
{
  espFlash.set(value);
}
void ESPFlashCounter::reset(void)
{
  set(this->initialValue);
  return;
}

int ESPFlashCounter::get(void)
{
  int value;
  value = espFlash.get();
  return value;
}

#endif /*ESPFLASHCOUNTER_H_*/
