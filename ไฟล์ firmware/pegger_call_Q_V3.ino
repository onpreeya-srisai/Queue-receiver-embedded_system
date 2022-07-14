#include<WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

const char* ssid = "Aon132543";
const char* password = "06543218";

unsigned int timePeriod = 500;
unsigned long last_time = 0;
bool stateToogle = true;
bool callme = false;
int LED = 4;
unsigned int timePeriodweb = 1000;
unsigned long last_timeweb = 0;
int order;
int order_now;
String sensorReadings;
char mychar[6] = {'0', '1', '2', '3', '4', '5'};
int myid = 1;

void my_q(void)
{
  //////////////////my Q
  DynamicJsonDocument doc(5000); //บัฟเฟอร์ json
  String string_url = "http://warinnarat.com:8000/api/addoder/Client/1/"; //ลิงค์
  //string_url += String(mychar[i]); //นำคิวเสริมในลิงค์
  //string_url += "/";
  sensorReadings = httpGETRequest(string_url);
  //        sensorReadings = sensorReadings.substring(1, sensorReadings.length());
  Serial.println(sensorReadings);
  deserializeJson(doc, sensorReadings); //เปลี่ยนเป็น json

  JsonObject obj = doc.as<JsonObject>(); //เปลี่ยนเป็น json Object

  char buffer[500];
  serializeJsonPretty(obj, buffer); //ตัวแปลง object เป็น สตริงโดยเก็บไว้ใน arr char buffer จะออกมาเป็น key:value
  Serial.println(buffer);
  //strcat( menu_arr, String(obj["menu_name"]).c_str() );
  order = obj["order_id"];
  Serial.println(order);
}

void q_now(void)
{
  DynamicJsonDocument doc(5000); //บัฟเฟอร์ json
  String string_url = "http://warinnarat.com:8000/api/addoder/Q/"; //ลิงค์
  //string_url += String(mychar[i]); //นำคิวเสริมในลิงค์
  //string_url += "/";
  sensorReadings = httpGETRequest(string_url);
  sensorReadings = sensorReadings.substring(1, sensorReadings.length());
  Serial.println(sensorReadings);
  deserializeJson(doc, sensorReadings); //เปลี่ยนเป็น json

  JsonObject obj = doc.as<JsonObject>(); //เปลี่ยนเป็น json Object

  char buffer[500];
  serializeJsonPretty(obj, buffer); //ตัวแปลง object เป็น สตริงโดยเก็บไว้ใน arr char buffer จะออกมาเป็น key:value
  Serial.println(buffer);
  //  strcat( menu_arr, String(obj["menu_name"]).c_str() );
  order_now = obj["order_id"];
  Serial.println(order_now);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED, OUTPUT);
}

void loop() {
  if ((millis() - last_timeweb) > timePeriodweb)
  {
    last_timeweb = millis();
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      //my Q
      my_q();
      //Q_now
      q_now();
      delay(20);

    }
    //check web call me?
    for (int i = 0; i < 3; i++)
    {
      if (order == order_now and order_now != 0 and order != 0)
      {
        Serial.printf("myid = %d \n", order);
        callme = true;
        break;
      }else
      {
        callme = false;
        break;
      }
    }
  }

  //ส่วนของ LED กระพริบเรียกทุกๆ 0.5 วิ
  if ( millis() - last_time > timePeriod)
  {
    last_time = millis();
    if (callme == true)
    {
      digitalWrite(LED, stateToogle ? LOW : HIGH);
      stateToogle = !stateToogle;
      Serial.println("callme true");
    }
    if (callme == false)
    {
      digitalWrite(LED, LOW);
      stateToogle = false;
      Serial.println("callme false");
    }
//    else if (callme==false and stateToogle==true)
//    {
//      digitalWrite(LED,LOW);
//      stateToogle = false;
//      Serial.println("callme true");
//    }
    
  }
//  if (callme == true)
//  {
//    dacWrite(25, WaveFormTable[wave_type][i]);
//    i++;
//    if (i >= Num_Samples) i = 0;
//    adcValue = analogRead(adcPin);
//    Serial.printf("0, 4095, %d\n", adcValue);
//  }
}


String httpGETRequest(String serverName)
{
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
