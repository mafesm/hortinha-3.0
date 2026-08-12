#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <time.h>

/**
 * @brief Inicializa o sistema de arquivos LittleFS
 * @return true se montou com sucesso, false caso contrário
 */
bool iniciarPersistencia();

/**
 * @brief Escreve um JSON em arquivo do dia correspondente ao timestamp
 * @param timestamp Timestamp Unix do momento da leitura
 * @param jsonString String JSON com os dados a salvar
 * @return true se escreveu com sucesso, false caso contrário
 *
 * Cria arquivo CSV com cabeçalho: timestamp,json
 * Formato gravado: <timestamp_unix>,<json_completo>\n
 */
bool escreverLeituraJSON(time_t timestamp, const String &jsonString);

/**
 * @brief Remove arquivos com mais de N dias
 * @param dias Número de dias (padrão 15)
 */
void limparArquivosAntigos(int dias = 15);
