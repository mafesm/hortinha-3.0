# Estrutura de Código Reorganizada - Hortinha 3.0

## 📁 Organização

O projeto foi reorganizado seguindo as **melhores práticas de C/C++** com separação clara entre declarações (headers) e implementações (CPP).

### Estrutura ESP32

```
esp32/hortinha_esp32/
├── hortinha_esp32.ino      # Arquivo principal (entrypoint)
├── include/                 # Headers (apenas declarações)
│   ├── persistencia.h
│   ├── alertas_espec.h
│   ├── serial_proto.h
│   ├── agendador_uvc.h
│   ├── servidorweb.h
│   └── dashboard.h
└── src/                     # Implementações (.cpp)
    ├── persistencia.cpp
    ├── alertas_espec.cpp
    ├── serial_proto.cpp
    ├── agendador_uvc.cpp
    ├── servidorweb.cpp
    └── dashboard.cpp
```

### Estrutura Arduino

```
arduino/hortinha_main/
├── hortinha_main.ino       # Arquivo principal (entrypoint)
├── include/                 # Headers (apenas declarações)
│   ├── sensores.h
│   ├── bomba_nft.h
│   ├── uvc.h
│   ├── led.h
│   ├── modos.h
│   ├── alertas_local.h
│   └── serial_proto.h
└── src/                     # Implementações (.cpp)
    ├── sensores.cpp
    ├── bomba_nft.cpp
    ├── uvc.cpp
    ├── led.cpp
    ├── modos.cpp
    ├── alertas_local.cpp
    └── serial_proto.cpp
```

## 🔧 Padrões e Boas Práticas

### 1. **Headers Limpos** (em `include/`)

Headers contêm **apenas declarações**:
- Estruturas e tipos (`struct`, `enum`)
- Declarações de funções com documentação Doxygen
- Variáveis globais externas (`extern`)
- Constantes de configuração
- Includes das dependências necessárias

**Exemplo:**
```cpp
#pragma once

#include <Arduino.h>

/**
 * @brief Inicializa o sistema de sensores
 * Configura DHT11, LDR e PIR
 */
void iniciarSensores();

/**
 * @brief Faz leitura de todos os sensores
 * Atualiza variáveis globais: temperatura, umidade, luz, presenca
 */
void lerSensores();

// Variáveis globais
extern float temperatura;
extern float umidade;
```

### 2. **Implementações Organizadas** (em `src/`)

Arquivos .cpp contêm:
- Includes dos headers correspondentes
- Definições de variáveis estáticas (privadas ao módulo)
- Implementações de todas as funções
- Funções auxiliares como `static` (privadas)

**Exemplo:**
```cpp
#include "../include/sensores.h"
#include <DHT.h>

// Configuração privada do módulo
#define DHTPIN 10
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Variáveis globais
float temperatura = 0.0;
float umidade = 0.0;

// Funções privadas (static)
static void validarLeituras() { }

// Implementações públicas
void iniciarSensores() { }
void lerSensores() { }
```

### 3. **Variáveis Globais Controladas**

- Declaradas em **um único .cpp** (arquivo de implementação)
- Declaradas como `extern` no **.h** correspondente
- Evita duplicação e conflitos de ligação

### 4. **Documentação Doxygen**

Todos os headers incluem comentários:
```cpp
/**
 * @brief Descrição breve da função
 * @param param1 Descrição do parâmetro
 * @return Descrição do retorno
 */
```

### 5. **Include Guards**

Todos os headers usam `#pragma once` (moderno e seguro):
```cpp
#pragma once

#include <Arduino.h>
// ... resto do header
```

## 🔄 Fluxo de Compilação

1. **Arduino IDE** compila cada arquivo .cpp independentemente
2. O linker combina as implementações
3. Variáveis globais `extern` são resolvidas em tempo de link
4. **Benefício:** Cada módulo é independente e testável

## 📊 Módulos ESP32

| Módulo | Responsabilidade |
|--------|------------------|
| `persistencia` | Gravação de dados em LittleFS |
| `alertas_espec` | Análise de alertas e riscos ambientais |
| `serial_proto` | Comunicação UART com Arduino |
| `agendador_uvc` | Agendamento de ciclos UVC |
| `servidorweb` | Servidor HTTP e rotas |
| `dashboard` | Geração de HTML do dashboard |

## 📊 Módulos Arduino

| Módulo | Responsabilidade |
|--------|------------------|
| `sensores` | Leitura de DHT11, LDR, PIR |
| `bomba_nft` | Controle da válvula servo e irrigação |
| `uvc` | Controle do LED UVC |
| `led` | Feedback RGB do sistema |
| `modos` | Leitura dos DIP switches |
| `alertas_local` | Detecção de anomalias e alertas |
| `serial_proto` | Comunicação UART com ESP32 |

## ✅ Benefícios da Reorganização

- ✅ **Modularidade:** Cada módulo com responsabilidade única
- ✅ **Manutenibilidade:** Fácil localizar e modificar código
- ✅ **Reutilização:** Headers podem ser usados em outros projetos
- ✅ **Testabilidade:** Cada módulo pode ser testado independentemente
- ✅ **Escalabilidade:** Adicionar novos módulos sem afetar os existentes
- ✅ **Documentação:** Código autodocumentado com Doxygen
- ✅ **Padrão Industrial:** Segue convenções profissionais de C/C++

## 🚀 Próximos Passos

Para expandir o projeto:

1. **Novo módulo?** Crie:
   - `include/novo_modulo.h` (apenas declarações)
   - `src/novo_modulo.cpp` (implementação)

2. **Usar o módulo?**
   ```cpp
   #include "include/novo_modulo.h"
   ```

3. **Variável global?**
   ```cpp
   // Em novo_modulo.cpp:
   tipo variavel = valor;
   
   // Em novo_modulo.h:
   extern tipo variavel;
   ```

## 📝 Notas

- Os arquivos `.h` antigos ainda existem mas são **obsoletos**
- Remova-os após confirmar que tudo funciona
- Arduino IDE reconhece automaticamente a estrutura `include/src/`
