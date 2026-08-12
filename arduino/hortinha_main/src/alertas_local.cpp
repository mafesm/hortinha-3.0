#include "../include/alertas_local.h"
#include "../include/sensores.h"
#include "../include/serial_proto.h"

// Histerese dos alertas
#define HISTERESE_TEMP 5
#define HISTERESE_UMID 5
#define HISTERESE_LUZ 30

// Estados dos alertas
bool alertaCalor = false;
bool alertaFrio = false;
bool alertaUmidAlta = false;
bool alertaUmidBaixa = false;
bool alertaLuz = false;
bool alertaFotoperiodo = false;
bool alertaPresenca = false;

void iniciarAlertasLocais()
{
    delay(100);
    Serial.println("[ALERTAS] Sistema de alertas iniciado");
}

void verificarAlertas()
{
    if (temperatura > 30 && !alertaCalor)
    {
        alertaCalor = true;
        enviarNotificacao("TEMP");
        Serial.print(temperatura);
        Serial.println("°C [ALERTA] Crítico — risco de pendoamento precoce");
    }
    else if (temperatura <= (30 - HISTERESE_TEMP) && alertaCalor)
    {
        alertaCalor = false;
    }

    if (temperatura < 12 && !alertaFrio)
    {
        alertaFrio = true;
        enviarNotificacao("TEMP");
        Serial.print(temperatura);
        Serial.println("°C [ALERTA] Abaixo do ideal — crescimento lento");
    }
    else if (temperatura >= (12 + HISTERESE_TEMP) && alertaFrio)
    {
        alertaFrio = false;
    }

    if (umidade > 80 && !alertaUmidAlta)
    {
        alertaUmidAlta = true;
        enviarNotificacao("UMID");
        Serial.println("[ALERTA] Risco alto de Botrytis, míldio e Cercospora");
    }
    else if (umidade <= (80 - HISTERESE_UMID) && alertaUmidAlta)
    {
        alertaUmidAlta = false;
    }

    if (umidade < 60 && !alertaUmidBaixa)
    {
        alertaUmidBaixa = true;
        enviarNotificacao("UMID");
        Serial.println("[ALERTA] Estresse hídrico foliar");
    }
    else if (umidade >= (60 + HISTERESE_UMID) && alertaUmidBaixa)
    {
        alertaUmidBaixa = false;
    }

    if (luz < 200 && !alertaLuz)
    {
        alertaLuz = true;
        enviarNotificacao("LUZ");
        Serial.println("[ALERTA] Luz insuficiente - abaixo do mínimo indoor");
    }
    else if (luz >= (200 + HISTERESE_LUZ) && alertaLuz)
    {
        alertaLuz = false;
    }

    if (diasDeficitFotoperiodo >= 3 && !alertaFotoperiodo)
    {
        alertaFotoperiodo = true;
        enviarNotificacao("FOTO");
        Serial.println("[ALERTA] Fotoperíodo baixo por 3 dias - risco de pendoamento");
    }
    else if (diasDeficitFotoperiodo == 0 && alertaFotoperiodo)
    {
        alertaFotoperiodo = false;
    }

    if (presenca && !alertaPresenca)
    {
        alertaPresenca = true;
        enviarNotificacao("PRES");
        Serial.println("[ALERTA] Presença detectada!");
    }
    else if (!presenca && alertaPresenca)
    {
        alertaPresenca = false;
    }
}
