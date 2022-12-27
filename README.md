# ESPFlash
An abstraction layer that simplifies the storing of vectorised data in the filesystem on the ESP8266 and ESP32.

ESPFlash is a lightweight library that was created to make SPIFFS usage simple and easy to understand on the ESP8266 and ESP32. 

## Features
- Simple template based interface to store and retrieve generic vectorised data in flash memory using SPIFFS with ESPFlash.
- Implementation of simple SPIFFS based integer counter with ESPFlashCounter.
- Implementation of simple SPIFFS based string storer using ESPFlashString.

## Why Would I Want This?
- ESPFlash enables the storage of generic data in a persistant matter. This is data that will exist through multiple power cycles or software resets.
- ESPFlash takes care of a lot of the nastiness that exists when using SPIFFS using the concept of vectorised data. This includes the following functionailty:
  - Automatically starts SPIFFS if it has not already been started.
  - Automatically truncates filenames that are over 32 characters in length.
  - Calculates the number of generic "elements" stored in a file.
  - Overwrites elements.
  - Appends elements.
  - Gets elements.
  - Clears elements.
  
## One Thing to Keep In Mind
SPIFFS is not particularly fast. It is not designed to be used in a manner that requires high-throughput data input/output. ESPFlash does not significantly increase the performance issues of SPIFFS, but you should consider if SPIFFS usage is suitable for your application. 
  
## Installation
Download this file as a zip, and extract the resulting folder into your Arduino Libraries folder. See [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries). Alternatively, use the Arduino IDE and library manager to find and install ESPFlash.

## Examples
The blog post [ESPFlash: An Arduino Library for Storing Data in the ESP Filesystem](https://dalegi.com/2020/04/22/espflash-an-arduino-library-for-storing-data-in-the-esp-filesystem/) contains some useful comparisons between ESPFlash usage and SPIFFS usage. In addition, here are some basic ESPFlash example:
- Simple ESPFlash integer example - Create ESPFlash instance with file name of "exampleInteger". Set a single elements value, and get it back.
```c++
SPIFFS.begin()
ESPFlash<int> espFlashInteger("/exampleInteger");
espFlashInteger.set(10);
int testInteger = espFlashInteger.get();
```
- Simple ESPFlash vector example - Create ESPFlash instance with file name of "exampleInteger". Append 10 randomly generated elements. Get the elements back.

```c++
SPIFFS.begin()
ESPFlash<int> espFlashInteger("/exampleArray");
for(int ii = 0; ii < 10; ii++)
{
  espFlashInteger.append(random(100));
}
int testGet[10];
espFlashInteger.getFrontElements(testGet, sizeof(testGet));
```
- Simple ESPFlash vector example - Create ESPFlash instance with file name of "exampleInteger". Append 50 randomly generated elements. Get the last 10 elements back.

```c++
SPIFFS.begin()
ESPFlash<int> espFlashInteger("/exampleArray");
for(int ii = 0; ii < 50; ii++)
{
  espFlashInteger.append(random(100));
}
int testGet[10];
espFlashInteger.getBackElements(testGet, sizeof(testGet));
```

- Simple ESPFlashCounter example - Create ESPFlashCounter instance with file name of "exampleCounter". Increment the counter. Get the Counter

```c++
SPIFFS.begin()
ESPFlashCounter exampleCounter("/exampleCounter");
exampleCounter.increment();
int testGet = exampleCounter.get();
```

- Simple ESPFlashString example - Create ESPFlashString instance with file name of "exampleString". Set a string. Get the string.

```c++
SPIFFS.begin()
ESPFlashCounter exampleString("/exampleString");
exampleString.set("Hello!");
String string = exampleString.get();
```

## Further Examples  
[millisArray.ino](examples/millisArray/millisArray.ino)

[powerCycleCounter.ino](examples/powerCycleCounter/powerCycleCounter.ino)

[ssidStorage.ino](examples/ssidStorage/ssidStorage.ino)


