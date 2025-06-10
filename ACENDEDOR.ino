#include <WiFi.h>

const char* ssid = "Gui";
const char* password = "guigui69";

WiFiServer server(80);

const int ledVerde = 2;
const int ledVermelho = 4;

unsigned long previousMillis = 0;
const long interval = 1000; // 1 segundo
bool estadoVerde = false;
bool modoAlternancia = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

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
  // Alternância automática
  if (modoAlternancia) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      estadoVerde = !estadoVerde;
      digitalWrite(ledVerde, estadoVerde ? HIGH : LOW);
      digitalWrite(ledVermelho, estadoVerde ? LOW : HIGH);
    }
  }

  // Servidor Web
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Comandos manuais
    if (request.indexOf("/verde/on") != -1) {
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledVermelho, LOW);
      modoAlternancia = false;
    }
    if (request.indexOf("/verde/off") != -1) {
      digitalWrite(ledVerde, LOW);
      modoAlternancia = false;
    }
    if (request.indexOf("/vermelho/on") != -1) {
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledVerde, LOW);
      modoAlternancia = false;
    }
    if (request.indexOf("/vermelho/off") != -1) {
      digitalWrite(ledVermelho, LOW);
      modoAlternancia = false;
    }

    // Alternância
    if (request.indexOf("/alternar/on") != -1) {
      modoAlternancia = true;
    }
    if (request.indexOf("/alternar/off") != -1) {
      modoAlternancia = false;
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, LOW);
    }

    // Resposta HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<style>");
    client.println("body { font-family: Arial; background-color: #f4f4f4; text-align: center; padding: 50px; }");
    client.println("button { font-size: 18px; padding: 12px 30px; margin: 10px; border: none; border-radius: 8px; cursor: pointer; }");
    client.println(".verde { background-color: #4CAF50; color: white; }");
    client.println(".vermelho { background-color: #f44336; color: white; }");
    client.println(".alternar { background-color: #2196F3; color: white; }");
    client.println("</style></head><body>");
    client.println("<h1>Controle dos LEDs</h1>");

    client.println("<h2>LED Verde</h2>");
    client.println("<button class=\"verde\" onclick=\"location.href='/verde/on'\">Ligar Verde</button>");
    client.println("<button class=\"verde\" onclick=\"location.href='/verde/off'\">Desligar Verde</button>");

    client.println("<h2>LED Vermelho</h2>");
    client.println("<button class=\"vermelho\" onclick=\"location.href='/vermelho/on'\">Ligar Vermelho</button>");
    client.println("<button class=\"vermelho\" onclick=\"location.href='/vermelho/off'\">Desligar Vermelho</button>");

    client.println("<h2>Modo Alternância</h2>");
    client.println("<button class=\"alternar\" onclick=\"location.href='/alternar/on'\">Ativar Alternancia</button>");
    client.println("<button class=\"alternar\" onclick=\"location.href='/alternar/off'\">Desativar Alternancia</button>");

    client.println("</body></html>");
    client.stop();
  }
}
