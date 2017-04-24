#include <ESP8266WebServer.h>
#include <DHT.h>
#include "RestClient.h"
#include "credentials.h"
    
#define DHTTYPE DHT21
#define DHTPIN  4
#define DHTPOWERPIN 5

const char* ssid = SSID;
const char* password = PASSWORD;
const char* host = HOST;

String body = "{\"context\": \"vessels.self\",\"updates\": [{\"values\": [{\"path\": \"environment.inside.temperature\",\"value\": TEMP1 }, {\"path\": \"environment.inside.humidity\",\"value\": HUMID1 } ], \"source\": { \"label\": \"esp8266fakesensor\" } } ] }";

RestClient rclient = RestClient(host, 3000);

ADC_MODE(ADC_VCC);
int vcc = 0;


String response;

ESP8266WebServer server(80);

// Start NTP only after IP network is connected
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
}

// Manage network disconnection
void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
}

// Initialize DHT sensor
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01
DHT dht(DHTPIN, DHTTYPE, 21); // 11 works fine for ESP8266

float humidity, temp, dewpoint;  // Values read from sensor

// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 1000;              // interval at which to read sensor

String webString;

void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}

void setup(void)
{
  static WiFiEventHandler e1, e2;
  pinMode(DHTPOWERPIN, OUTPUT);
  digitalWrite(DHTPOWERPIN, HIGH);

  e1 = WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client
  e2 = WiFi.onStationModeDisconnected(onSTADisconnected);
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  dht.begin();           // initialize temperature sensor

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);

  server.on("/temp", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();       // read sensor
    webString="Temperature: "+String(temp,1)+" C";   // Arduino has a hard time with float to string
    server.send(200, "text/plain", webString);            // send to someones browser when asked
  });

  server.on("/humidity", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();           // read sensor
    webString="Humidity: "+String((int)humidity)+"%";
    server.send(200, "text/plain", webString);               // send to someones browser when asked
  });

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void)
{
  gettemperature();
  server.handleClient();
}


void gettemperature() {

  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp = dht.readTemperature();     // Read temperature as Fahrenheit
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

     float gamma = log(humidity / 100) + ((17.62 * temp) / (243.5 + temp));
     dewpoint = 243.5 * gamma / (17.62 - gamma);

    vcc = ESP.getVcc();
    vcc = vcc * 1.1113;
    Serial.print("vcc: ");
    Serial.println(vcc);

    response = "";
    String bodytmp = body.substring(0, body.length());
    bodytmp.replace("TEMP1", String(temp+273, 1));
    bodytmp.replace("HUMID1", String(humidity));
    Serial.println("[Sending a request] "+bodytmp);
    int str_len = bodytmp.length() + 1; 
    char char_array[str_len+1];
    bodytmp.toCharArray(char_array, str_len);

    rclient.setContentType("application/json");
    int statusCode = rclient.post("/signalk/v1/api/_test/delta", char_array, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("Response body from server: ");
    Serial.println(response);
    delay(100);

    // shut down sensor
    digitalWrite(DHTPOWERPIN, LOW);
    // go to sleep
    ESP.deepSleep(10 * 1000000);
  }
}
