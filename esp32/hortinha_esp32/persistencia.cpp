#include "persistencia.h"

static String nomeDoArquivoParaData(time_t t)
{
    struct tm *tminfo = localtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "/data/%04d%02d%02d.csv",
             tminfo->tm_year + 1900, tminfo->tm_mon + 1, tminfo->tm_mday);
    return String(buf);
}

static void garantirDiretorioData()
{
    if (!LittleFS.exists("/data"))
    {
        if (!LittleFS.mkdir("/data"))
        {
            Serial.println("[PERSIST] Erro ao criar diretório /data");
        }
    }
}

bool iniciarPersistencia()
{
    // Tenta montar LittleFS; se falhar, tenta formatar e montar novamente.
    if (!LittleFS.begin())
    {
        if (!LittleFS.begin(true))
        {
            return false;
        }
    }

    garantirDiretorioData();

    // Faz uma limpeza inicial de arquivos antigos (15 dias).
    limparArquivosAntigos(15);
    Serial.println("[PERSIST] LittleFS iniciado com sucesso");
    return true;
}

bool escreverLeituraJSON(time_t timestamp, const String &jsonString)
{

    Serial.print("[DEBUG] timestamp: ");
    Serial.println((long)timestamp);

    String nome = nomeDoArquivoParaData(timestamp);
    Serial.print("[DEBUG] arquivo: ");
    Serial.println(nome);

    bool isNewFile = !LittleFS.exists(nome);
    Serial.print("[DEBUG] isNewFile: ");
    Serial.println(isNewFile);

    File f = LittleFS.open(nome, FILE_APPEND);
    Serial.print("[DEBUG] file ok: ");
    Serial.println(f ? "SIM" : "NAO");

    garantirDiretorioData();

    // Verifica se arquivo existe para adicionar cabeçalho se necessário
    if (!f)
    {
        Serial.println("[PERSIST] Erro ao abrir arquivo para escrita");
        return false;
    }

    if (isNewFile)
    {
        f.println("timestamp,json"); // cabeçalho só no arquivo novo
    }

    // Gravamos: timestamp,json
    f.print((uint32_t)timestamp);
    f.print(',');
    f.println(jsonString);
    f.close();

    // Remover arquivos antigos ocasionalmente (a cada 100 gravações)
    static int contador = 0;
    if (++contador >= 100)
    {
        limparArquivosAntigos(15);
        contador = 0;
    }
    return true;
}

static bool parseDateFromFilename(const String &filename, struct tm &out_tm)
{
    // espera nome no formato /data/YYYYMMDD.csv ou YYYYMMDD.csv
    String base = filename;
    int slash = base.lastIndexOf('/');
    if (slash >= 0)
        base = base.substring(slash + 1);
    if (base.length() < 8)
        return false;
    String y = base.substring(0, 4);
    String m = base.substring(4, 6);
    String d = base.substring(6, 8);
    out_tm = {};
    out_tm.tm_year = y.toInt() - 1900;
    out_tm.tm_mon = m.toInt() - 1;
    out_tm.tm_mday = d.toInt();
    out_tm.tm_hour = 0;
    out_tm.tm_min = 0;
    out_tm.tm_sec = 0;
    return true;
}

void limparArquivosAntigos(int dias)
{
    time_t now = time(nullptr);
    File dir = LittleFS.open("/data");
    if (!dir || !dir.isDirectory())
    {
        return;
    }

    File file = dir.openNextFile();
    while (file)
    {
        String fname = file.name();
        struct tm ftm;
        if (parseDateFromFilename(fname, ftm))
        {
            time_t filetime = mktime(&ftm);
            double age = difftime(now, filetime) / 86400.0;
            if (age > dias)
            {
                LittleFS.remove(fname);
                Serial.print("[PERSIST] Arquivo removido: ");
                Serial.println(fname);
            }
        }
        file.close();
        file = dir.openNextFile();
    }
}
