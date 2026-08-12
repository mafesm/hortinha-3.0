#define SERIAL_MEGA Serial2
#define RX_PIN 16 // rx 2
#define TX_PIN 17 // tx 2

#include <WiFi.h>
const char* ssid = "mafe_sm";
const char* password = "png_sol24";

// ===== Headers =====
#include "include/persistencia.h"
#include "include/alertas_espec.h"
#include "include/serial_proto.h"
#include "include/agendador_uvc.h"
#include "include/dashboard.h"
#include "include/servidorweb.h"

void setup() {
  Serial.begin(115200);                                    
  SERIAL_MEGA.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  configTime(-3 * 3600, 0, "pool.ntp.org", "time.google.com"); // UTC-3 (Brasília)
  Serial.print("Sincronizando NTP");
  time_t agora = time(nullptr);
  while (agora < 1000000000) { // aguarda timestamp válido
    delay(500);
    Serial.print(".");
    agora = time(nullptr);
  }
  Serial.println("\nNTP sincronizado!");
  Serial.print("Hora atual: ");
  Serial.println(ctime(&agora));

  if (!iniciarPersistencia()) {
      Serial.println("LITTLEFS FALHOU!");
  }
  iniciarAgendadorUVC();

  iniciarWebServer();


  Serial.println("\n===========================================");
  Serial.println("HORTINHA 3.0 - ESP32 (UART)");
  Serial.println("===========================================");
  Serial.print("Dashboard HTTP: http://");
  Serial.println(WiFi.localIP());
  Serial.print(WiFi.localIP());
  Serial.println(":5683");
  Serial.println("===========================================\n");
}

void loop() {
  processarWebServer();
  RECEBE();
  
  atualizarAgendadorUVC();
  delay(10);
}
