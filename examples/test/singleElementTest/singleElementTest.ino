#include "ESPFlash.h"
#include <limits>

void setup()
{ 
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting!");
  Serial.println();
  SPIFFS.begin();
  Serial.println("Formatting SPIFFS. Will take a while...");
  SPIFFS.format();

  unitTest<bool>("bool");
  unitTest<char>("char");
  unitTest<signed char>("signed char");
  unitTest<unsigned char>("unsigned char");
  unitTest<wchar_t>("wchar_t");
  unitTest<int8_t>("int8_t");
  unitTest<char16_t>("char16_t");
  unitTest<char32_t>("char32_t");
  unitTest<short>("short");
  unitTest<unsigned short>("unsigned short");
  unitTest<int>("int");
  unitTest<unsigned int>("unsigned int");
  unitTest<long>("long");
  unitTest<unsigned long>("unsigned long");

  unitTest<int16_t>("int16_t");
  unitTest<int32_t>("int32_t");
  unitTest<uint8_t>("uint8_t");
  unitTest<uint16_t>("uint16_t");
  unitTest<uint32_t>("uint32_t");
  
  unitTest<float>("float");
  unitTest<double>("double");

  Serial.println("Done!");
}

void loop()
{
}


template<typename T> void unitTest(const char* testName)
{
  Serial.print(testName);
  Serial.println(" unit test... ");
 
  String filename = "/";
  filename += testName;
  ESPFlash<T> test(filename.c_str());
  test.set(std::numeric_limits<T>::min());

  Serial.print("MIN TEST ");
  if(test.get() == std::numeric_limits<T>::min())
  {
    Serial.println("SUCCESS!!!");
  }
  else
  {
    Serial.println("FAILED...");
    Serial.print("test.get() ");
    Serial.println(test.get());
    Serial.print("std::numeric_limits<T>::min() ");
    Serial.println(std::numeric_limits<T>::min());
  }

  test.set(std::numeric_limits<T>::max());

  Serial.print("MAX TEST ");
  if(test.get() == std::numeric_limits<T>::max())
  {
    Serial.println("SUCCESS!!!");
  }
  else
  {
    Serial.println("FAILED...");
    Serial.println(test.get());
    Serial.print("std::numeric_limits<T>::max() ");
    Serial.println(std::numeric_limits<T>::max());
  }
} 
 
