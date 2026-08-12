#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @struct PACOTE
 * @brief Estrutura com dados do Arduino
 */
struct PACOTE
{
    float temperatura; ///< Temperatura em °C
    float umidade;     ///< Umidade relativa em %
    int luz;           ///< Leitura do sensor LDR (0-1023)
    bool presenca;     ///< Sensor PIR ativo
    bool modoManual;   ///< Sistema em modo manual
    bool irrigando;    ///< Bomba ligada
    bool uvcAtivo;     ///< LED UVC ligado
    int angulo;        ///< Ângulo do servo (0-180)
    String alertas;    ///< String com alertas ativos
    bool temRisco;     ///< Flag de risco ambiental
};

/// Estrutura global com dados atuais do Arduino
extern PACOTE dados;

/**
 * @brief Processa notificações recebidas do Arduino
 * @param tipo String indicando o tipo de notificação (TEMP, LUZ, UMID, PRES, FOTO)
 */
void GEN_NOTF(String tipo);

/**
 * @brief Gera JSON com todos os dados do ambiente
 * @return String formatada em JSON
 */
String gerarJsonDados();

/**
 * @brief Gera JSON compacto com dados principais
 * @return String formatada em JSON (versão reduzida)
 */
String gerarJsonCompacto();

/**
 * @brief Envia comando ao Arduino via UART
 * @param comando String com o comando a enviar
 */
void ENVIA(String comando);

/**
 * @brief Recebe e processa dados do Arduino
 * Deve ser chamada continuamente no loop()
 */
void RECEBE();
