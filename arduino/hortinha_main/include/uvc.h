#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa o LED UVC local
 */
void iniciarUVCLocal();

/**
 * @brief Atualiza o estado do UVC baseado em condições
 * Deve ser chamada continuamente no loop()
 */
void attUVC();

// Variáveis globais
extern bool uvcAtivo;
extern bool uvcSolicitado;
