#pragma once

#include <Arduino.h>

/**
 * @struct ResultadoAlertas
 * @brief Resultado da análise de alertas do ambiente
 */
struct ResultadoAlertas
{
    String texto;  ///< Texto descritivo dos alertas detectados
    bool temRisco; ///< Flag indicando se há risco (usado por agendador_uvc)
};

/**
 * @brief Analisa condições ambientais e gera alertas
 * @param temperatura Temperatura em °C
 * @param umidade Umidade relativa em %
 * @param luz Leitura do sensor de luz (0-1023)
 * @return ResultadoAlertas com texto dos alertas e flag de risco
 */
ResultadoAlertas gerarAlertasEspeciais(float temperatura, float umidade, int luz);

/**
 * @brief Verifica se há risco ambiental (wrapper rápido)
 * @param temperatura Temperatura em °C
 * @param umidade Umidade relativa em %
 * @param luz Leitura do sensor de luz
 * @return true se há risco, false caso contrário
 */
bool temRiscoAmbiental(float temperatura, float umidade, int luz);
