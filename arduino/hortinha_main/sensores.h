#pragma once

#include <DHT.h>

#define DHTPIN 10
#define DHTTYPE DHT11
#define LDR_PIN A0
#define PIR_PIN 9

DHT dht(DHTPIN, DHTTYPE);

float temperatura = 0.0;
float umidade = 0.0;
int luz = 0;
bool presenca = false;

unsigned long ultimoLeituraLuz = 0;
int luzFiltrada = 0;

unsigned long tempoLuzAcumuladoMs = 0;
unsigned long ultimoTickFotoperiodo = 0;
unsigned long ultimoResetFotoperiodo = 0;
int diasDeficitFotoperiodo = 0;
bool fotoperiodoAtivo = false;

void iniciarSensores()
{
    dht.begin();

    pinMode(LDR_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);

    Serial.println("Sensores iniciados.");
}

void lerSensores()
{
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    presenca = digitalRead(PIR_PIN);

    if (millis() - ultimoLeituraLuz >= 1000) // Atualiza a leitura da luz a cada 1segundos
    {
        ultimoLeituraLuz = millis();
        int leituraAtual = analogRead(LDR_PIN);
        luzFiltrada = (luzFiltrada * 0.7) + (leituraAtual * 0.3);
        luz = luzFiltrada;
    }

    if (isnan(temperatura))
        temperatura = 0.0;
    if (isnan(umidade))
        umidade = 0.0;
}

void iniciarFotoperiodo()
{
    tempoLuzAcumuladoMs = 0;
    ultimoTickFotoperiodo = millis();
    ultimoResetFotoperiodo = millis();
    diasDeficitFotoperiodo = 0;
    fotoperiodoAtivo = false;
}

void atualizarFotoperiodo()
{
    unsigned long agora = millis();
    if (agora - ultimoTickFotoperiodo < 1000) // Atualiza a cada 1 segundos
        return;

    unsigned long delta = agora - ultimoTickFotoperiodo;
    ultimoTickFotoperiodo = agora;

    fotoperiodoAtivo = luz >= 200;
    if (fotoperiodoAtivo)
    {
        tempoLuzAcumuladoMs += delta;
    }

    const unsigned long DIA_MS = 86400000UL;
    if (agora - ultimoResetFotoperiodo >= DIA_MS)
    {
        unsigned long diasPassados = (agora - ultimoResetFotoperiodo) / DIA_MS;
        for (unsigned long i = 0; i < diasPassados; i++)
        {
            if (tempoLuzAcumuladoMs < 12UL * 3600000UL)
                diasDeficitFotoperiodo++;
            else
                diasDeficitFotoperiodo = 0;

            tempoLuzAcumuladoMs = 0;
        }

        ultimoResetFotoperiodo += diasPassados * DIA_MS;
    }
}