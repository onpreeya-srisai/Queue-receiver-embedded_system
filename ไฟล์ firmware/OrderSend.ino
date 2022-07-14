#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <vector>
#include "ArduinoJson.h"
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define sw 33
#define downSwitch 17
#define addSwitch 5
#define printSwitch 16

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

bool state = true;
bool state_getmenu = false;
unsigned int timePeriod = 1000;
unsigned long last_time = 0;

unsigned int timePeriod_getmenu = 10000;
unsigned long last_getmenu = 0;
char mychar[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A'};
String sensorReadings;

std::vector<String> menu;
std::vector<String> order_cost;



int orders = 0;
int target = 0;
int order[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int costeach[10] = {30, 30, 30, 30, 69, 69, 69, 30, 45, 45};
String totalOrder = "";
int printcheck = 0 ;

//----------------Get Switch----------------
/*
void IRAM_ATTR addValue() 
{
  order[orders] = order[orders] + 1;
  Serial.println(order[orders]);
  delay(500);
}

void IRAM_ATTR ChooseValue() 
{
  if (orders < 9) {
        orders += 1 ;
      }
      else {
        orders = 0;
      }

      if (target == 0) {
        target = 16 ;
      }
      else {
        target = 0;
      }

      Serial.println(orders);
      delay(500);
}
*/
//----------------WIFI----------------

const char* ssid = "Pusskorn2";
const char* password = "Sirapop1973";
void setup()
{
  Serial.begin(115200);
  pinMode(downSwitch, INPUT_PULLUP);
  pinMode(addSwitch, INPUT_PULLUP);
  pinMode(printSwitch, INPUT_PULLUP);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  // Test Draw Char
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("ESP32 I2C MCP23017"));
  display.display();



  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  pinMode(sw, INPUT_PULLUP);

//  attachInterrupt(digitalPinToInterrupt(addSwitch), addValue, FALLING);// Falling เพื่อป้องกันการนับหลังจาก ปล่อยปุ่ม
//  attachInterrupt(digitalPinToInterrupt(downSwitch), ChooseValue, FALLING);
}






void loop()
{

  if (state_getmenu == false) {

    if ( millis() - last_getmenu > timePeriod_getmenu) {
      last_getmenu = millis();
      get_menu();
      state_getmenu = true;
    }
  }




  /*if (Serial.available() > 0)
  {
    String msg = Serial.readString();
    if (msg == "down\n" || msg == "down" )
    {
      if (orders < 9) {
        orders += 1 ;
      }
      else {
        orders = 0;
      }

      if (target == 0) {
        target = 16 ;
      }
      else {
        target = 0;
      }

      Serial.println(orders);
    }
    else if (msg == "add\n" || msg == "add")
    {
      order[orders] = order[orders] + 1;
      Serial.println(order[orders]);
    }

    else if (msg == "print\n" || msg == "print" )
    {
      if (printcheck == 0) {
        printcheck = 1 ;
        int costall = 1500;
        SendOder(costall);//ส่งราคาที่คำนวณได้เข้าไปให้ด้วย เก็บราคาไว้ให้ใน oder_cost เป็น vector เรียกใช้แบบ array ได้เลย
        
      }
      else {
        printcheck = 0;
      }

    }
    msg = "";
  }*/

// -------------------------- Switching System ------------------------
  if( digitalRead(addSwitch) == LOW)
  {
    order[orders] = order[orders] + 1;
      Serial.println(order[orders]);
      delay(1000);
  }

  if( digitalRead(downSwitch) == LOW)
  {
     if (orders < 9) {
        orders += 1 ;
      }
      else {
        orders = 0;
      }

      if (target == 0) {
        target = 16 ;
      }
      else {
        target = 0;
      }

      Serial.println(orders);
      delay(1000);
  }

  if( digitalRead(printSwitch) == LOW)
  {
   if (printcheck == 0) {
        printcheck = 1 ;
        int costall = 0;
        for(int each = 0 ; each < 10 ; each++)
        {
          costall = costall + (costeach[each] * order[each]);
          order[each] = 0;
        }
        SendOder(costall);//ส่งราคาที่คำนวณได้เข้าไปให้ด้วย เก็บราคาไว้ให้ใน oder_cost เป็น vector เรียกใช้แบบ array ได้เลย
        
      }
      else {
        printcheck = 0;
      }
      delay(5000);
  }
  
  if (state_getmenu == true)
  {
    if ( millis() - last_time > timePeriod) {
      last_time = millis();
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      if (orders == 0 || orders == 1) {
        display.setCursor(0, target);
        display.println(F("> "));
        display.setCursor(10, 0);
        display.println(menu[0]);
        display.setCursor(10, 16);
        display.println(menu[1]);
        display.setCursor(110, 0);
        display.println(order[0]);
        display.setCursor(110, 16);
        display.println(order[1]);

      }
      else if (orders == 2 || orders == 3) {
        display.setCursor(0, target);
        display.println(F("> "));
        display.setCursor(10, 0);
        display.println(menu[2]);
        display.setCursor(10, 16);
        display.println(menu[3]); //ค่าของmenu จะถูกเรียกมาจากเซิฟเวอร์ เท่ากับว่า ทุกครั้งที่เครื่องเปิดใช้งาน จะไปเรียกเมนูจะเซิฟเวอร์ ทำให้เราสามารถแก้เมนูได้ผ่านจากเว็บเลย
        display.setCursor(110, 0);
        display.println(order[2]);
        display.setCursor(110, 16);
        display.println(order[3]);
      }

       else if (orders == 4 || orders == 5) {
        display.setCursor(0, target);
        display.println(F("> "));
        display.setCursor(10, 0);
        display.println(menu[4]);
        display.setCursor(10, 16);
        display.println(menu[5]); //ค่าของmenu จะถูกเรียกมาจากเซิฟเวอร์ เท่ากับว่า ทุกครั้งที่เครื่องเปิดใช้งาน จะไปเรียกเมนูจะเซิฟเวอร์ ทำให้เราสามารถแก้เมนูได้ผ่านจากเว็บเลย
        display.setCursor(110, 0);
        display.println(order[4]);
        display.setCursor(110, 16);
        display.println(order[5]);
      }

       else if (orders == 6 || orders == 7) {
        display.setCursor(0, target);
        display.println(F("> "));
        display.setCursor(10, 0);
        display.println(menu[6]);
        display.setCursor(10, 16);
        display.println(menu[7]); //ค่าของmenu จะถูกเรียกมาจากเซิฟเวอร์ เท่ากับว่า ทุกครั้งที่เครื่องเปิดใช้งาน จะไปเรียกเมนูจะเซิฟเวอร์ ทำให้เราสามารถแก้เมนูได้ผ่านจากเว็บเลย
        display.setCursor(110, 0);
        display.println(order[6]);
        display.setCursor(110, 16);
        display.println(order[7]);
      }

       else if (orders == 8 || orders == 9) {
        display.setCursor(0, target);
        display.println(F("> "));
        display.setCursor(10, 0);
        display.println(menu[8]);
        display.setCursor(10, 16);
        display.println(menu[9]); //ค่าของmenu จะถูกเรียกมาจากเซิฟเวอร์ เท่ากับว่า ทุกครั้งที่เครื่องเปิดใช้งาน จะไปเรียกเมนูจะเซิฟเวอร์ ทำให้เราสามารถแก้เมนูได้ผ่านจากเว็บเลย
        display.setCursor(110, 0);
        display.println(order[8]);
        display.setCursor(110, 16);
        display.println(order[9]);
      }

      

      display.display();
      /*if (printcheck == 1) {
        Serial.println(totalOrder);
      }*/
    }
  }

  totalOrder = String(order[0]) + "+" + String(order[1]) + "+" + String(order[2]) + "+" + String(order[3]) + "+" + String(order[4]) + "+" + String(order[5]) + "+" + String(order[6]) + "+" + String(order[7]) + "+" + String(order[8]) + "+" + String(order[9]) ;

}
String httpGETRequest(String serverName) {

  HTTPClient http;
  http.begin(serverName);
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}

void SendOder(int costall)
{
  
    HTTPClient http;
    http.begin("http://warinnarat.com:8000/api/addoder/cart/");
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<500> doc;
    doc["order_food_items"] = totalOrder;
    doc["order_cost"] = costall;
    String requestBody;
    serializeJson(doc, requestBody);
    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {

      String response = http.getString();

      Serial.println(httpResponseCode);
      Serial.println(response);
  }
}


void get_menu() {
  if (WiFi.status() == WL_CONNECTED) {
    for (int i = 1 ; i < 11 ; i++)
    {

      DynamicJsonDocument doc(5000);
      String string_url = "http://warinnarat.com:8000/api/task/";
      if (mychar[i] == 'A')
      {
        string_url += "10";
      } else {
        string_url += String(mychar[i]);
      }
      sensorReadings = httpGETRequest(string_url);
      deserializeJson(doc, sensorReadings);
      JsonObject obj = doc.as<JsonObject>();
      menu.push_back(obj["menu_name"]);
      order_cost.push_back(obj["order_cost"]);
      delay(20);
    }
  }
}
