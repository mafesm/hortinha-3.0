#pragma once
#include <ArduinoJson.h>

unsigned long ultimoEnvioUART = 0;

// Serial1: canal de comando/telemetria UART
// rx do ESP32 -> pin 18 do Arduino
// tx do ESP32 -> pin 19 do Arduinos
// Serial: canal de debug local (monitor serial)
void GEN_NOTF(const String &tipo)
{
    if (tipo == "TEMP")
    {
        PISCA_COR(255, 0, 0); // vermelho
        Serial1.println("[NOTIF] Temperatura anormal");
    }
    else if (tipo == "LUZ")
    {
        PISCA_COR(255, 255, 0); // amarelo
        Serial1.println("[NOTIF] Muita luz");
    }
    else if (tipo == "UMID")
    {
        PISCA_COR(0, 255, 255); // ciano
        Serial1.println("[NOTIF] Umidade anormal");
    }
    else if (tipo == "PRES")
    {
        PISCA_COR(255, 165, 0); // laranja
        Serial1.println("[NOTIF] Presença identificada");
    }
    else if (tipo == "FOTO")
    {
        PISCA_COR(128, 0, 128); // roxo
        Serial1.println("[NOTIF] Fotoperíodo baixo");
    }
    // GEN_RGB();
}

void iniciarSerialArduino()
{
    Serial.begin(115200); // debug local
    delay(50);
    Serial1.begin(9600); // canal de comando UART (sincronizar com ESP32 Serial2)
    delay(50);
    Serial.println("[UART] iniciado");
}

void enviarNotificacao(const String &tipo)
{
    Serial1.print("NOTIF:");
    Serial1.println(tipo);
    GEN_NOTF(tipo);
}

void enviarDadosUART()
{
    Serial1.print("{");
    Serial1.print("\"temp\":");
    Serial1.print(temperatura, 1);
    Serial1.print(",\"umid\":");
    Serial1.print(umidade, 1);
    Serial1.print(",\"luz\":");
    Serial1.print(luz);
    Serial1.print(",\"pres\":");
    Serial1.print(presenca ? 1 : 0);
    Serial1.print(",\"manual\":");
    Serial1.print(modoManual ? 1 : 0);
    Serial1.print(",\"irr\":");
    Serial1.print(irrigando ? 1 : 0);
    Serial1.print(",\"uvc\":");
    Serial1.print(uvcAtivo ? 1 : 0);
    Serial1.print(",\"angulo\":");
    Serial1.print(anguloAtual);
    Serial1.println("}");

    // Serial.println("[UART] Dados enviados");
}

void processarComandoUART()
{
    if (!Serial1.available())
        return;

    String comando = Serial1.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("CHUVA:"))
    {
        int prob = comando.substring(6).toInt();
        if (prob > 70)
        {
            fecharIrrigacao();
            Serial1.print("[UART] Chuva forte (");
            Serial1.print(prob);
            Serial1.println("%) - Irrigação pausada");
        }
        else if (prob > 40)
        {
            Serial1.print("[UART] Chuva prevista (");
            Serial1.print(prob);
            Serial1.println("%) - Monitorando condição");
        }
        else
        {
            Serial1.println("[UART] Sem chuva - Normal");
        }
    }
    else if (comando == "GEADA:1")
    {
        fecharIrrigacao();
        Serial1.println("[UART] ALERTA GEADA - Irrigação pausada");
    }
    else if (comando == "GEADA:0")
    {
        Serial1.println("[UART] Alerta geada CANCELADO");
    }
    else if (comando == "UVC:1")
    {
        //Serial.println("[UART] uvc solicitado")
        uvcSolicitado = true;
        Serial.println("[UART] UVC solicitado");
    }
    else if (comando == "UVC:0")
    {
        uvcSolicitado = false;
        Serial.println("[UART] UVC desligado");
    }
    else
    {
        Serial.print("[UART] Comando desconhecido: ");
        Serial.println(comando);
    }
}
