#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <time.h>

// Inicializa o sistema de arquivos LittleFS. Retorna true se montou com sucesso.
bool iniciarPersistencia();

// Escreve um JSON em arquivo do dia correspondente ao timestamp.
// Cria arquivo CSV com cabeçalho: timestamp,json
// Formato gravado: <timestamp_unix>,<json_completo>\n
bool escreverLeituraJSON(time_t timestamp, const String &jsonString);

// Remove arquivos com mais de `dias` dias (padrão 15).
void limparArquivosAntigos(int dias = 15);
