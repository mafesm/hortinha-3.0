#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa os canais UART (Serial1 para ESP32, Serial para debug)
 */
void iniciarSerialArduino();

/**
 * @brief Envia notificação para o ESP32
 * @param tipo String indicando tipo: TEMP, LUZ, UMID, PRES, FOTO
 */
void enviarNotificacao(const String &tipo);

/**
 * @brief Envia dados do ambiente em JSON para o ESP32
 */
void enviarDadosUART();

/**
 * @brief Processa comandos recebidos do ESP32
 * Deve ser chamada continuamente no loop()
 */
void processarComandoUART();

// Variável de controle de timing
extern unsigned long ultimoEnvioUART;
