#include <WiFi.h>

const char* ssid = "C137";       // <<<<<<< NÃO ESQUEÇA O ; NO FINAL
const char* password = "bacalhau2022";   // <<<<<<< ISSO AQUI TAMBÉM PRECISA DO ;

WiFiServer server(80);

const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/on") != -1) {
      digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("/off") != -1) {
      digitalWrite(ledPin, LOW);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<style>");
    client.println("body { font-family: Arial; text-align: center; padding-top: 50px; background-color: #f2f2f2; }");
    client.println("h1 { color: #333; }");
    client.println("button { padding: 20px 40px; font-size: 20px; margin: 20px; border: none; border-radius: 10px; cursor: pointer; }");
    client.println(".on { background-color: #4CAF50; color: white; }");
    client.println(".off { background-color: #f44336; color: white; }");
    client.println("</style></head>");
    client.println("<body><h1>Controle da Luz</h1>");
    client.println("<button class=\"on\" onclick=\"location.href='/on'\">LIGAR</button>");
    client.println("<button class=\"off\" onclick=\"location.href='/off'\">DESLIGAR</button>");
    client.println("</body></html>");


    delay(10);
    client.stop();
  }
}
