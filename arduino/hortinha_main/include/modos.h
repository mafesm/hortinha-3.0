#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa o sistema de seletores de modo (DIP switches)
 */
void iniciarModos();

/**
 * @brief Faz leitura dos DIPs e atualiza estados
 * Deve ser chamada continuamente no loop()
 */
void lerModos();

// Variáveis globais de modo
extern bool modoManual;
extern bool modoBombaLiga;
extern bool modoBombaDesliga;
extern bool uvcSolicitado;
