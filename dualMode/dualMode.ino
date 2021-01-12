#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <ESP8266WiFi.h>


WiFiServer server(80);


//--Enter Name & Password of your WiFi
const String ssid = "Mahadi Hassan";
const String pass = "mithun1st";


#define ana A0
#define sw1 D5
#define buz D7
#define sw2 D6
#define red D4
#define blu D0
#define gre D3
#define rel D8

bool ac = true;

void setup() {
  pinMode (ana, OUTPUT);
  pinMode (sw1, INPUT);
  pinMode (buz, OUTPUT);
  pinMode (sw2, INPUT);
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (gre, OUTPUT);
  pinMode (rel, OUTPUT);

  Serial.begin(115200);

  lcd.begin();
  lcd.clear();

  digitalWrite(buz, 1);
  lcd.setCursor(1, 0);
  lcd.print("Mahadi Hassan");
  lcd.setCursor(4, 1);
  lcd.print("MITHUN");
  delay(700);
  lcd.clear();
  digitalWrite(buz, 0);


  disp(500);
  buzz(100);

  lcd.setCursor(6, 0);
  lcd.print("I.O.T.");
  lcd.setCursor(3, 1);
  lcd.print("Client Server");

  while (!digitalRead(sw1)) {
    if (digitalRead(sw2)) {
      buzz(100);
      ac = !ac;
    }
    if (ac) {
      lcd.setCursor(0, 0);
      lcd.print("#");
      lcd.setCursor(0, 1);
      lcd.print(" ");

    }
    else {
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("#");
    }
    delay(50);
  }
  budi(300);


  //--------------------------ap(setUp)---------
  if (ac) {

    WiFi.begin(ssid, pass);
    Serial.println();

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

    budi(700);

    Serial.println();
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("IP Address:");
    lcd.setCursor(1, 1);
    lcd.print(WiFi.localIP());


    while (!digitalRead(sw1)) {
      delay(100);
    }

    server.begin();

  }

  //  ------------------------cs(setUp)-------------
  else {
    WiFi.begin(ssid, pass);
    Serial.println();

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

    budi(700);

    Serial.println();
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("IP Address:");
    lcd.setCursor(1, 1);
    lcd.print(WiFi.localIP());


    while (!digitalRead(sw1)) {
      delay(100);
    }

    server.begin();


  }


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

  lcd.setCursor(0, 1);
  lcd.print("Transmit: ");
  lcd.setCursor(0, 0);
  lcd.print("Received: x_xx");

}

void loop() {
  String str = "";

  while (ac) { //------------------ap----------------------
    iot();
  }
  while (!ac) { //-------------client server---------------
    clientServer();
  }

}


//----------------------------function--------------------------------

String s1 ;
String s2 ;
String s3 ;
String s;

String ts1 ;
String ts2 ;
String ts3 ;

String ts;

void clientServer() {

  s1 = (String) map(analogRead(ana), 0, 1024, 10, 99);
  s2 = (String) digitalRead(sw1);
  s3 = (String) digitalRead(sw2);

  s = receivedData();


  if (s == "a_on") {
    digitalWrite(red, 1);
  }
  else if (s == "a_off") {
    digitalWrite(red, 0);
  }
  if (s == "b_on") {
    digitalWrite(gre, 1);
  }
  else if (s == "b_off") {
    digitalWrite(gre, 0);
  }
  if (s == "c_on") {
    digitalWrite(blu, 1);
  }
  else if (s == "c_off") {
    digitalWrite(blu, 0);
  }

  else if (s == "led on") {
    digitalWrite(red, 1);
    digitalWrite(gre, 1);
    digitalWrite(blu, 1);
  }
  else if (s == "led off") {
    digitalWrite(red, 0);
    digitalWrite(gre, 0);
    digitalWrite(blu, 0);
  }



  if (s != ts || s1 != ts1 || s2 != ts2 || s3 != ts3) {

    transmitData(s1, s3, s2);

    lcd.clear();
    if (s != '\0') {
      lcd.setCursor(0, 0);
      lcd.print("Received: " + s);
    }

    lcd.setCursor(0, 1);
    lcd.print("\t\t\t\t");
    lcd.setCursor(0, 1);
    lcd.print("Transmit: " + s1 + " " + s3 + " " + s2);
  }

  ts = s;
  ts1 = s1;
  ts2 = s2;
  ts3 = s3;
}


void buzz(int d) {
  digitalWrite(buz, HIGH);
  delay(d);
  digitalWrite(buz, LOW);
}
void disp(int d) {
  lcd.noDisplay();
  delay(d);
  lcd.display();
}
void budi(int d) {
  digitalWrite(buz, 1);
  lcd.noDisplay();
  delay(d);
  lcd.display();
  digitalWrite(buz, 0);
}



void iot() {

}

//---------------------------
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
