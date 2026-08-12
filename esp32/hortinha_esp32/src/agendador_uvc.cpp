#include "../include/agendador_uvc.h"
#include "../include/serial_proto.h"

// Timings configuráveis
const unsigned long UVC_CHECK_INTERVAL_MS = 10000; // Verifica a cada 10 segundos
const unsigned long UVC_CYCLE_DURATION_MS = 15000; // Duração de cada ciclo (15 segundos)

// Estados do agendador
unsigned long ultimoTickUVC = 0;
unsigned long inicioCicloUVC = 0;
bool janelaUVCAtiva = false;

bool condicoesAptasParaCicloUVC()
{
    if (dados.modoManual)
        return false;
    return dados.temRisco;
}

void iniciarAgendadorUVC()
{
    ultimoTickUVC = millis();
    inicioCicloUVC = 0;
    janelaUVCAtiva = false;
}

void iniciarCicloUVC()
{
    ENVIA("UVC:1");
    janelaUVCAtiva = true;
    inicioCicloUVC = millis();
    Serial.println("[UVC] Ciclo agendado");
}

void finalizarCicloUVC()
{
    ENVIA("UVC:0");
    janelaUVCAtiva = false;
    inicioCicloUVC = 0;
    Serial.println("[UVC] Ciclo finalizado");
}

void atualizarAgendadorUVC()
{
    if (millis() - ultimoTickUVC < UVC_CHECK_INTERVAL_MS)
        return;

    ultimoTickUVC = millis();

    if (!janelaUVCAtiva)
    {
        if (condicoesAptasParaCicloUVC())
        {
            iniciarCicloUVC();
        }
        return;
    }

    if (dados.presenca && janelaUVCAtiva)
    {
        Serial.println("[UVC] Presença detectada, interrompendo ciclo");
        finalizarCicloUVC();
        return;
    }

    if (millis() - inicioCicloUVC >= UVC_CYCLE_DURATION_MS)
    {
        finalizarCicloUVC();
    }
}
