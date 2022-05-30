#include <SoftwareSerial.h>
#include <ArduinoHttpClient.h>
#include <avr/wdt.h>

//////gsm
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 650 // for response
#endif
#define TINY_GSM_DEBUG Serial
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 9600
#define pinReset 6
SoftwareSerial SerialAT(68, 69);  // RX, TX

// network  connection
const char apn[]      = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

//communication variables
// const char server[]   = "35.226.209.188";
const char server[]   = "137.63.184.136";
char resource[]="/api_v1/general/";//endpoint will be hard written here
String contentType ="application/json";
const int  port       = 8000;//port http

TinyGsm        modem(SerialAT);

TinyGsmClient client(modem);
HttpClient          http(client, server, port);

void setup() {
  Serial.begin(115200);
  delay(10);

  // need to put some gsm setup code here

}

void loop() {
  char payload[50] = {0};
  StaticJsonDocument<1024> doc;
  bool status = 0;
  doc["status"] = status;
  serializeJson(doc, payload);

  setupgsm();
  connectnet();
}

void setupgsm(){
  pinMode(pinReset ,OUTPUT);//reset pin
  delay(10);
  digitalWrite(pinReset, HIGH);
  delay(500);
  digitalWrite(pinReset, LOW);
  delay(500);
  digitalWrite(pinReset, HIGH);
  delay(3000);

  Serial.println("Wait...");
  //modem.setBaud(9600);
  TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
  // SerialAT.begin(9600);
  delay(6000);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
   //modem.init();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
}


void connectnet(){
  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    return;
  }
  Serial.println(" success");
  if (modem.isNetworkConnected()) { 
    Serial.println("Network connected");
  }
    Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println(" fail");
    return;
  }
  Serial.println(" success");

  if (modem.isGprsConnected()) {
    Serial.println("GPRS connected"); 
  }
  
  delay(5000);
}

int sendData(char* postData) {
  wdt_enable(WDTO_8S); // In case connection to server fails
  Serial.print(F("Performing HTTP POST request... "));
  //http.connectionKeepAlive();  // Currently, this is needed for HTTPS
  //int err = http.get(resource);
  int err = http.post(resource,contentType,postData);//even if they are char arrays
  if (err != 0) {
    Serial.println(F("failed to connect"));
    return -1;
  }
  wdt_disable();
  Serial.println("**** sending data ****");
  int status = http.responseStatusCode();
  Serial.print(F("Response status code: "));
  Serial.println(status);
  if ((status != 200) && (status != -3)) {
    return -1;
  }

  wdt_enable(WDTO_8S);
  if (status == -3) {
    markData(age, dataFile);
    digitalWrite(SD_CARD_LED,LOW);
    http.stop();
    Serial.println(F("Server disconnected"));
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
    while(1) {}
  }
  Serial.println(F("Response Headers:"));
  while (http.headerAvailable()) {
    String headerName  = http.readHeaderName();
    String headerValue = http.readHeaderValue();
    //Serial.println("    " + headerName + " : " + headerValue);
  }
  int length = http.contentLength();
  if (length >= 0) {
    Serial.print(F("Content length is: "));
    Serial.println(length);
  }
  if (http.isResponseChunked()) {
    Serial.println(F("The response is chunked"));
  }

  String body = http.responseBody();
  Serial.println(F("Response:"));
  Serial.println(body);

  //Serial.print(F("Body length is: "));
  //Serial.println(body.length());
  wdt_disable();
  return 1;
}

