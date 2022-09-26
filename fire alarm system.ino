#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library
#include <MQ2.h>


#define FIREBASE_HOST "fire-system-home-default-rtdb.asia-southeast1.firebasedatabase.app/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "y7oXMeJRZYNSGpf99rYJZkhD9OlnWfiKRuisi1VS"
#define WIFI_SSID "family_basri"
#define WIFI_PASSWORD "CHRISNA007"

#define DHTPIN 13    // Connect Data pin of DHT to D2
int led = 16;     // Connect LED to D5
int flame_sensor = 12; //Connected to D6 pin of NodeMCU
int buzzer = 4; //Connected to D2 pin of NodeMCU
int gas_sensor = A0; //Connected to A0 pin of NodeMCU
int temp_gas_sensor;
int flame_sensor_read;

#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;


void setup()
{

  Serial.begin(9600);
  pinMode(flame_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(gas_sensor, INPUT);
  
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);


  dht.begin();
  pinMode(led,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate1(){
  flame_sensor_read = digitalRead(flame_sensor); // Instruksi untuk membaca nilai digital sensor
  if (flame_sensor_read == LOW ) {             
   // Serial.println(flame_sensor_read);
   // Serial.println("Flame detected");  // Menampilkan karakter pada serial monitor
    Firebase.setString(firebaseData, "/FirebaseIOT/flame_sensor", "Flame detected");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
     
  } else if (flame_sensor_read == HIGH ){
    //Serial.println(flame_sensor_read);
    //Serial.println("No flame detected"); // Menampilkan karakter pada serial monitor
    Firebase.setString(firebaseData, "/FirebaseIOT/flame_sensor", "No Flame detected");
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
}else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
void sensorUpdate2(){
  temp_gas_sensor = analogRead(gas_sensor); // Instruksi untuk membaca nilai digital sensor
  if (Firebase.setString(firebaseData, "/FirebaseIOT/gas_sensor", temp_gas_sensor)){             
   // Serial.println(temp_smoke_sensor);
   // Serial.println(temp_gas_sensor);  
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
     
  
}else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  
}
void sensorUpdate(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C  ,"));
  Serial.print(f);
  Serial.println(F("F  "));

  

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
void loop() {
  sensorUpdate();
  sensorUpdate2();
  sensorUpdate1();
  if (flame_sensor_read==0 ) {
    digitalWrite(led, HIGH); 
    digitalWrite(buzzer, HIGH);
}

  else { 
    digitalWrite(led, LOW); 
    digitalWrite(buzzer, LOW);
}
}