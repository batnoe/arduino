
/*#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); */ // décommentez pour un DHT22

#include <DallasTemperature.h>
#include <OneWire.h>
OneWire oneWire(5);                             //Bus One Wire sur la pin 2 de l'arduino
DallasTemperature sensors(&oneWire);            //Utilistion du bus Onewire pour les capteurs
DeviceAddress sensorDeviceAddress;              //Vérifie la compatibilité des capteurs avec la librairie


#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress1[] = {0x9C, 0x9c, 0x1F, 0xC2, 0x8A, 0x78};   // <-Upesy _{0x24, 0x6F, 0x28, 0xB1, 0x45, 0x48}; <-grand écran salle
uint8_t broadcastAddress2[] = {0x24, 0x6F, 0x28, 0xB1, 0x45, 0x48};  // TFT2.8 DEVKIT

typedef struct struct_message {
  float c;
  //float d;
} struct_message;

struct_message myData;

/*// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
} */

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[12];
  Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  sensors.begin();  //dht.begin();
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  /*esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
         
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } */

   esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // register second peer
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  /*float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  
  myData.c = t;
  myData.d = h; */
  sensors.requestTemperatures();
  myData.c = sensors.getTempCByIndex(0);
  
  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   esp_err_t result = esp_now_send(0
   , (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000*2);
}
