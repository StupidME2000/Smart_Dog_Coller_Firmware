#include "WiFi.h"

String SOS_NUM; // Add a number on which you want to receive call or SMS

boolean stringComplete = false;
String inputString = "";
String fromGSM = "";
bool CALL_END = 1;
char* response = " ";
String res = "";
int c = 0;


void setup()
{
  Serial.begin(115200);                      // For Serial Monitor
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // For A9G Board

  WiFi.mode(WIFI_OFF);  // WiFi OFF
  btStop();   // Bluetooth OFF

  // Waiting for A9G to setup everything for 20 sec
  delay(20000);

  Serial1.println("AT");               // Just Checking
  delay(1000);

  Serial1.println("AT+GPS = 1");      // Turning ON GPS
  delay(1000);

  Serial1.println("AT+GPSLP = 2");      // GPS low power
  delay(1000);

  Serial1.println("AT+CMGF = 1");
  delay(1000);

  Serial1.println("AT+CSMP  = 17,167,0,0 ");
  delay(1000);

  Serial1.println("AT+CPMS = \"SM\",\"ME\",\"SM\" ");
  delay(1000);

  Serial1.println("AT+SNFS = 2");
  delay(1000);

//  Serial1.println("AT+CLVL = 8");
//  delay(1000);
 
}

void loop()
{
  //listen from GSM Module
  if (Serial1.available())
  {
    char inChar = Serial1.read();
    

    if (inChar == '\n') {
      
      //check the state
      if (fromGSM == "SEND LOCATION\r")
      {
        Get_gmap_link(0);  // Send Location without Call
        SOS_NUM = ""; // Reseting the SOS_NUM 
      }

      
      //write the actual response
      //Serial.println("This is from GSM --> ");
      Serial.println(fromGSM);
      //clear the buffer
      fromGSM = "";

    }

    else if (inChar == ' '){

      if (fromGSM == "+CMT:"){
        //Serial.println("Inside +CMT:");
        //Serial.print(fromGSM);
        
        
        char num;
        while(num != ','){

          num = Serial1.read();
          SOS_NUM += num;         
        }
        Serial.println("SMS Received from --> ");
        Serial.println(SOS_NUM);
        
        
      }
    

      else{
        fromGSM += inChar;
        
      }

//      Serial.println(fromGSM);
//      //clear the buffer
//      fromGSM = "";  
    }
    else
      {
        fromGSM += inChar;
      }
    delay(20);
  }

  if (Serial.available()) {
      int inByte = Serial.read();
      Serial1.write(inByte);
    }

}


void Get_gmap_link(bool makeCall)
{

  delay(1000);
  Serial1.println("AT+LOCATION = 2");
  Serial.println("AT+LOCATION = 2");

  while (!Serial1.available());
  while (Serial1.available())
  {
    char add = Serial1.read();
    res = res + add;
    delay(1);
  }

  res = res.substring(17, 38);
  response = &res[0];

  Serial.print("Recevied Data - "); Serial.println(response); // printin the String in lower character form
  Serial.println("\n");

  if (strstr(response, "GPS NOT"))
  {
    Serial.println("No Location data");
    //------------------------------------- Sending SMS without any location
    Serial1.println("AT+CMGF=1");
    delay(1000);
    Serial1.println("AT+CMGS=" + SOS_NUM + "\r");
    delay(1000);

    Serial1.println ("Unable to fetch location. Please try again");
    delay(1000);
    Serial1.println((char)26);
    delay(1000);
  }
  else
  {

    int i = 0;
    while (response[i] != ',')
      i++;

    String location = (String)response;
    String lat = location.substring(2, i);
    String longi = location.substring(i + 1);
    Serial.println(lat);
    Serial.println(longi);

    String Gmaps_link = ( "http://maps.google.com/maps?q=" + lat + "+" + longi); //http://maps.google.com/maps?q=38.9419+-78.3020
    //------------------------------------- Sending SMS with Google Maps Link with our Location
    Serial1.println("AT+CMGF=1");
    delay(1000);
    Serial1.println("AT+CMGS=" + SOS_NUM + "\r");
    delay(1000);

    Serial1.println ("I'm here " + Gmaps_link);
    delay(1000);
    Serial1.println((char)26);
    delay(1000);

    Serial1.println("AT+CMGD=1,4"); // delete stored SMS to save memory
    delay(5000);

    
  }
  response = "";
  res = "";
}
