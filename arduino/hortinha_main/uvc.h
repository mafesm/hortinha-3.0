#pragma once

#include "led.h"

extern bool presenca;

bool uvcAtivo = false;
bool uvcSolicitado = false;

led uvc;

#define UVC_PIN 3

void travaUVC();

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

void attUVC()
{
    travaUVC();
    if (modoManual && uvcSolicitado)
    {
        digitalWrite(uvc.pin, HIGH);
        return;
    }
    if (uvcAtivo)
    {
        digitalWrite(uvc.pin, HIGH);
    }
}

void travaUVC()
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
