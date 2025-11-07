#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// === CONFIGURAÇÕES DE REDE ===
const char* ssid = "WoKwi-GUEST";            // Nome da rede Wi-Fi
const char* password = "";   // Senha da rede Wi-Fi
String serverName = "https://wet-bobcats-allow.loca.lt/api/velocidade"; // URL do LocalTunnel

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("🔌 Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado ao Wi-Fi!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());

  // Inicializa o MPU6050
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("✅ MPU6050 conectado com sucesso!");
  } else {
    Serial.println("❌ Falha ao conectar MPU6050!");
    while (1);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Leitura de aceleração
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    // Cálculo simples de velocidade (simulação)
    float velocidade = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2) + pow(az / 16384.0, 2)) * 10;

    // Monta JSON
    String payload = "{\"nomeJogadora\":\"Jogadora_A\",\"velocidade\":" + String(velocidade, 2) + "}";
    Serial.print("📤 Enviando: ");
    Serial.println(payload);

    // Envia os dados via POST
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("✅ Resposta do servidor: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("📥 Retorno: " + response);
    } else {
      Serial.print("❌ Erro ao enviar: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("⚠️ Wi-Fi desconectado, reconectando...");
    WiFi.begin(ssid, password);
  }

  delay(5000); // Envia a cada 5 segundos
}