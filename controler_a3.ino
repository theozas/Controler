
//point to point wifi coms @ 2.4GHz
//sketch is mod from https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
//mod by paulius to learn wifi coms
//prototype code to se latancy of coms
#include <esp_now.h>
#include <WiFi.h>

//addres of slave esp for main esp
//uint8_t broadcastAddress[] = {0x84, 0x0D, 0x8E, 0xE6, 0x78, 0x74};
//address of main esp for slave esp
uint8_t broadcastAddress[] = {0xBC, 0xDD, 0xC2, 0xD0, 0xE5, 0xD4};
int incomingReadings, outgoingData ,timeNow, timeOld, timeDif;
String success;

//cal back funtion is called when data is sent
void OnDataSent(const uint8_t * mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packed Send Status:\t");
  //prints status of tranmition
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Desivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

//callback function when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData , sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);

timeOld = incomingReadings;
  //any data received shuld be saved here
}


void setup() {
  Serial.begin(115200);
  //set device as wifi station
  WiFi.mode(WIFI_STA);
  //init esp_now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;//?
  }

  //register call back funtion
  esp_now_register_send_cb(OnDataSent);

  //register peer its like pairing devices in bluetooth coms
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;//?
  }
  //register callback funtion
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  //read data here
  timeNow = millis();
  outgoingData = timeNow;
  timeDif = timeOld - timeNow;
  //send data via ESP NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingData, sizeof(outgoingData));

  if (result == ESP_OK) {
    Serial.print("Send with success, pig: ");
    Serial.print(timeDif);
  } else {
    Serial.println("Error sending data");
  }
  
}
