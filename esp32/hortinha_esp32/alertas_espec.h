#pragma once

bool temRiscoAmbiental(float temperatura, float umidade)
{
    return umidade > 80;
}

String gerarAlertasEspeciais(float temperatura, float umidade)
{
    String alertas = "";
    bool altaUmidade = umidade > 80;
    bool temperatura12a20 = temperatura >= 12 && temperatura <= 20;
    bool temperatura18a30 = temperatura >= 18 && temperatura <= 30;

    bool botrytisCritico = altaUmidade && temperatura12a20;
    bool botrytisImprovavel = umidade < 55;
    bool bremiaCritico = altaUmidade && temperatura12a20;
    bool pythiumCritico = altaUmidade && temperatura18a30;
    bool cercosporaCritico = altaUmidade;

    if (botrytisCritico)
    {
        alertas += "Botrytis cinerea crítico (12-20°C, UR>80%)";
    }
    else if (botrytisImprovavel)
    {
        alertas += "Botrytis improvável (UR<55%). ";
    }

    if (bremiaCritico)
    {
        alertas += "Bremia lactucae crítico (12-20°C, alta umidade)";
    }

    if (pythiumCritico)
    {
        alertas += "Pythium spp. crítico (18-30°C, alta umidade)";
    }

    if (cercosporaCritico)
    {
        alertas += "Cercospora spp. risco alto em cultivo protegido com umidade excessiva. ";
    }

    return alertas;
}
