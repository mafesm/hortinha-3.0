#pragma once

struct led
{
    int luz;
    int pin;
    bool pisc;
    unsigned long ult_p;
};

struct ledRGB
{
    led R;
    led G;
    led B;
    int est;
};

extern bool modoManual;
extern bool irrigando;
extern bool uvcAtivo;

ledRGB rgb;

void STAR(int pinR, int pinG, int pinB)
{
    rgb.R.pin = pinR;
    rgb.G.pin = pinG;
    rgb.B.pin = pinB;

    rgb.R.luz = 0;
    rgb.G.luz = 0;
    rgb.B.luz = 0;

    rgb.R.pisc = false;
    rgb.G.pisc = false;
    rgb.B.pisc = false;

    rgb.R.ult_p = 0;
    rgb.G.ult_p = 0;
    rgb.B.ult_p = 0;

    rgb.est = 0;

    pinMode(rgb.R.pin, OUTPUT);
    pinMode(rgb.G.pin, OUTPUT);
    pinMode(rgb.B.pin, OUTPUT);
}

void COR(int r, int g, int b)
{
    rgb.R.luz = r;
    rgb.G.luz = g;
    rgb.B.luz = b;

    analogWrite(rgb.R.pin, r);
    analogWrite(rgb.G.pin, g);
    analogWrite(rgb.B.pin, b);
}

void PISCA_COR(int r, int g, int b, int vezes = 3)
{
    for (int i = 0; i < vezes; i++)
    {
        COR(r, g, b);
        delay(200);
        COR(0, 0, 0);
        delay(200);
    }
}

void GEN_RGB()
{
    if (modoManual)
    {
        if (uvcAtivo)
        {
            COR(255, 0, 255);
            //  Serial.println("[LED] Modo Manual + UVC Ativo - Magenta");
            return;
        }
        COR(255, 165, 0);
        Serial.println("[LED] Modo Manual - Laranja");
        return;
    }

    if (uvcAtivo)
    {
        COR(125, 0, 255);
        //  Serial.println("[LED] UVC Ativo - Roxo");
        return;
    }

    if (irrigando)
    {
        COR(0, 0, 255);
        // Serial.println("[LED] Irrigando - Azul");
        return;
    }

    COR(0, 255, 0);
    // Serial.println("[LED] Sistema Normal - Verde");
}
