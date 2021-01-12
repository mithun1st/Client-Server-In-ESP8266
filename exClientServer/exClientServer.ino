#include <ESP8266WiFi.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Enter the Name & Password of your Wi-Fi
const char* ssid = "Mahadi Hassan";
const char* pass = "mithun1st";

WiFiServer server(80);

#define ana A0
#define sw1 D5
#define buz D7
#define sw2 D6
#define red D4
#define blu D0
#define gre D3
#define rel D8

String ts1 = "", ts2 = "", ts3 = "";

void setup() {

  pinMode (ana, OUTPUT);
  pinMode (sw1, INPUT);
  pinMode (buz, OUTPUT);
  pinMode (sw2, INPUT);
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (gre, OUTPUT);
  pinMode (rel, OUTPUT);
  lcd.begin();

  lcd.clear();


  Serial.begin(115200);
  wifiSetup(ssid, pass);


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tra>> ");
  lcd.setCursor(0, 1);
  lcd.print("Rec<<  x_xx");
  digitalWrite(buz, 0);
}


void loop() {

  String sensor1 = (String)map(analogRead(ana), 0, 1024, 10, 999);
  String sensor2 = (String) digitalRead(sw2);
  String sensor3 = (String) digitalRead(sw1);

  transmitData(sensor1, sensor2, sensor3);
  String str = receivedData();



  if (str == "a_on") {
  digitalWrite(red, 1);
  }
  else if (str == "a_off") {
  digitalWrite(red, 0);
  }
  if (str == "b_on") {
  digitalWrite(gre, 1);
  }
  else if (str == "b_off") {
  digitalWrite(gre, 0);
  }
  if (str == "c_on") {
  digitalWrite(blu, 1);
  }
  else if (str == "c_off") {
  digitalWrite(blu, 0);
  }

  else if (str == "led on"  || str == "d_on") {
  digitalWrite(red, 1);
    digitalWrite(gre, 1);
    digitalWrite(blu, 1);
  }
  else if (str == "led off" || str == "d_off") {
  digitalWrite(red, 0);
    digitalWrite(gre, 0);
    digitalWrite(blu, 0);
  }



  //--------------------- lcd 1602

  if (str != "\0") {
  lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Rec<<  " + str);
    digitalWrite(buz, 1);
    delay(50);
    digitalWrite(buz, 0);
    Serial.println(str);
  }
  lcd.setCursor(0, 0);
  lcd.print("Tra>>  " + sensor1 + " " + sensor2 + " " + sensor3);
}










//----- Do Not Change Anything If Unnecessary !! -----

void wifiSetup(String s, String p) {
  digitalWrite(buz, 1);
  lcd.setCursor(1, 0);
  lcd.print("Mahadi Hassan");
  lcd.setCursor(4, 1);
  lcd.print("MITHUN");
  delay(700);

  Serial.println("\nConnecting to " + s);
  WiFi.begin(s, p);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wi-Fi Connecting");
  lcd.setCursor(0, 1);
  delay(500);
  int i = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    if (i == 8) {
      i = 0;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
    }
    lcd.print(". ");
    buzz(200);
    delay(300);
    i++;
  }

  Serial.println(WiFi.localIP());

  budi(700);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("IP Address:");
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP());


  while (!digitalRead(sw1)) {
    delay(100);
  }


  Serial.println("\nWiFi connected");
  server.begin();
  Serial.println("Server started");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Web Server Being");
  lcd.setCursor(0, 1);
  for (int i = 0; i <= 8; i++) {
    lcd.print(". ");
    buzz(30);
    delay(30);
  }
  budi(700);
  lcd.clear();

}
void buzz(int d) {
  digitalWrite(buz, HIGH);
  delay(d);
  digitalWrite(buz, LOW);
}
void budi(int d) {
  digitalWrite(buz, 1);
  lcd.noDisplay();
  delay(d);
  lcd.display();
  digitalWrite(buz, 0);
}
//---------------------------------api
String gs1 = "", gs2 = "", gs3 = "";
String receivedData() {
  WiFiClient client = server.available();
  if (client) {
    String req = client.readStringUntil('\r');
    req = req.substring(5, req.length() - 9);
    client.flush();
    String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    web += gs1 + "#" + gs2 + "#" + gs3 + "#\r\n";
    client.print(web);
    return req;
  }
}
void transmitData(String s1, String s2, String s3) {
  gs1 = s1;
  gs2 = s2;
  gs3 = s3;
}
