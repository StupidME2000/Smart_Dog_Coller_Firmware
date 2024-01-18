#include <ArduinoHttpClient.h>
#include<FirebaseArduino.h>

const char FIREBASE_HOST[] = ""
const String FIREBASE_AUTH =""
const String Firebase_path =""
const int SSL_PORT         = 

void setup ()
{
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  }

void loop ()
{
  Firebase.pushString("");
  Firebase.pushString("");
    if (Firebase.failed())
    {
      Serial.print("pushing/log failed:");
      Serial.println(Firebase.error());
      return;
    }
  }
