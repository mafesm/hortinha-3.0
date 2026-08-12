#pragma once

#include <Arduino.h>
#include <Servo.h>

/**
 * @brief Inicializa o sistema de bomba NFT
 */
void iniciarBombaNFT();

/**
 * @brief Abre a irrigação (ativa a bomba)
 */
void abrirIrrigacao();

/**
 * @brief Fecha a irrigação (desativa a bomba)
 */
void fecharIrrigacao();

/**
 * @brief Atualiza o servo (movimentação do ângulo)
 * Deve ser chamada continuamente no loop()
 */
void atualizarServo();

/**
 * @brief Inicializa o agendador de ciclos de bomba
 */
void iniciarAgendadorBomba();

/**
 * @brief Verifica se as condições estão aptas para ciclo de irrigação
 * @return true se pode iniciar, false caso contrário
 */
bool condicoesAptasParaCicloBomba();

/**
 * @brief Inicia um ciclo de irrigação
 */
void iniciarCicloBomba();

/**
 * @brief Finaliza o ciclo de irrigação ativo
 */
void finalizarCicloBomba();

/**
 * @brief Atualiza o agendador de irrigação
 * Deve ser chamada continuamente no loop()
 */
void controlarIrrigacao();

// Variáveis globais
extern bool irrigando;
extern int anguloAtual;
