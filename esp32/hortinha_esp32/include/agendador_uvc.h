#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa o agendador de UVC
 */
void iniciarAgendadorUVC();

/**
 * @brief Verifica se as condições estão aptas para iniciar ciclo de UVC
 * @return true se pode iniciar, false caso contrário
 */
bool condicoesAptasParaCicloUVC();

/**
 * @brief Inicia um ciclo de UVC
 */
void iniciarCicloUVC();

/**
 * @brief Finaliza o ciclo de UVC ativo
 */
void finalizarCicloUVC();

/**
 * @brief Atualiza o estado do agendador de UVC
 * Deve ser chamada continuamente no loop()
 */
void atualizarAgendadorUVC();
