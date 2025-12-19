#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Discord_WebHook.h>

#define water_pin 35  //水位感測器
#define light_pin 39  //光感測器
#define dirt_pin 36   //土壤溼度感測器
#define dht_pin 26   //溫度感測器
#define dht_type DHT11

#define mortar_pin 32 //抽水馬達(接在繼電器上)

// 定義溫度、亮度、濕度範圍
float tempLow = 18.0;
float tempHigh = 30.0;
float dirtHumLow = 40.0;
float dirtHumHigh = 70.0;
int lightLow = 1000;
int lightHigh = 3000;

int water_gate = 2000;      //停止澆水的域值
int dirt_gate = 3000;      //啟動澆水的溼度域值
bool mortar_state = false;

// 網路連線變數
WiFiClient client;
HTTPClient http;
const char* ssid = "Tammy";
const char* password = "123123123";
Discord_Webhook discord;
String channel_id = "1436165223194038484";
String token = "k9GqhPuA9JqYd6MMWh2dGo0elifmR93GmYNlndESow1ZZapi2YnSaj7vMdhRRxPISC3-";

//initialize DHT
DHT dht(dht_pin, dht_type); 

void setup() {
  Serial.begin(115200);
  pinMode(water_pin, INPUT);
  pinMode(light_pin, INPUT);
  pinMode(dirt_pin, INPUT);
  pinMode(mortar_pin, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  discord.begin(channel_id, token);
  discord.addWiFi(ssid, password);
  discord.connectWiFi();

  
  dht.begin();
  delay(2000);
}

void loop() {
  //讀取土壤溼度
  int dirt_val = analogRead(dirt_pin);
  Serial.print("dirt level: ");
  Serial.println(dirt_val);

  int water_val = analogRead(water_pin);
    Serial.print("water level: ");
    Serial.println(water_val);

  int light_val = analogRead(light_pin);
    Serial.print("light level: ");
    Serial.println(light_val);

  float temp_val = dht.readTemperature(); 
  float hum_val = dht.readHumidity(); 

  Serial.println(temp_val);
  Serial.println(hum_val);

  lightAlert(light_val);
  humidAlert(hum_val);
  tempAlert(temp_val);

  //啟動馬達澆水
  if(dirt_val >= dirt_gate && water_val < water_gate){
    digitalWrite(mortar_pin, HIGH);
    delay(5000);
    digitalWrite(mortar_pin, LOW);
    mortar_state = true;
  }
  else{
    if(mortar_state){
      mortar_state = false;
      discord.sendEmbed("[澆水已完成]","","#65280"); 
    }
    digitalWrite(mortar_pin, LOW);
  }
  delay(1000);
}


// discord 亮度警訊
void lightAlert(int light){
  String str = "Light Value = " + String(light);
  if(light < lightLow){
    discord.sendEmbed("[警告：亮度過高]",str,"#65280"); 
  }else if(light > lightHigh){
    discord.sendEmbed("[警告：亮度過低]",str,"#65280"); 
  }
}
// discord 濕度警訊
void humidAlert(float dirtHum){
  String str = "Humidity Value = " + String(dirtHum);
  if(dirtHum < dirtHumLow){
    discord.sendEmbed("[警告：濕度過低]",str,"#65280"); 
  }else if(dirtHum > dirtHumHigh){
    discord.sendEmbed("[警告：濕度過高]",str,"#65280"); 
  }
}
// discord 溫度警訊
void tempAlert(float temp){
  String str = "Temperature Value = " + String(temp);
  if(temp < tempLow){
    discord.sendEmbed("[警告：溫度過低]",str,"#65280"); 
  }else if(temp > tempHigh){
    discord.sendEmbed("[警告：溫度過高]",str,"#65280"); 
  }
}
