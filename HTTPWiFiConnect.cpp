/*
  HTTPWiFiConnect.h - Library to connect ESP8266 boards to WiFi using simple HTTP GET request.
  Created by Simerneet Singh(A.K.A Saber), August 13, 2018.
  Relased into the Public Domain
*/

HTTPWiFiConnect :: HTTPWiFiConnect(String WiFiName){
  Serial.begin(115200);
  delay(1000);
  wifiName = WiFiName;
  bool SPIFFSresult = SPIFFS.begin();
  Serial.println("File system Started: " + SPIFFSresult);

  if(!SPIFFS.exists("/WIFI.txt")){
    Serial.println("WIFI has not been configured");
    launchAP();
  }
  else{
    WIFI = SPIFFS.open("/WIFI.txt", "r");
    Serial.println("Connecting to WiFi");
    String WIFIssid = WIFI.readStringUntil('\n');
    String WIFIpass = WIFI.readStringUntil('\n');

    WiFi.begin(WIFIssid.c_str(), WIFIpass.c_str());
    if(!checkWiFi()){
      Serial.println("!WiFi credentials not correct!");
      launchAP();
      Serial.println("WiFi AP started");
    }
    else
      startServer();
  }
  WIFI.close();
}

void HTTPWiFiConnect :: checkWiFi(){
  int check = 0;

  while(check<20){
    if(WiFi.status() == WL_CONNECTED)
      return true;
    delay(500);
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());
    check++;
  }

  Serial.println("\n!Connection timed out!");
  return false;
}

void HTTPWiFiConnect :: startServer(){
    server.on("/ConfigWlan", configWiFi);
    server.begin();
}

void HTTPWiFiConnect :: launchAP(){
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  WiFi.softAP(wifiName);
  Serial.println("AP started! connect to \" "+ wifiName + " \" to configure WiFi.");
  Serial.print("Reachable IP : ");
  Serial.println(WiFi.softAPIP());
  startServer();
}

void HTTPWiFiConnect :: configWiFi(){
  String ssid = server.arg("ssid");
  String pass = server.arg("password");
  WiFi.begin(ssid.c_str(), pass.c_str());
    if(checkWiFi()){

      WIFI = SPIFFS.open("/WIFI.txt","w");

      WIFI.println(ssid);
      WIFI.println(pass);
      startServer();

      Serial.print("Local IP : ");
      Serial.println(WiFi.localIP());
    }
    else{
      Serial.println("!WiFi credentials not correct!");
      launchAP();
      Serial.println("WiFi AP started");
    }

    server.send(200,"text/html", indexPage.c_str());
}

void HTTPWiFiConnect :: handleServer(){
    server.handleClient();
}