# 🌱 Hortinha 3.0

Sistema embarcado de monitoramento e controle automatizado de horta indoor com NFT (Nutrient Film Technique), desenvolvido como TCC.

---

## Visão Geral

O sistema é composto por dois microcontroladores em comunicação UART:

- **Arduino Mega** — leitura de sensores, controle de atuadores (servo, LED UVC, LED RGB), detecção de modo via chave DIP
- **ESP32** — conectividade Wi-Fi, servidor web HTTP, persistência em LittleFS, agendamento inteligente de UVC e integração com NTP

---

## Hardware

| Componente | Descrição |
|---|---|
| Arduino Mega | Controlador principal de sensores e atuadores |
| ESP32 | Gateway Wi-Fi, servidor web, persistência |
| DHT11 | Sensor de temperatura e umidade (pino 10) |
| LDR | Sensor de luminosidade (pino A0) |
| PIR | Sensor de presença (pino 9) |
| Servo Motor | Controle da válvula de irrigação (pino 8) |
| LED UVC | Esterilização ambiental (pino 3) |
| LED RGB | Indicador visual de estado (pinos 13, 12, 11) |
| Chave DIP 4 vias | Seleção de modos de operação (pinos 4–7) |

---

## Arquitetura

```
Arduino Mega  ←── UART (Serial1 / Serial2) ──→  ESP32
   │                                               │
   ├─ Sensores (DHT11, LDR, PIR)                  ├─ Servidor Web HTTP (porta 80)
   ├─ Servo (válvula NFT)                         ├─ Dashboard HTML
   ├─ LED UVC                                     ├─ Persistência CSV (LittleFS)
   ├─ LED RGB (status)                            ├─ Agendador UVC
   └─ DIP switch (modos)                          └─ NTP (UTC-3 Brasília)
```

---

## Módulos — Arduino

### `sensores.h`
- Leitura de temperatura e umidade (DHT11) a cada ciclo
- Leitura de luminosidade com filtro exponencial (`0.7 * anterior + 0.3 * novo`)
- Detecção de presença via PIR
- Controle de **fotoperíodo**: acumula horas de luz por dia e incrementa `diasDeficitFotoperiodo` se < 12 h/dia

### `bomba_nft.h`
- Controla a válvula de irrigação via servo motor
- Modo automático: verifica a cada 10 s se `umidade < 40%` e `luz < 200` para iniciar ciclo de 15 s
- Modo manual: controlado pela chave DIP
- Animação de varredura do servo enquanto irrigando

### `uvc.h`
- Controla LED UVC para esterilização
- **Trava de segurança**: desliga automaticamente se presença for detectada
- Pode ser ativado por chave DIP ou via comando UART

### `modos.h`
- Lê chave DIP de 4 vias
- Modos disponíveis (combinações de DIP1–DIP4):

| Modo | DIP1 | DIP2 | DIP3 | DIP4 |
|---|---|---|---|---|
| Manual | ✅ | ❌ | ❌ | ❌ |
| Bomba Liga | ✅ | ✅ | ❌ | ❌ |
| Bomba Desliga | ✅ | ❌ | ✅ | ❌ |
| UVC Manual | ✅ | ❌ | ❌ | ✅ |

### `led.h`
- LED RGB indica estado do sistema:

| Cor | Estado |
|---|---|
| 🟢 Verde | Sistema normal (automático) |
| 🔵 Azul | Irrigando |
| 🟠 Laranja | Modo manual |
| 🟣 Roxo | UVC ativo |
| 💜 Magenta | Manual + UVC |

### `alertas_local.h`
- Monitora variáveis com histerese para evitar alertas repetidos:

| Alerta | Condição |
|---|---|
| Calor | Temperatura > 30°C |
| Frio | Temperatura < 12°C |
| Umidade Alta | Umidade > 80% |
| Umidade Baixa | Umidade < 60% |
| Luz Baixa | Luz < 200 |
| Fotoperíodo | 3 dias consecutivos com déficit |
| Presença | PIR ativo |

### `serial_proto.h` (Arduino)
- Envia JSON compacto via UART a cada 1 segundo: `{"temp":..,"umid":..,"luz":..,"pres":..,"manual":..,"irr":..,"uvc":..,"angulo":..}`
- Envia notificações no formato `NOTIF:<TIPO>`
- Processa comandos recebidos: `CHUVA:<prob>`, `GEADA:1/0`, `UVC:1/0`

---

## Módulos — ESP32

### `serial_proto.h` (ESP32)
- Recebe e parseia JSON do Arduino (ArduinoJson)
- Gera alertas de patógenos com base na temperatura e umidade
- Salva leituras em CSV no LittleFS a cada 20 segundos

### `agendador_uvc.h`
- Verifica a cada 10 s se `umidade > 80%` para iniciar ciclo UVC de 15 s
- Interrompe o ciclo imediatamente se presença for detectada
- Envia `UVC:1` / `UVC:0` via UART para o Arduino

### `alertas_espec.h`
- Detecta risco de patógenos fúngicos com base em temperatura, umidade e luz
- Retorna `ResultadoAlertas` com `texto` de alertas e `temRisco`
- Usa `temRiscoAmbiental()` como wrapper para obter apenas o bool sem recalcular

### `persistencia.h` / `persistencia.cpp`
- Armazena leituras em arquivos CSV diários em `/data/YYYYMMDD.csv`
- Limpeza automática de arquivos com mais de 15 dias
- Cabeçalho CSV: `timestamp,json`

### `servidorweb.h`
- Servidor HTTP na porta 80
- Rotas disponíveis:

| Rota | Descrição |
|---|---|
| `GET /` | Dashboard HTML |
| `GET /getData` | JSON com estado atual |
| `GET /listarArquivos` | Lista CSVs disponíveis |
| `GET /baixarArquivo?file=NOME` | Download de CSV |

### `dashboard.h`
- Interface web responsiva com atualização automática a cada 2 s
- Exibe: temperatura, umidade, luminosidade, presença, UVC, modo, irrigação, ângulo do servo
- Alertas visuais de temperatura alta, umidade baixa e risco de patógenos
- Modo noturno visual (fundo escuro quando `modoNoturno` ativo)
- Download de relatórios CSV históricos

---

## Protocolo UART

**Baud rate:** Arduino `Serial1` ↔ ESP32 `Serial2` a **9600 bps**

| Direção | Mensagem | Descrição |
|---|---|---|
| Arduino → ESP32 | `{"temp":25.0,"umid":65.0,...}` | Telemetria a cada 1 s |
| Arduino → ESP32 | `NOTIF:TEMP` | Notificação de alerta |
| ESP32 → Arduino | `CHUVA:75` | Probabilidade de chuva (%) |
| ESP32 → Arduino | `GEADA:1` | Alerta de geada |
| ESP32 → Arduino | `UVC:1` | Ativar UVC |
| ESP32 → Arduino | `UVC:0` | Desativar UVC |

---

## Como Usar

1. Faça o upload do sketch `hortinha_main.ino` no Arduino Mega via Arduino IDE
2. Faça o upload do sketch `hortinha_esp32.ino` no ESP32
3. Configure o SSID e senha Wi-Fi em `hortinha_esp32.ino`:
   ```cpp
   const char* ssid = "SUA_REDE";
   const char* password = "SUA_SENHA";
   ```
4. Conecte os pinos UART:
   - Arduino TX1 (pino 18) → ESP32 RX2 (pino 16)
   - Arduino RX1 (pino 19) → ESP32 TX2 (pino 17)
5. Acesse o dashboard pelo IP exibido no monitor serial do ESP32

---

## Dependências

### Arduino
- `DHT sensor library` (Adafruit)
- `Servo.h` (built-in)
- `ArduinoJson`

### ESP32
- `WiFi.h` (built-in ESP32)
- `WebServer.h` (built-in ESP32)
- `LittleFS.h` (built-in ESP32)
- `ArduinoJson`