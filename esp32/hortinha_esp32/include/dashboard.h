#pragma once

#include <Arduino.h>

/**
 * @brief Gera o HTML da página dashboard
 * @return String com HTML completo (CSS + JS inline)
 *
 * Dashboard responsivo com:
 * - Visualização de sensores em tempo real
 * - Estado do sistema (LED indicador)
 * - Controles e status de irrigação
 * - Download de arquivos CSV
 */
String SendHTML();
