/*
  ESPFlash.h

  Copyright (c) 2020 Dale Giancono. All rights reserved..
  This file defines the ESPFlash class. PUT DESCRIPTION HERE.

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
#ifndef ESPFLASH_H_
#define ESPFLASH_H_

#include <FS.h>

template<class T>
class ESPFlash
{
  private:
    const char* fileName;    
  public:
    /* Creates the ESPFlash instance. In practice it starts SPIFFS and gets the file name. */
    ESPFlash(const char* fileName);
    /* Gets the number of elements of type T stored in the ESPFlash instance. 
     *  In practice it gets the ESPFlash file length in bytes using SPIFFS and divides the result 
     *  by the size of type T.
     */
    size_t length(void);
    
    /* Truncates the associated ESPFlash SPIFFS file and creates the first element containung type T. */
    /* Returns true if successful */
    bool set(const T data);
    /* Sets the element specified by the index parameter if it exists. */
    /* Returns true if successful */
    bool setElementAt(const T data, uint32_t index);
    /* Truncates the associated ESPFlash SPIFFS file and creates elements specified by Type T. */
    /* Returns true if successful */
    bool setElements(const T* data, size_t size = 1);

    /* Gets the first element of type T stored in the associated ESPFlash SPIFFS file if it exists */
    T get(void);
    /* Gets the element of type T as specified by the index parameter if it exists. */
    T getElementAt(uint32_t index);
    /* Gets elements of Type T from the start of the file stored in the associated ESPFlash SPIFFS file if it exists */
    /* Returns the number of elements "got" */
    size_t getFrontElements (T* data, size_t size);
    /* Gets elements of Type T from the end of the file stored in the associated ESPFlash SPIFFS file if it exists */
    /* Returns the number of elements "got" */
    size_t getBackElements(T* data, size_t size);
};

template<class T> ESPFlash<T>::ESPFlash(const char* fileName)
{
  this->fileName = fileName;
  SPIFFS.begin();      
  return;
};

template<class T> size_t ESPFlash<T>::length(void)
{
  File file;
  size_t sizeInBytes;
  size_t numberOfElements;

  sizeInBytes = 0;
  numberOfElements = 0;
  /* Open the file specified by the filename with read privileges*/
  file = SPIFFS.open(this->fileName, "r");
  
  if(file)
  {
    /* Get the size of the file in bytes */
    sizeInBytes = file.size();
    file.close();
    /* Calculate number of elements by dividing by size of type T */ 
    numberOfElements = sizeInBytes/sizeof(T);
  }
  
  return numberOfElements;
}

template<class T> bool ESPFlash<T>::set(const T data)
{
  File file;
  size_t bytesWritten;
  bool success; 
  
  bytesWritten = 0;
  success = false;
  /* Truncate and open file specified by fileName with write privileges*/
  file = SPIFFS.open(this->fileName, "w");
  
  if(file)
  {
    /* Write type T to SPIFFS */
    bytesWritten = file.write((uint8_t*)&data, sizeof(T));
    file.close();
    /* Check if successful by the number of bytes written */
    /* If not successful, delete the file that was possibly 
     *  created to make sure data does not end up being
     *  corrupted */
    if(bytesWritten == sizeof(T))
    {
      success = true;
    }
    else
    {
      SPIFFS.remove(this->fileName);
    }
  }
  
  return success;
};

template<class T> bool ESPFlash<T>::setElements(const T* data, size_t size)
{
  File file;
  size_t bytesWritten;
  size_t elementsSizeInBytes;
  bool success; 
  
  bytesWritten = 0;
  success = false;
  /* Truncate and open file specified by fileName with write privileges*/
  file = SPIFFS.open(this->fileName, "w");

  if(file)
  {
    elementsSizeInBytes = sizeof(T)*size;
    /* Write type T to SPIFFS */
    bytesWritten = file.write((uint8_t*)&data, elementsSizeInBytes);
    file.close();

    /* Check if successful by the number of bytes written */
    /* If not successful, delete the file that was possibly 
     *  created to make sure data does not end up being
     *  corrupted */
    if(bytesWritten == elementsSizeInBytes)
    {
      success = true;
    }
    else
    {
      SPIFFS.remove(this->fileName);
    }
  }
  
  return success;
}

template<class T> T ESPFlash<T>::get(void)
{
  T value;
  File file = SPIFFS.open(this->fileName, "r");
  file.read((uint8_t*)&value, sizeof(T));
  file.close();
  return value;
}

template<class T> T ESPFlash<T>::getElementAt(uint32_t index)
{
  T value;
  File file;
  value = (T)0;
  if(index < length())
  {
    file = SPIFFS.open(this->fileName, "r");
    file.seek(index*sizeof(T), SeekSet);
    file.read((uint8_t*)&value, sizeof(T));
    file.close();
  }
  return value;
}

template<class T> size_t ESPFlash<T>::getFrontElements(T* data, uint32_t size)
{
  File file = SPIFFS.open(this->fileName, "r");
  file.read((uint8_t*)data, sizeof(T)*size);
  file.close();
  return;
}
#endif /*ESPFLASHDATA_H_*/
