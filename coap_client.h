#pragma once

void callback_info(CoapPacket &packet, IPAddress ip, int port)
{
    String response = gerarJsonDados();
    coap.sendResponse(ip, port, packet.messageid, response.c_str());
    Serial.println("[CoAP] GET /info requisitado");
}

void callback_comando(CoapPacket &packet, IPAddress ip, int port)
{
    char payload[packet.payloadlen + 1];
    memcpy(payload, packet.payload, packet.payloadlen);
    payload[packet.payloadlen] = '\0';

    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);

    if (doc.containsKey("modo_noturno"))
    {
        modoNoturno = doc["modo_noturno"];
        ENVIA(modoNoturno ? "NOTURNO:1" : "NOTURNO:0");
    }

    if (doc.containsKey("previsao_chuva"))
    {
        int prob = doc["previsao_chuva"];
        ENVIA("CHUVA:" + String(prob));
    }

    String response = "{\"status\":\"ok\"}";
    coap.sendResponse(ip, port, packet.messageid, response.c_str());
}

void iniciarCoap()
{
    coap.server(callback_info, "info");
    coap.server(callback_comando, "comando");
    coap.start();
}
