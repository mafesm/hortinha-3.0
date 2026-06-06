# Organização do projeto

Este workspace foi separado em dois projetos principais:

```text
arduino/
└── hortinha_main/
    ├── hortinha_main.ino
    ├── sensores.h
    ├── bomba_nft.h
    ├── uvc.h
    ├── fotoperíodo.h
    ├── modos.h
    ├── serial_proto.h
    └── alertas_local.h

esp32/
└── hortinha_esp32/
    ├── hortinha_esp32.ino
    ├── agendador_uvc.h
    ├── webserver.h
    ├── dashboard.h
    ├── persistencia.h
    ├── coap_client.h
    ├── led_rgb.h
    └── serial_proto.h
```

## O que abrir no Arduino IDE

- Arduino Uno/Mega ou equivalente: [arduino/hortinha_main/hortinha_main.ino](arduino/hortinha_main/hortinha_main.ino)
- ESP32: [esp32/hortinha_esp32/hortinha_esp32.ino](esp32/hortinha_esp32/hortinha_esp32.ino)

## Arquivos de apoio

- Preview local do dashboard: [hortinha20_preview.html](hortinha20_preview.html)
- HTML do painel do ESP32: [esp32/hortinha_esp32/dashboard.h](esp32/hortinha_esp32/dashboard.h)

## Módulos (detalhados)

**Arduino (hortinha_main)**
- **`sensores.h`**: Interface com sensores ambientais — DHT11 (temperatura/umidade), LDR (luminosidade) e PIR (presença). Inclui funções: `iniciarSensores()`, `lerSensores()`, e lógica de fotoperíodo (acumula tempo de luz diário e conta dias com déficit).
- **`bomba_nft.h`**: Controle da válvula/servo da irrigação (modo NFT). Gerencia movimento do servo, modos de economia e funções: `iniciarBombaNFT()`, `abrirIrrigacao()`, `fecharIrrigacao()`, `atualizarServo()` e `controlarIrrigacao()`.
- **`led.h`**: Helpers para LED RGB (estrutura `led` e `ledRGB`), funções de inicialização e controle de cor/piscar (`STAR()`, `COR()`, `PISCA_COR()`, `GEN_RGB()`). Usado para feedback visual local.
- **`modos.h`**: Leitura de DIP switches para determinar modos de operação (manual, ligar/desligar bomba, UVC). Fornece `iniciarModos()` e `lerModos()`.
- **`uvc.h`**: Lógica local associada ao LED UVC: controle do pino, travas de segurança (presença) e funções para ativação/desativação segura do UVC (`iniciarUVCLocal()`, `travaUVC()`, `attUVC()`).
- **`alertas_local.h`**: Regras locais de alerta (histerese para temperatura, umidade, luz), detecta condições críticas e aciona `enviarNotificacao()` quando aplicável.
- **`serial_proto.h`**: Protocolo serial entre Arduino e ESP32 — formata dados em JSON, envia notificações e processa comandos recebidos por UART (ex.: `ECON:`, `UVC:`). Contém `iniciarSerialArduino()`, `enviarNotificacao()` e `processarComandoUART()` .

**ESP32 (hortinha_esp32)**
- **`persistencia.h` / `persistencia.cpp`**: Implementação de persistência com LittleFS. Grava leituras em arquivos diários (`/data/YYYYMMDD.csv`) no formato `<timestamp>,<dados>`, monta o sistema de arquivos e remove arquivos com mais de 15 dias (`limparArquivosAntigos()`). Funções principais: `iniciarPersistencia()` e `escreverLeitura()`.
- **`serial_proto.h`**: Lado ESP32 do protocolo serial; parseia pacotes JSON recebidos do Arduino, mantém struct `PACOTE` com o estado, serializa dados para endpoints e encapsula envio/recebimento via `ENVIA()`/`RECEBE()`.
- **`webserver.h`**: Endpoints HTTP do servidor embutido: `/` (retorna HTML via `SendHTML()`), `/getData` (JSON com estado) e `/setEconomia` (ajusta `modoEconomia`). Integra com `gerarJsonDados()` e `ENVIA()` para repassar comandos ao Arduino.
- **`dashboard.h`**: Template HTML do painel (em string) usado por `SendHTML()` — interface responsiva que consome `/getData` e `/setEconomia` via fetch, exibe sensores e controles.
- **`agendador_uvc.h`**: Agendador que avalia condições ambientais (usa `alertas_espec.h`) e aciona ciclos de UVC automaticamente, incluindo segurança por presença. Funções: `iniciarAgendadorUVC()`, `atualizarAgendadorUVC()`, `iniciarCicloUVC()` e `finalizarCicloUVC()`.
- **`alertas_espec.h`**: Regras especializadas para identificar riscos fitossanitários (Botrytis, Bremia, Pythium, Cercospora) com base em temperatura e umidade, e função `gerarAlertasEspeciais()` que retorna descrições para o dashboard.

## Observações

- Os sketches antigos na raiz foram mantidos como histórico.
- O nome [fotoperíodo.h](arduino/hortinha_main/fotoperíodo.h) usa acento porque foi criado assim na estrutura atual.

## Diagrama UML do Projeto

O diagrama UML abaixo descreve a arquitetura do projeto, mostrando os componentes principais em cada placa e os fluxos de dados entre Arduino, ESP32 e o dashboard web.

- `Arduino Uno/Mega`: sensoriamento local, controle da bomba NFT, UVC, LED RGB, alertas e protocolo serial UART.
- `ESP32`: servidor web HTTP, dashboard HTML, persistência em LittleFS, agendador UVC, alertas especializados e protocolo serial.

Arquivo de diagrama UML: [`project_architecture.puml`](project_architecture.puml) 

- Os sketches antigos na raiz foram mantidos como histórico.
- O nome [fotoperíodo.h](arduino/hortinha_main/fotoperíodo.h) usa acento porque foi criado assim na estrutura atual.

