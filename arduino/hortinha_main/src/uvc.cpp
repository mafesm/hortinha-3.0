#include "../include/uvc.h"
#include "../include/led.h"
#include "../include/modos.h"
#include "../include/sensores.h"

#define UVC_PIN 3

bool uvcAtivo = false;
bool uvcSolicitado = false;

static led uvc = {};

void iniciarUVCLocal()
{
    uvc.pin = UVC_PIN;
    uvc.luz = 0;
    uvc.pisc = false;
    uvc.ult_p = 0;

    uvcAtivo = false;
    uvcSolicitado = false;

    pinMode(uvc.pin, OUTPUT);
    digitalWrite(uvc.pin, LOW);
    Serial.println("[UVC] LED UVC iniciado");
}

static void travaUVC()
{
    if (uvcSolicitado && presenca)
    {
        uvcAtivo = false;
        uvcSolicitado = false;
        Serial.println("[UVC] Presença detectada - UVC bloqueado");
    }
    else
    {
        uvcAtivo = uvcSolicitado;
    }
}

void attUVC()
{
    travaUVC();
    if (uvcAtivo)
    {
        digitalWrite(uvc.pin, HIGH);
        Serial.println("[UVC] Ligado");
    }
    else
    {
        digitalWrite(uvc.pin, LOW);
    }
}
