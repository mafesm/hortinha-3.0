#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa o sistema de alertas locais
 */
void iniciarAlertasLocais();

/**
 * @brief Verifica as condições ambientais e dispara alertas
 * Deve ser chamada continuamente no loop()
 */
void verificarAlertas();

// Variáveis globais de alertas
extern bool alertaCalor;
extern bool alertaFrio;
extern bool alertaUmidAlta;
extern bool alertaUmidBaixa;
extern bool alertaLuz;
extern bool alertaFotoperiodo;
extern bool alertaPresenca;
