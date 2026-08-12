#include "../include/bomba_nft.h"
#include "../include/modos.h"
#include "../include/sensores.h"

// Hardware
#define SERVO_PIN 8
static Servo valvula;

// Estados e timings
bool irrigando = false;
int anguloAtual = 0;
static int direcao = 1;
static unsigned long ultimoMovimento = 0;
const int VELOCIDADE_BASE = 30;

const unsigned long BOMBA_CHECK_INTERVAL_MS = 10000; // Verifica a cada 10 segundos
const unsigned long BOMBA_CYCLE_DURATION_MS = 15000; // Duração ciclo: 15 segundos

static unsigned long ultimoTickBomba = 0;
static unsigned long inicioCicloBomba = 0;
static bool janelaBombaAtiva = false;

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

static void atualizarAgendadorBomba()
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
