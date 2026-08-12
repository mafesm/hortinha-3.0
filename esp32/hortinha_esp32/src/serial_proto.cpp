#include "../include/serial_proto.h"
#include "../include/persistencia.h"
#include "../include/alertas_espec.h"

// Instância global da estrutura de dados
PACOTE dados = {};

// Controle de salvamento periódico
unsigned long ultimoSalvamentoJSON = 0;
const unsigned long INTERVALO_SALVAMENTO_MS = 20000; // Salva a cada 20 segundos

void GEN_NOTF(String tipo)
{
    Serial.print("[NOTIF] Arduino: ");
    Serial.println(tipo);

    if (tipo == "TEMP")
    {
        Serial.println("  -> Temperatura anormal no Arduino");
    }
    else if (tipo == "LUZ")
    {
        Serial.println("  -> Luz insuficiente no Arduino");
    }
    else if (tipo == "UMID")
    {
        Serial.println("  -> Umidade anormal no Arduino");
    }
    else if (tipo == "PRES")
    {
        Serial.println("  -> Presença detectada no Arduino");
    }
    else if (tipo == "FOTO")
    {
        Serial.println("  -> Fotoperíodo baixo no Arduino");
    }
}

String gerarJsonDados()
{
    StaticJsonDocument<512> doc;

    doc["temperatura"] = dados.temperatura;
    doc["umidade"] = dados.umidade;
    doc["luz"] = dados.luz;
    doc["presenca"] = dados.presenca;
    doc["modoManual"] = dados.modoManual;
    doc["irrigando"] = dados.irrigando;
    doc["uvcAtivo"] = dados.uvcAtivo;
    doc["angulo"] = dados.angulo;
    doc["alertas"] = dados.alertas;

    String response;
    serializeJson(doc, response);
    return response;
}

String gerarJsonCompacto()
{
    StaticJsonDocument<256> doc;

    doc["temp"] = dados.temperatura;
    doc["umid"] = dados.umidade;
    doc["luz"] = dados.luz;
    doc["irr"] = dados.irrigando;

    String response;
    serializeJson(doc, response);
    return response;
}

void ENVIA(String comando)
{
    SERIAL_MEGA.println(comando);
    Serial.print("[UART] Enviado: ");
    Serial.println(comando);
}

void RECEBE()
{
    if (!SERIAL_MEGA.available())
        return;

    String linha = SERIAL_MEGA.readStringUntil('\n');
    linha.trim();

    Serial.print("[UART] Recebido ");
    Serial.println(linha);

    if (linha.startsWith("NOTIF:"))
    {
        String tipo = linha.substring(6);
        GEN_NOTF(tipo);
        return;
    }

    if (linha.startsWith("["))
    {
        Serial.println("[arduino] " + linha);
        return;
    }

    if (linha.startsWith("{"))
    {
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, linha);

        if (error)
        {
            Serial.print("[UART] Erro ao parsear JSON: ");
            Serial.println(error.c_str());
            return;
        }

        dados.temperatura = doc["temp"] | 0.0;
        dados.umidade = doc["umid"] | 0.0;
        dados.luz = doc["luz"] | 0;
        dados.presenca = doc["pres"] | false;
        dados.modoManual = doc["manual"] | false;
        dados.irrigando = doc["irr"] | false;
        dados.uvcAtivo = doc["uvc"] | false;
        dados.angulo = doc["angulo"] | 0;
        auto resultado = gerarAlertasEspeciais(dados.temperatura, dados.umidade, dados.luz);
        dados.alertas = resultado.texto;
        dados.temRisco = resultado.temRisco;

        // Salvar JSON a cada intervalo definido
        if (millis() - ultimoSalvamentoJSON >= INTERVALO_SALVAMENTO_MS)
        {
            ultimoSalvamentoJSON = millis();
            String jsonCompleto = gerarJsonDados();
            if (escreverLeituraJSON(time(nullptr), jsonCompleto))
            {
                Serial.println("[PERSIST] JSON salvo com sucesso");
            }
            else
            {
                Serial.println("[PERSIST] Erro ao salvar JSON");
            }
        }
    }
}
