#pragma once

#include <Servo.h>

extern bool modoBombaLiga;
extern bool modoBombaDesliga;
extern bool modoManual;
extern bool presenca;
extern float umidade;
extern int luz;

bool irrigando = false;
int anguloAtual = 0;
int direcao = 1;
unsigned long ultimoMovimento = 0;
const int VELOCIDADE_BASE = 30;
#define SERVO_PIN 8
Servo valvula;

const unsigned long BOMBA_CHECK_INTERVAL_MS = 10000; // Verifica a cada 10 segundos se as condições para iniciar um ciclo de irrigação estão atendidas
const unsigned long BOMBA_CYCLE_DURATION_MS = 15000; // Duração de cada ciclo de irrigação (15 segundos)
unsigned long ultimoTickBomba = 0;
unsigned long inicioCicloBomba = 0;
bool janelaBombaAtiva = false;

void abrirIrrigacao();
void fecharIrrigacao();
void iniciarAgendadorBomba();
bool condicoesAptasParaCicloBomba();
void iniciarCicloBomba();
void finalizarCicloBomba();
void atualizarAgendadorBomba();

void iniciarBombaNFT()
{
    valvula.attach(SERVO_PIN);
    valvula.write(0);
    fecharIrrigacao();
    iniciarAgendadorBomba();
    Serial.println("[SERVO] Válvula iniciada (fechada)");
}

void abrirIrrigacao()
{
    irrigando = true;
    Serial.println("[IRR] Irrigação ATIVADA");
}

void fecharIrrigacao()
{
    irrigando = false;
    anguloAtual = 0;
    valvula.write(0);
    Serial.println("[IRR] Irrigação DESLIGADA");
}

void atualizarServo()
{
    if (!irrigando)
        return;

    unsigned long agora = millis();

    if (agora - ultimoMovimento >= (unsigned long)VELOCIDADE_BASE)
    {
        ultimoMovimento = agora;
        anguloAtual += direcao * 2;

        if (anguloAtual >= 180)
        {
            anguloAtual = 180;
            direcao = -1;
        }
        else if (anguloAtual <= 0)
        {
            anguloAtual = 0;
            direcao = 1;
        }

        valvula.write(anguloAtual);
    }
}

void iniciarAgendadorBomba()
{
    ultimoTickBomba = millis();
    inicioCicloBomba = 0;
    janelaBombaAtiva = false;
}

bool condicoesAptasParaCicloBomba()
{
    if (modoManual)
        return false;

    return umidade < 40 && luz < 200;
}

void iniciarCicloBomba()
{
    abrirIrrigacao();
    janelaBombaAtiva = true;
    inicioCicloBomba = millis();
    Serial.println("[IRR] Ciclo agendado");
}

void finalizarCicloBomba()
{
    fecharIrrigacao();
    janelaBombaAtiva = false;
    inicioCicloBomba = 0;
    Serial.println("[IRR] Ciclo finalizado");
}

void atualizarAgendadorBomba()
{
    if (millis() - ultimoTickBomba < BOMBA_CHECK_INTERVAL_MS)
        return;

    ultimoTickBomba = millis();

    if (!janelaBombaAtiva)
    {
        if (condicoesAptasParaCicloBomba())
        {
            iniciarCicloBomba();
        }
        return;
    }

    if (millis() - inicioCicloBomba >= BOMBA_CYCLE_DURATION_MS)
    {
        finalizarCicloBomba();
    }
}

void controlarIrrigacao()
{
    if (modoManual && modoBombaLiga)
    {
        abrirIrrigacao();
        janelaBombaAtiva = false;
        return;
    }

    if (modoManual && modoBombaDesliga)
    {
        finalizarCicloBomba();
        return;
    }
    else if (!modoManual)
        atualizarAgendadorBomba();
}
