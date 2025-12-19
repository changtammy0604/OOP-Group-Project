#include <WiFi.h>
#include <HTTPClient.h>
#include <Discord_WebHook.h>

// 定義溫度、亮度、濕度範圍
float tempLow = 18.0;
float tempHigh = 25.0;
float dirtHumLow = 40.0;
float dirtHumHigh = 60.0;
int lightLow = 500;
int lightHigh = 900;

float temp_val;
float hum_val;
float light_val;

// 網路連線變數
WiFiClient client;
HTTPClient http;
const char* ssid = "mingming";
const char* password = "cheese0609";
Discord_Webhook discord;
String channel_id = "1436165223194038484";
String token = "k9GqhPuA9JqYd6MMWh2dGo0elifmR93GmYNlndESow1ZZapi2YnSaj7vMdhRRxPISC3-";
// https://discord.com/api/webhooks/1436165223194038484/k9GqhPuA9JqYd6MMWh2dGo0elifmR93GmYNlndESow1ZZapi2YnSaj7vMdhRRxPISC3-

// 初始設定
void setup() {
  //pinNode
  Serial.begin(115200);
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
}

// discord 亮度警訊
void lightAlert(int light){
  String str = "Light Value = " + String(light);
  if(light < lightLow){
    discord.sendEmbed("[警告：亮度過低]",str,"#65280"); 
  }else if(light > lightHigh){
    discord.sendEmbed("[警告：亮度過高]",str,"#65280"); 
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

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    lightAlert(light_val);
    humidAlert(hum_val);
    tempAlert(temp_val);

    delay(5000);
  }
}

