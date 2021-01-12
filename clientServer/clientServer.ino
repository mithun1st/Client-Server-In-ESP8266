#include <ESP8266WiFi.h>

//Enter the Name & Password of your Wi-Fi
const char* ssid = "Mahadi Hassan";
const char* pass = "mithun1st";

WiFiServer server(80);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  wifiSetup(ssid, pass);

}


void loop() {

  /*    transmitData(p1, p2, p3);

    this function have three parameter,
    you can transmit three kind of data
    to users as a String Variable.
    if u have (int or double) type data
    like 'sensor3' just, simply convert
    it to String like in this example.

  */


  String sensor1 = "temperature";
  String sensor2 = "humidity";
  double  sensor3 = 3.1415;

  transmitData(sensor1, sensor2, (String)sensor3);




  /*      receivedData();

     this function return a String value
     as a received data from users. you
     can simply store the data in a String
     variable, ex: "String var=receivedData();"

  */


  String str = receivedData();

  if (str == "a_on") {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(">>\t" + str);
  }
  else if (str == "a_off") {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(">>\t" + str);
  }
  else if (str != "\0") {
    Serial.println(">>\t" + str);
  }

}










//----- Do Not Change Anything If Unnecessary !! -----

void wifiSetup(String s, String p) {
  Serial.println("\nConnecting to " + s);
  WiFi.begin(s, p);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}
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
