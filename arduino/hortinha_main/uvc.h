#pragma once

#include "led.h"

extern bool presenca;

bool uvcAtivo;
bool uvcSolicitado;

led uvc;

#define UVC_PIN 3

void travaUVC();

void iniciarUVCLocal() {
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

void attUVC() {
    //Serial.println("att uvc");
  travaUVC();
  if (uvcAtivo) {
    digitalWrite(uvc.pin, HIGH);
    Serial.println("UVC ligado");
  } else digitalWrite(uvc.pin, LOW);
}

void travaUVC() {
  if (uvcSolicitado && presenca) {
    uvcAtivo = false;
    uvcSolicitado = false;
    Serial.println("[UVC] Presença detectada - UVC bloqueado");
  } else {
    uvcAtivo = uvcSolicitado;
    //Serial.println(uvcAtivo);
  }
}
