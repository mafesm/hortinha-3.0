#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa os sensores (DHT11, LDR, PIR)
 */
void iniciarSensores();

/**
 * @brief Faz leitura de todos os sensores
 * Atualiza variáveis globais: temperatura, umidade, luz, presenca
 */
void lerSensores();

/**
 * @brief Inicializa o sistema de fotoperíodo
 */
void iniciarFotoperiodo();

/**
 * @brief Atualiza o contador de fotoperíodo
 * Deve ser chamada continuamente no loop()
 */
void atualizarFotoperiodo();

// Variáveis globais de sensores
extern float temperatura;
extern float umidade;
extern int luz;
extern bool presenca;
extern int diasDeficitFotoperiodo;
