#pragma once

#define DIP1 7
#define DIP2 6
#define DIP3 5
#define DIP4 4

bool modoManual = false;
bool modoBombaLiga = false;
bool modoBombaDesliga = false;
extern bool uvcSolicitado;

bool prevModoManual = false;
bool prevModoBombaLiga = false;
bool prevModoBombaDesliga = false;
bool prevuvcSolicitado = false;
bool prevUvcMode = false;

void iniciarModos()
{
    pinMode(DIP1, INPUT_PULLUP);
    pinMode(DIP2, INPUT_PULLUP);
    pinMode(DIP3, INPUT_PULLUP);
    pinMode(DIP4, INPUT_PULLUP);
}

void lerModos()
{
    bool dip4 = !digitalRead(DIP1);
    // Serial.print(dip4);
    bool dip3 = !digitalRead(DIP2);
    // Serial.print(dip3);
    bool dip2 = !digitalRead(DIP3);
    // Serial.print(dip2);
    bool dip1 = !digitalRead(DIP4);
    // Serial.println(dip1);

    modoManual = dip1 && !dip2 && !dip3 && !dip4;
    modoBombaLiga = dip1 && dip2 && !dip3 && !dip4;
    modoBombaDesliga = dip1 && !dip2 && dip3 && !dip4;
    bool uvcMode = dip1 && !dip2 && !dip3 && dip4;

    // Gerar um pulso único em uvcSolicitado apenas na borda de subida do modo UVC.
    if (uvcMode && !prevUvcMode)
    {
        uvcSolicitado = true;
    }
    prevUvcMode = uvcMode;

    if (modoManual != prevModoManual)
    {
        Serial.print("[MODOS] Modo Manual ");
        Serial.println(modoManual ? "ON" : "OFF");
        prevModoManual = modoManual;
    }
    if (modoBombaLiga != prevModoBombaLiga)
    {
        Serial.print("[MODOS] Modo Bomba Liga ");
        Serial.println(modoBombaLiga ? "ON" : "OFF");
        prevModoBombaLiga = modoBombaLiga;
    }
    if (modoBombaDesliga != prevModoBombaDesliga)
    {
        Serial.print("[MODOS] Modo Bomba Desliga ");
        Serial.println(modoBombaDesliga ? "ON" : "OFF");
        prevModoBombaDesliga = modoBombaDesliga;
    }
    if (uvcSolicitado != prevuvcSolicitado)
    {
        Serial.print("[MODOS] Modo UVC Manual ");
        Serial.println(uvcSolicitado ? "ON" : "OFF");
        prevuvcSolicitado = uvcSolicitado;
    }
}
