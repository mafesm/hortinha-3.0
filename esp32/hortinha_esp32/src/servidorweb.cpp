#include "../include/servidorweb.h"
#include "../include/serial_proto.h"
#include "../include/dashboard.h"
#include <WebServer.h>
#include <LittleFS.h>

static WebServer server(80);

static void handleRoot()
{
    server.send(200, "text/html", SendHTML());
}

static void handleGetData()
{
    String response = gerarJsonDados();
    server.send(200, "application/json", response);
}

static void handleListarArquivos()
{
    String response = "[";
    File dir = LittleFS.open("/data");
    if (!dir || !dir.isDirectory())
    {
        server.send(200, "application/json", "[]");
        return;
    }

    bool primeiro = true;
    File file = dir.openNextFile();
    while (file)
    {
        if (!primeiro)
            response += ",";
        response += "\"";
        response += file.name();
        response += "\"";
        primeiro = false;
        file.close();
        file = dir.openNextFile();
    }
    response += "]";
    server.send(200, "application/json", response);
}

static void handleBaixarArquivo()
{
    if (!server.hasArg("file"))
    {
        server.send(400, "text/plain", "Parâmetro 'file' ausente");
        return;
    }

    String arg = server.arg("file");
    if (arg.indexOf('/') >= 0 || arg.indexOf('.') != arg.lastIndexOf('.'))
    {
        server.send(400, "text/plain", "Nome inválido");
        return;
    }
    String filename = "/data/" + arg;
    File file = LittleFS.open(filename, FILE_READ);
    if (!file)
    {
        server.send(404, "text/plain", "Arquivo não encontrado");
        return;
    }

    server.sendHeader("Content-Disposition", "attachment; filename=" + server.arg("file"));
    server.streamFile(file, "text/csv");
    file.close();
}

void iniciarWebServer()
{
    server.on("/", handleRoot);
    server.on("/getData", handleGetData);
    server.on("/listarArquivos", handleListarArquivos);
    server.on("/baixarArquivo", handleBaixarArquivo);
    server.begin();
    Serial.println("[WEB] Servidor iniciado na porta 80");
}

void processarWebServer()
{
    server.handleClient();
}
