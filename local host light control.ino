#include <ESP8266WiFi.h>

const char *ssid = "Area 51 - America";
const char *password = "Darshana1997";

WiFiServer server(80);

String header;

String LED1_State = "off";
String LED2_State = "off";

const int LED_1 = D0;
const int LED_2 = D2;


unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
 
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client");         
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("LED 1 on");
              LED1_State = "on";
              digitalWrite(LED_1, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("LED 1 off");
              LED1_State = "off";
              digitalWrite(LED_1, LOW);
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("LED 2 on");
              LED2_State = "on";
              digitalWrite(LED_2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("LED 2 off");
              LED2_State = "off";
              digitalWrite(LED_2, LOW);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".head { position: fixed; left: 0; top: 0; width: 100%; background-color: #057DF6; font-size:35px; font-weight: bold; color: white; text-align: center; }");
            client.println(".footer { position: fixed; left: 0; bottom: 0; background-color: #057DF6; width: 100%; color: white; text-align: center;}");
            client.println(".button { background-color: #c0392b; border: none; color: white; padding: 16px 40px; width: 150px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A; width: 150px;}</style></head>");
            client.println("<div class=\"head\"> <p>Home Automation system</p> </div></br></br></br></br></br></br></br>");
          
            client.println("<h3>Bulb 1</h3>");    
            if (LED1_State=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button\">ON </button></a></p>");
            } 
               
            client.println("<h3>Bulb 2</h3>");   
            if (LED2_State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button \">ON </button></a></p>");
            }
            client.println("<div class=\"footer\"> <p>DS Perera</p> </div>");
            client.println("</body></html>");
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;   
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
