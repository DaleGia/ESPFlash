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
  T testData[10];
  T checkGet;
  T checkGetFront[5];
  T checkGetBack[5];

  /* Prepare test data */
  for(int ii = 0; ii < 10; ii++)
  {
    testData[ii] = std::numeric_limits<T>::max()/(ii+1);
    test.append(testData[ii]);  
  }
  
  Serial.println("get TEST ");
  checkGet = testData[0];
  if(test.get() == checkGet)
  {
    Serial.println("SUCCESS!!!");
  }
  else
  {
    Serial.println("FAILED...");
    Serial.print("test.getElementAt(ii) ");
    Serial.println(test.get());
    Serial.print("checkGet ");
    Serial.println(checkGet);
  }
    
  Serial.println("getFrontElements TEST ");
  if(!test.getFrontElements(checkGetFront, 5))
  {
    Serial.println("getFrontElements ERROR ");
  }
  for(int ii = 0; ii < 5; ii++)
  {
    if(checkGetFront[ii] == testData[ii])
    {
      Serial.println("SUCCESS!!!");
    }
    else
    {
      Serial.println("FAILED...");
      Serial.print("checkGetFront[");
      Serial.print(ii);
      Serial.print("] ");
      Serial.println(checkGetFront[ii]);

      Serial.print("testData[");
      Serial.print(ii);
      Serial.print("] ");
      Serial.println(testData[ii]);
    }
  }

  Serial.println("getBackElements TEST ");
  if(!test.getBackElements(checkGetBack, 5))
  {
    Serial.println("checkGetBack ERROR ");
  }
  for(int ii = 0; ii < 5; ii++)
  {
    if(checkGetBack[ii] == testData[ii+5])
    {
      Serial.println("SUCCESS!!!");
    }
    else
    {
      Serial.println("FAILED...");
      Serial.println(checkGetBack[ii]);
      Serial.print("checkGetBack[");
      Serial.print(ii);
      Serial.print("] ");
      Serial.print("testData[");
      Serial.print(ii+5);
      Serial.print("] ");
      Serial.println(testData[ii+5]);
    }
  }
  
  Serial.println("getElementAt TEST ");
  for(int ii = 0; ii < test.length(); ii++)
  {
    if(test.getElementAt(ii) == testData[ii])
    {
      Serial.println("SUCCESS!!!");
    }
    else
    {
      Serial.println("FAILED...");
      Serial.print("test.getElementAt(");
      Serial.print(ii);
      Serial.print(") ");
      Serial.println(test.getElementAt(ii));
      Serial.print("testData[ii");
      Serial.print(ii);
      Serial.print("] ");
      Serial.println(testData[ii]);
    }
  }
  Serial.println();
} 
 
