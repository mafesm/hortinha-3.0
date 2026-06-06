#include <DHT.h>

#define DHTPIN 10
#define DHTTYPE DHT11
#define LDR_PIN A0
#define PIR_PIN 9

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  delay(1000);
  Serial.println("Sensor Calibration Test");
  Serial.println("------------------------");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int luz = analogRead(LDR_PIN);
  bool presenca = digitalRead(PIR_PIN);

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler DHT11. Verifique conexoes e alimentacao.");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura, 1);
    Serial.println(" C");

    Serial.print("Umidade:    ");
    Serial.print(umidade, 1);
    Serial.println(" %");
  }

  Serial.print("Luz (A0):   ");
  Serial.println(luz);
  Serial.print("Presenca:   ");
  Serial.println(presenca ? "SIM" : "NAO");
  Serial.println("------------------------");

  delay(2000);
}
