#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#define SENSOR D2
#define led1 D5
#define led2 D6
WiFiClient client;
WiFiServer server(80);
long currentMillis = 0;
long previousMillis = 0;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
int interval = 1000;
float flowRate;
float  totalLiters;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
  
}

void setup()
{
  WiFi.begin("9", "123456789");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  server.begin();
  
  Serial.begin(115200);
  pinMode(SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  totalLiters = 0.0;
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}
void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) /calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    totalLiters=(totalMilliLitres/1.45);

    Serial.print("Total miliLiters: ");
    Serial.println(totalLiters);
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  // put your main code here, to run repeatedly:
  //Gets a client that is connected to the server and has data available for reading.    
 client = server.available();
//Gets a client that is connected to the server and has data available for reading.    
  if (client == 1)
  {  
    String request =  client.readStringUntil('\n');
    Serial.println(request);
    request.trim();
    if(request == "GET /led1on HTTP/1.1")
    {
      digitalWrite(led1, HIGH);
      client.println("<h3>led is on<h3>");
    }
    if(request == "GET /led1off HTTP/1.1")
    {
      digitalWrite(led1, LOW);
      client.println("<h3>LED is off<h3>");
    }
    if(request == "GET /led2on HTTP/1.1")
    {
      digitalWrite(led2, HIGH);
    }
    if(request == "GET /led2off HTTP/1.1")
    {
      digitalWrite(led2, LOW);
    }
  }
  
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
//  client.println("<h1>Welcome to the Webpage!</h1>");
  client.println("<h3>LED Controls<h3>");
  client.println("<br>");
  client.println("<a href=\'/led1on\'\"><button>Relay 1ON</button></a>");
  client.println("<a href=\'/led1off\'\"><button>Relay 1 OFF</button></a><br/>");
  client.println("<a href=\'/led2on\'\"><button>Relay 2 ON</button></a>");
  client.println("<a href=\'/led2off\'\"><button>Relay 2 OFF</button></a><br/>");
  client.println("</html>");
  }
