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
  public:
    /* Creates the ESPFlash instance. In practice it starts SPIFFS and gets the file name. */
    ESPFlash(const char* fileName);
    /* Gets the number of elements of type T stored in the ESPFlash instance. 
     *  In practice it gets the ESPFlash file length in bytes using SPIFFS and divides the result 
     *  by the size of type T.
     */
    size_t length(void);
    
    /* Truncates the associated ESPFlash SPIFFS file and creates the first element containing type T. */
    /* Returns true if successful */
    bool set(const T data);
    /* Sets the element specified by the index parameter if it exists. */
    /* Returns true if successful */
    bool setElementAt(const T data, uint32_t index);
    /* Truncates the associated ESPFlash SPIFFS file and creates elements specified by Type T. */
    /* Returns true if successful */
    bool setElements(const T* data, size_t size);

    /* Opens the associated ESPFlash SPIFFS file and appends element containing type T. */
    /* Returns true if successful */
    bool append(const T data);
    /* Opens the associated ESPFlash SPIFFS file and appends elements specified by Type T. */
    /* Returns true if successful */
    bool appendElements(const T* data, size_t size);
    
    /* Gets the first element of type T stored in the associated ESPFlash SPIFFS file if it exists */
    T get(void);
    /* Gets the element of type T as specified by the index parameter if it exists. */
    T getElementAt(uint32_t index);
    /* Gets elements of Type T from the start of the file stored in the associated ESPFlash SPIFFS file if it exists */
    /* Returns the number of elements "got" */
    bool getFrontElements (T* data, size_t size);
    /* Gets elements of Type T from the end of the file stored in the associated ESPFlash SPIFFS file if it exists */
    /* Returns the number of elements "got" */
    bool getBackElements(T* data, size_t size);

  private:
    const char* fileName;  
    enum WRITE_MODE
    {
      OVERWRITE = 0,
      APPEND,
    };
    
    bool writeElement(const T data, enum WRITE_MODE mode);
    bool writeElements(const T* data, size_t size, enum WRITE_MODE mode);
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
  return writeElement(data, WRITE_MODE::OVERWRITE); 
};

template<class T> bool ESPFlash<T>::setElements(const T* data, size_t size)
{
  return writeElements(data, size, WRITE_MODE::OVERWRITE);
};

template<class T> bool ESPFlash<T>::append(const T data)
{
  return writeElement(data, WRITE_MODE::APPEND); 
};

template<class T> bool ESPFlash<T>::appendElements(const T* data, size_t size)
{
  return writeElements(data, size, WRITE_MODE::APPEND);
};

template<class T> T ESPFlash<T>::get(void)
{
  T value;
  File file = SPIFFS.open(this->fileName, "r");
  file.read((uint8_t*)&value, sizeof(T));
  file.close();
  return value;
};

template<class T> T ESPFlash<T>::getElementAt(uint32_t index)
{
  T value;
  size_t bytesRead;
  File file;
  
  bytesRead = 0;
  value = (T)0; 
  if(index < length())
  {
    file = SPIFFS.open(this->fileName, "r");
    if(file)
    {
      file.seek(index*sizeof(T), SeekSet);
      bytesRead = file.read((uint8_t*)&value, sizeof(T));
      file.close();
      /* Check if successful by the number of bytes read */
      if(bytesRead != sizeof(T))
      {
        /* An error has occured */
        value = (T)0; 
      }
    }
  }
  
  return value;
};

template<class T> bool ESPFlash<T>::getFrontElements(T* data, uint32_t size)
{
  File file;
  size_t numberOfBytes;
  size_t bytesRead;
  bool success;
  
  success = false;
  numberOfBytes = sizeof(T)*size;;
  bytesRead = 0;
  /* Open the file specified by the filename with read privileges*/
  file = SPIFFS.open(this->fileName, "r");
  if(size < length())
  {
    if(file)
    {
      bytesRead = file.read((uint8_t*)data, numberOfBytes);
      file.close();
  
      /* Check if successful by the number of bytes read */
      if(bytesRead == numberOfBytes)
      {
        success = true;
      }
    }
  }
  return success;
};

template<class T> bool ESPFlash<T>::getBackElements(T* data, uint32_t size)
{
  File file;
  size_t numberOfBytes;
  size_t firstElementIndex;
  size_t bytesRead;
  bool success;
  
  success = false;
  bytesRead = 0;
  numberOfBytes = sizeof(T)*size;
  firstElementIndex = 0;
  /* Open the file specified by the filename with read privileges*/
  file = SPIFFS.open(this->fileName, "r");
  if(size < length())
  {
    firstElementIndex = file.size() - numberOfBytes;
    if(file)
    {
      file.seek(firstElementIndex, SeekSet);
      bytesRead = file.read((uint8_t*)data, numberOfBytes);
      file.close();
  
      /* Check if successful by the number of bytes read */
      if(bytesRead == numberOfBytes)
      {
        success = true;
      }
    }
  }
  return success;
};

template<class T> bool ESPFlash<T>::writeElement(const T data, WRITE_MODE mode)
{
    File file;
  size_t bytesWritten;
  bool success; 
  
  bytesWritten = 0;
  success = false;
  /*open file specified by fileName with write privileges*/
  if(WRITE_MODE::OVERWRITE == mode)
  {
    file = SPIFFS.open(this->fileName, "w");
  }
  else if(ESPFlash::APPEND == mode)
  {
    file = SPIFFS.open(this->fileName, "a");
  }
  else
  {
    /* error */
    return success;
  }
  
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

template<class T> bool ESPFlash<T>::writeElements(const T* data, size_t size, enum WRITE_MODE mode)
{
  File file;
  size_t bytesWritten;
  size_t elementsSizeInBytes;
  bool success; 
  
  bytesWritten = 0;
  success = false;

  /*open file specified by fileName with write privileges*/
  if(WRITE_MODE::OVERWRITE == mode)
  {
    file = SPIFFS.open(this->fileName, "w");
  }
  else if(WRITE_MODE::APPEND == mode)
  {
    file = SPIFFS.open(this->fileName, "a");
  }
  else
  {
    /* error */
    return success;
  }

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
};
#endif /*ESPFLASHDATA_H_*/
