#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
 
const char* ssid = "Kowalnet";
const char* password = "";
const int status = WL_IDLE_STATUS;
const String nothingToDo = "\"no\"";
WiFiClient client;
HTTPClient http;
const int PORT = 5000;
const char SERVER[] = "192.168.43.66";
const int WIRE_SLAVE_ID = 9;

void setup () {
 
  Serial.begin(115200);
  while(!Serial){} 
  Wire.begin();
  
  connectToWifi();
}
 
void loop() {
    
    if (WiFi.status() == WL_CONNECTED) {
        getBatteryDataFromServer(0);
		getBatteryDataFromServer(1);
        sendBatteryDataToServer();
    }
    else{
        connectToWifi();
    }
    
    delay(4000);
}

void connectToWifi(){
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting..");
        WiFi.begin(ssid, password);
        delay(10000);
    }
    Serial.println("connected");
}

void getBatteryDataFromServer(int battery){
    const String url = "http://192.168.43.66:5000/Microcontroller/getBatteryData/9/0/" + String(battery);
    http.begin(client, url);
    delay(1000);
    int httpCode = http.GET();  
    
    if (httpCode > 0) { 
        String response = http.getString();
        http.end();
        
        if(response == nothingToDo)
        {
            return;
        }
        Serial.println(response);
        sendProfilesDataToMicro(response);
        const String CANAL_FLAG = "" + String(battery);
        sendStringToMicro(CANAL_FLAG);
        sendBatteryDataToMicro(response);
    }
}

void sendProfilesDataToMicro(const String& response)
{
    int indexTemp = response.indexOf("chargingProfiles");
    int indexStart = 0;
    indexStart = response.indexOf('{', indexTemp);
    while(indexStart != -1)
    {
        int indexEnd = response.indexOf('}', indexStart);
        String profileJSON = response.substring(indexStart, indexEnd+1);
        sendStringToMicro(profileJSON);
        delay(50);
        indexTemp = indexEnd;
        indexStart = response.indexOf('{', indexTemp);
    }
}

void sendBatteryDataToMicro(const String& response)
{
    int indexStart = response.indexOf("{\"BatteryMode");
    int indexEnd = response.indexOf('}', indexStart);
    String batteryJSON = response.substring(indexStart, indexEnd+1);
    sendStringToMicro(batteryJSON);
    delay(50);
}

void sendStringToMicro(const String& data){
    const int MAX_BYTES = 32;
    String part;
    int i = 0;
    int diff = data.length()+1 - MAX_BYTES; //+1 because of additional '\0'
    while( i < diff)
    {
        part = data.substring(i, i+MAX_BYTES);
        Wire.beginTransmission(WIRE_SLAVE_ID);
        Wire.write(part.c_str());
        Serial.println(part);
        Wire.endTransmission();
        delay(1);
        i += MAX_BYTES;
    }
    part = data.substring(i);
    Wire.beginTransmission(WIRE_SLAVE_ID);
    Wire.write(part.c_str());
    Serial.println(part);
    Wire.write('\0');
    Wire.endTransmission();
}

void sendBatteryDataToServer(){
    const int MAX_DATA_SIZE = 32;
    Wire.requestFrom(WIRE_SLAVE_ID, MAX_DATA_SIZE);
    String tempBuffer;
    tempBuffer.reserve(MAX_DATA_SIZE);
    while( Wire.available() )
    {
        char c = Wire.read();
        tempBuffer += c;
    }
    
    int indexStart = 0;
    int indexEnd = tempBuffer.indexOf('\n', indexStart);
    String currentCanal = tempBuffer.substring(indexStart,indexEnd);
	if(currentCanal == "-1")
	{
		return;
	}
	
    indexStart = indexEnd + 1;
    indexEnd = tempBuffer.indexOf('\n', indexStart);
    String percentage = tempBuffer.substring(indexStart,indexEnd);
    indexStart = indexEnd + 1;
    indexEnd = tempBuffer.indexOf('\n', indexStart);
    String batteryVoltage = tempBuffer.substring(indexStart,indexEnd);
    indexStart = indexEnd + 1;
    indexEnd = tempBuffer.indexOf('\n', indexStart);
    String current = tempBuffer.substring(indexStart,indexEnd);
    
    String jsonBody = "{\"Completion\":\"" + percentage + "\", \"CellVoltage\":\"" + batteryVoltage + "\", \"CurrentFlowing\":\"" + current + "\"}";
    String serverName = "http://192.168.43.66:5000/Microcontroller/sendChargingData/9/0/" + currentCanal;
    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");
    http.POST(jsonBody);
    http.end();
}
