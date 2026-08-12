# Guia de Boas Práticas - Hortinha 3.0

## 🎯 Princípios Fundamentais

### 1. Separação de Responsabilidades

Cada módulo deve ter **uma única responsabilidade**:
- ❌ Ruim: Um arquivo que lê sensores, controla LED E envia dados
- ✅ Bom: Um arquivo só para sensores, outro para LED, outro para UART

### 2. Interfaces Limpas

Os headers definem o **contrato** do módulo:
- Funções públicas bem documentadas
- Variáveis globais com `extern` explícito
- Constantes de configuração claramente indicadas

### 3. Implementação Privada

Detalhes internos ficam no .cpp:
- Funções auxiliares como `static`
- Configurações de hardware (pinos, timings)
- Variáveis internas

---

## 📋 Checklist para Novo Módulo

Ao criar um novo módulo, siga este checklist:

### 1. Criar o Header (`include/meu_modulo.h`)

```cpp
#pragma once

#include <Arduino.h>

/**
 * @brief Descrição clara do módulo
 * @details Explicação mais detalhada se necessário
 */

// Tipos e estruturas
struct MeuTipo { };

// Funções públicas com documentação
void meuiniciar();
void meuProcessar();

// Variáveis globais (extern)
extern bool meuEstado;
extern int meuValor;
```

### 2. Criar a Implementação (`src/meu_modulo.cpp`)

```cpp
#include "../include/meu_modulo.h"
#include <dependency.h>  // outras dependências

// Configuração privada
#define MEU_PIN 5
static const int MEU_TIMEOUT = 1000;

// Variáveis globais (definições)
bool meuEstado = false;
int meuValor = 0;

// Variáveis internas (static - privadas)
static unsigned long ultimoTempo = 0;

// Funções privadas
static void auxiliar() {
    // implementação
}

// Funções públicas
void meuIniciar() {
    // implementação
}

void meuProcessar() {
    // implementação
}
```

### 3. Incluir no .ino

```cpp
#include "include/meu_modulo.h"

void setup() {
    meuIniciar();
}

void loop() {
    meuProcessar();
}
```

---

## 🔍 Debugging

### Verificar Símbolos Não Definidos

Se receber erro "undefined reference":
1. Verificar se a função está definida em algum .cpp
2. Verificar se o .cpp está sendo compilado
3. Verificar spelling (maiúsculas/minúsculas)

### Verificar Variáveis Globais

Se receber "multiple definition":
1. Verificar se está definido em apenas UM .cpp
2. Verificar se está como `extern` no .h
3. Não definir em múltiplos .cpp!

---

## 🔐 Regras de Ouro

| Regra | Explicação |
|-------|-----------|
| **1 definição** | Cada variável global definida em UM .cpp |
| **1 extern** | Cada variável global declarada como `extern` no .h |
| **Headers limpos** | Nada de implementação de função nos headers |
| **Includes mínimos** | Incluir apenas o necessário (evita recompilações) |
| **Static para privadas** | Funções privadas são `static` no .cpp |
| **Documentação sempre** | Adicionar Doxygen em funções públicas |

---

## 📚 Exemplo Prático: Novo Módulo para Ventilador

### 1. Header (`include/ventilador.h`)

```cpp
#pragma once

#include <Arduino.h>

/**
 * @brief Controla ventilador automático baseado em temperatura
 */

// Estados possíveis
enum VentiladorEstado {
    VENTILADOR_DESLIGADO = 0,
    VENTILADOR_LIGADO = 1
};

/**
 * @brief Inicializa o módulo ventilador
 */
void iniciarVentilador();

/**
 * @brief Atualiza o estado do ventilador
 * Deve ser chamada continuamente no loop()
 */
void atualizarVentilador();

/**
 * @brief Ativa o ventilador manualmente
 */
void ligarVentilador();

/**
 * @brief Desativa o ventilador manualmente
 */
void desligarVentilador();

// Variáveis globais
extern VentiladorEstado ventiladorEstado;
extern int ventiladorVelocidade;
```

### 2. Implementação (`src/ventilador.cpp`)

```cpp
#include "../include/ventilador.h"
#include "include/sensores.h"  // precisa da temperatura

// Configuração
#define VENTILADOR_PIN 11
#define TEMP_ATIVACAO 28
#define TEMP_DESATIVACAO 26

// Variáveis globais
VentiladorEstado ventiladorEstado = VENTILADOR_DESLIGADO;
int ventiladorVelocidade = 0;

// Variáveis internas
static unsigned long ultimoCheck = 0;
static const unsigned long CHECK_INTERVAL_MS = 5000;

void iniciarVentilador() {
    pinMode(VENTILADOR_PIN, OUTPUT);
    digitalWrite(VENTILADOR_PIN, LOW);
    Serial.println("[VENTILADOR] Iniciado");
}

static void atualizarVelocidade() {
    if (ventiladorEstado == VENTILADOR_LIGADO) {
        int velocidade = map(temperatura, TEMP_ATIVACAO, 35, 100, 255);
        velocidade = constrain(velocidade, 100, 255);
        analogWrite(VENTILADOR_PIN, velocidade);
        ventiladorVelocidade = velocidade;
    }
}

void ligarVentilador() {
    ventiladorEstado = VENTILADOR_LIGADO;
    atualizarVelocidade();
    Serial.println("[VENTILADOR] Ligado");
}

void desligarVentilador() {
    ventiladorEstado = VENTILADOR_DESLIGADO;
    digitalWrite(VENTILADOR_PIN, LOW);
    ventiladorVelocidade = 0;
    Serial.println("[VENTILADOR] Desligado");
}

void atualizarVentilador() {
    unsigned long agora = millis();
    if (agora - ultimoCheck < CHECK_INTERVAL_MS)
        return;
    
    ultimoCheck = agora;
    
    if (temperatura >= TEMP_ATIVACAO && 
        ventiladorEstado == VENTILADOR_DESLIGADO) {
        ligarVentilador();
    }
    else if (temperatura <= TEMP_DESATIVACAO && 
             ventiladorEstado == VENTILADOR_LIGADO) {
        desligarVentilador();
    }
    
    if (ventiladorEstado == VENTILADOR_LIGADO) {
        atualizarVelocidade();
    }
}
```

### 3. Usar no .ino

```cpp
#include "include/ventilador.h"

void setup() {
    // ...
    iniciarVentilador();
}

void loop() {
    // ...
    atualizarVentilador();
}
```

---

## 🎓 Recursos Úteis

### Doxygen
- `/** ... */` - Comentários de documentação
- `@brief` - Descrição breve
- `@param` - Documentar parâmetro
- `@return` - Documentar retorno
- `@details` - Explicação detalhada

### Boas Práticas Gerais
- Nomes descritivos em português ou inglês (consistente)
- Constantes em MAIÚSCULAS
- Variáveis em camelCase ou snake_case (consistente)
- Funções em camelCase
- Comentar lógica complexa

---

## ⚠️ Erros Comuns

### ❌ Erro 1: Definir em múltiplos arquivos

```cpp
// alertas.h
bool alerta = false;  // ❌ ERRADO! Será incluído em vários .cpp

// sensores.h
bool temperatura = 0;  // ❌ ERRADO! Duplicação
```

### ✅ Solução

```cpp
// alertas.h
extern bool alerta;  // ✅ Apenas declaração

// alertas.cpp
bool alerta = false;  // ✅ Definição única
```

---

### ❌ Erro 2: Implementar no header

```cpp
// sensores.h
void lerSensores() {  // ❌ ERRADO!
    temperatura = dht.readTemperature();
}
```

### ✅ Solução

```cpp
// sensores.h
void lerSensores();  // ✅ Apenas declaração

// sensores.cpp
void lerSensores() {  // ✅ Implementação aqui
    temperatura = dht.readTemperature();
}
```

---

## 📊 Métricas de Qualidade

Um bom módulo tem:
- ✅ Header < 50 linhas
- ✅ Implementação 100-300 linhas
- ✅ Máximo 10-15 funções públicas
- ✅ 100% das funções públicas documentadas
- ✅ Sem dependências circulares

---

## 🔄 Refatoração Futura

Se precisar refatorar:
1. Mantenha a interface pública (header) igual
2. Mude apenas a implementação (.cpp)
3. Se mudar o header, atualize todos os callers
4. Teste incrementalmente

---

## 📞 Suporte

Para dúvidas sobre a organização:
1. Consulte este documento
2. Veja exemplos de módulos existentes
3. Siga o padrão dos módulos atuais
