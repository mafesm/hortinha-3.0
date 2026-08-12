#include "../include/serial_proto.h"
#include "../include/led.h"
#include "../include/modos.h"

unsigned long ultimoEnvioUART = 0;

void iniciarSerialArduino()
{
    Serial.begin(115200); // debug local
    delay(50);
    Serial1.begin(9600); // canal UART para ESP32
    delay(50);
    Serial.println("[UART] Iniciado");
}

void enviarNotificacao(const String &tipo)
{
    Serial1.print("NOTIF:");
    Serial1.println(tipo);

    // Feedback visual local
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
}

void enviarDadosUART()
{
    extern float temperatura;
    extern float umidade;
    extern int luz;
    extern bool presenca;
    extern bool modoManual;
    extern bool irrigando;
    extern bool uvcAtivo;
    extern int anguloAtual;

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
}

void processarComandoUART()
{
    extern bool uvcSolicitado;

    if (!Serial1.available())
        return;

    String comando = Serial1.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("CHUVA:"))
    {
        int prob = comando.substring(6).toInt();
        if (prob > 70)
        {
            Serial1.print("[UART] Chuva forte (");
            Serial1.print(prob);
            Serial1.println("%) - Irrigação pausada");
        }
        else if (prob > 40)
        {
            Serial1.print("[UART] Chuva prevista (");
            Serial1.print(prob);
            Serial1.println("%) - Monitorando");
        }
        else
        {
            Serial1.println("[UART] Sem chuva - Normal");
        }
    }
    else if (comando == "GEADA:1")
    {
        Serial1.println("[UART] ALERTA GEADA - Irrigação pausada");
    }
    else if (comando == "GEADA:0")
    {
        Serial1.println("[UART] Alerta geada CANCELADO");
    }
    else if (comando == "UVC:1")
    {
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
