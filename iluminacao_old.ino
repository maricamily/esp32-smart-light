#include <WiFi.h>
#include <WebServer.h>

// id e senha do wifi
const char* ssid = "";
const char* password = "";

// o pino q vai ser conectado ao rele
const int relePin = 5;

// iniciando o servidor na porta 80
WebServer server(80);

// como a lampada esta
bool estadoRele = false;

// HTML da página
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Controle de Iluminação</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body style="text-align: center; font-family: Arial;">
  <h1>Iluminação Inteligente</h1>
  <p>Status da Lâmpada: <strong>%STATUS%</strong></p>
  <form action="/toggle">
    <button style="padding: 12px 24px; font-size: 16px;">Alternar Luz</button>
  </form>
</body>
</html>
)rawliteral";

// function p o status no HTML
String processor(const String& var) {
  if (var == "STATUS") {
    return estadoRele ? "LIGADA" : "DESLIGADA";
  }
  return String();
}

// function principal do server
void handleRoot() {
  String htmlPage = html;
  htmlPage.replace("%STATUS%", estadoRele ? "LIGADA" : "DESLIGADA");
  server.send(200, "text/html", htmlPage);
}

// alternando o estado da lampada
void handleToggle() {
  estadoRele = !estadoRele;
  digitalWrite(relePin, estadoRele ? HIGH : LOW);
  handleRoot();
}

void setup() {
  Serial.begin(115200);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient();
}

