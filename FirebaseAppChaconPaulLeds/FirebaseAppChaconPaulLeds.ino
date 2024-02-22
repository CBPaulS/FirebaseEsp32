#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>
#define FIREBASE_USE_PSRAM

//Conectar a una red WiFi 
const char* WIFI_SSID = "TP-Link_9182";//el nombre de la red
const char* WIFI_PASSWORD = "58445990";//la contraseña de la red
//Recursos de Firebase
const char* API_KEY = "AIzaSyCx9k_b0x6zWWMmSIMkSyD4ilnwgo2KelU";
const char* FIREBASE_PROJECT_ID = "ngbytes-fireauth-a77ba";
//CONFIGURAR UN EMAIL Y CONTRASEÑA EN AUTHENTICATION de Firebase
const char* USER_EMAIL = "espcheicon@chacon.com";
const char* USER_PASSWORD = "espCheicon";

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int led1 = 21;
const int led2 = 22;
const int led3 = 23;


//Funcion para configuara WiFi
void setupWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;  // Ver addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void setup() {
  Serial.begin(115200);
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);//Verificamos la version
  setupWiFi();//Llamamos la funcion de configuración wifi
  setupFirebase();//Llamamos la funcion de configuración Firebase
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  String path = "controlLED";//RUTA DE COLECCION
  FirebaseJson json;

  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), "")) {

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, fbdo.payload().c_str());

    if (!error) {
      for (JsonObject document : doc["documents"].as<JsonArray>()) {
        const char* document_name = document["name"];//PARAMETRO NOMBRE == ID Documento
        //Serial.print(document);
        const bool state = document["fields"]["encender"]["booleanValue"];//PARAMETRO DE CAMPOS A REVISAR
        const bool state2 = document["fields"]["encender2"]["booleanValue"];//PARAMETRO DE CAMPOS A REVISAR
        const bool state3 = document["fields"]["encender3"]["booleanValue"];//PARAMETRO DE CAMPOS A REVISAR
        Serial.print(" : ");
        if (strstr(document_name, "LED1") != nullptr) {//COMPARAR NOMBRE CON  RESULTADO ESPERADO LED1
          state ? Serial.print("LED1 ON") : Serial.print("LED1 OFF");
          if (state == true){
            digitalWrite(led1, 1);
          }else{
            digitalWrite(led1, 0);
          }
        }
		
    if (strstr(document_name, "LED2") != nullptr) {//COMPARAR NOMBRE CON  RESULTADO ESPERADO LED2
          state2 ? Serial.print("LED2 ON") : Serial.print("LED2 OFF");
          if (state2 == true){
            digitalWrite(led2, 1);
          }else{
            digitalWrite(led2, 0);
          }
        }
		
    if (strstr(document_name, "LED3") != nullptr) {//COMPARAR NOMBRE CON  RESULTADO ESPERADO LED3
          state3 ? Serial.print("LED3 ON") : Serial.print("LED3 OFF");
          if (state3 == true){
            digitalWrite(led3, 1);
          }else{
            digitalWrite(led3, 0);
          }
        }  
		  }
    delay(100);
    }
  } 
}
