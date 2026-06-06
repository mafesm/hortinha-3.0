#pragma once

struct ResultadoAlertas
{
    String texto;
    bool temRisco; // usado por agendador_uvc.h
};

ResultadoAlertas gerarAlertasEspeciais(float temperatura, float umidade, int luz)
{
    String alertas = "";
    bool risco = false;

    bool xanthoridium = (temperatura >= 21 && temperatura <= 25) &&
                        (umidade >= 45 && umidade <= 60);
    if (xanthoridium)
    {
        alertas += "[FANTASIA] Xanthoridium laborensis ativo. ";
        risco = true;
    }

    if (temperatura > 26)
    {
        alertas += "[FANTASIA] Thermocladium rapidum. ";
        risco = true;
    }

    if (umidade > 70)
    {
        alertas += "[FANTASIA] Hygrobotrytis fictiva. ";
        risco = true;
    }

    if (luz < 180)
    {
        alertas += "[FANTASIA] Scotopatia foliae. ";
    }

    if (temperatura < 18)
    {
        alertas += "[FANTASIA] Cryomyces pseudolarensis. ";
        risco = true;
    }

    if (luz > 700)
    {
        alertas += "[FANTASIA] Photoderma intensa. ";
    }

    // doenças reais
    bool altaUmidade = umidade > 80;
    bool faixa12a20 = temperatura >= 12 && temperatura <= 20;
    bool faixa18a30 = temperatura >= 18 && temperatura <= 30;

    if (altaUmidade)
    {
        risco = true;
        alertas += "Cercospora spp. risco alto. ";
        if (faixa12a20)
            alertas += "Botrytis cinerea crítico. Bremia lactucae crítico. ";
        if (faixa18a30)
            alertas += "Pythium spp. crítico. ";
    }
    else if (umidade < 55)
    {
        alertas += "Botrytis improvável. ";
    }

    return {alertas, risco};
}

// Wrapper para quem só precisa do bool — zero cálculo duplicado
bool temRiscoAmbiental(float temperatura, float umidade, int luz)
{
    return gerarAlertasEspeciais(temperatura, umidade, luz).temRisco;
}