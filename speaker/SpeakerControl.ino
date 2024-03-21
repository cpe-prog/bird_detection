#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define Speaker D3

#define WIFI_SSID "I'm in!"
#define WIFI_PASSWORD "connected"
#define API_KEY "AIzaSyDrywOcBL1FyFKqzsA0g6wPuHWaYzXN8uU"
#define DATABASE_URL "https://bird-77fb0-default-rtdb.firebaseio.com/" 


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
bool speaker = false;


void setup() {
  pinMode(Speaker, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
   while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
   Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}



void loop() {
  if (Firebase.ready() && signupOK && (millis() -  sendDataPrevMillis > 1000 || sendDataPrevMillis == 0 )) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getBool(&fbdo, "bird/1/maya")){
      if (fbdo.dataType() == "boolean"){
      speaker = fbdo.boolData();
      Serial.println("Seccess: " + fbdo.dataPath() + ": " + speaker + "(" + fbdo.dataType() + ")");
      digitalWrite(Speaker, speaker);
      }
    }
    else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }

}
