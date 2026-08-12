#pragma once

#include <Arduino.h>
#include <WebServer.h>

/**
 * @brief Inicializa o servidor web na porta 80
 * Configura rotas para:
 *   - GET  /               : página HTML principal
 *   - GET  /getData        : JSON com dados atuais
 *   - GET  /listarArquivos : lista de arquivos CSV
 *   - GET  /baixarArquivo  : download de arquivo CSV
 */
void iniciarWebServer();

/**
 * @brief Processa requisições do servidor web
 * Deve ser chamada continuamente no loop()
 */
void processarWebServer();
