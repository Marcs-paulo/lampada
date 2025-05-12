#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Wi-Fi
#define WIFI_SSID "brisa-63201"
#define WIFI_PASSWORD "4z08no6d"

// Firebase
#define FIREBASE_HOST "lampada-b9268-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyD3kW55aQ6SaLyz54CcL5MsTUXuPu5FT_c"

// Relé
#define PIN_RELE 5

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

int falhasConsecutivas = 0;
const int MAX_FALHAS = 5;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, HIGH); // Desliga por segurança ao iniciar

  // Conecta ao Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi conectado.");

  // Configuração do Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Leitura inicial do estado no Firebase
  if (Firebase.getString(firebaseData, "/luz/status")) {
    String estado = firebaseData.stringData();
    if (estado == "ligado") {
      digitalWrite(PIN_RELE, LOW);
      Serial.println("Estado inicial: Relé LIGADO");
    } else {
      digitalWrite(PIN_RELE, HIGH);
      Serial.println("Estado inicial: Relé DESLIGADO");
    }
  } else {
    digitalWrite(PIN_RELE, HIGH); // Mantém desligado por segurança
    Serial.print("Falha ao ler estado inicial: ");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() {
  // Verifica conexão Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Reconectando...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
      delay(500);
      Serial.print(".");
      tentativas++;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nFalha ao reconectar Wi-Fi. Reiniciando...");
      ESP.restart();
    }
    Serial.println("\nWi-Fi reconectado.");
  }

  // Leitura do estado no Firebase
  if (Firebase.getString(firebaseData, "/luz/status")) {
    falhasConsecutivas = 0;
    String estado = firebaseData.stringData();
    Serial.print("Estado recebido: ");
    Serial.println(estado);

    if (estado == "ligado") {
      digitalWrite(PIN_RELE, LOW);
      Serial.println("Relé LIGADO");
    } else if (estado == "desligado") {
      digitalWrite(PIN_RELE, HIGH);
      Serial.println("Relé DESLIGADO");
    } else {
      Serial.println("Comando desconhecido.");
    }
  } else {
    falhasConsecutivas++;
    Serial.print("Erro ao ler /luz/status: ");
    Serial.println(firebaseData.errorReason());

    if (falhasConsecutivas >= MAX_FALHAS) {
      Serial.println("Falhas consecutivas detectadas. Reiniciando...");
      ESP.restart();
    }
  }

  delay(1000); // Aguarda um tempo antes da próxima verificação
}


