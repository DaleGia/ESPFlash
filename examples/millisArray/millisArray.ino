/*
  millisArray.ino

  Copyright (c) 2020 Dale Giancono. All rights reserved..
  This file is a an application to demonstrte the abilities of the 
  ESPFlash Library. It uses an ESPFlash<uint32_t> instance to store
  10 millis values in an array.

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

#include "ESPFlash.h"

#define ARRAY_FILEPATH  "/millisArray"

ESPFlash<uint32_t> millisArray(ARRAY_FILEPATH);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Begining millisArray example...");

  for(int ii = 0; ii < 10; ii++)
  {
    millisArray.append(millis());
    for(int jj = 0; jj < millisArray.length(); jj++)
    {
      Serial.printf("%d ", millisArray.getElementAt(jj));
    }
    Serial.println();
  }

  Serial.println("Example finished...");
}

void loop() 
{

}
