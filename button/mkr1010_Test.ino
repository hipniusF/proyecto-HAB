/*

 This example connects to an unencrypted Wifi network.
 Then it prints the  MAC address of the Wifi module,
 the IP address obtained, and other network details.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;  // the Wifi radio's status
int readPin = 6;

char serverAddress[] = IP_WEBSOCKET;  // server address
int port = PORT_WEBSOCKET;

WiFiClient wifi;


WebSocketClient client = WebSocketClient(wifi, serverAddress, port);

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);   
  //Initialize serial and wait for port to open:
//  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    // // Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    // Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    // Serial.print("Attempting to connect to WPA SSID: ");
    // Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  // Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  client.begin();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // check the network connection once every 10 seconds:

  if (digitalRead(readPin)) {
    
    if(client.connected()) {
      sendMessageToWebSocket(CALL_MSG);
      delay(1000);
    } else {
      client.begin();
    }
  }

}

void sendMessageToWebSocket(String msg) {
  client.beginMessage(TYPE_TEXT);
  client.print(msg);
  client.endMessage();
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  // Serial.print("IP Address: ");
  // Serial.println(ip);
  // Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  // Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  // Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  // Serial.print("signal strength (RSSI):");
  // Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  // Serial.print("Encryption Type:");
  // Serial.println(encryption, HEX);
  // Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      // Serial.print("0");
    }
    // Serial.print(mac[i], HEX);
    if (i > 0) {
      // Serial.print(":");
    }
  }
  // Serial.println();
}
