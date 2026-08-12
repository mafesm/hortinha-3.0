#pragma once

#include <Arduino.h>

/**
 * @struct led
 * @brief Estrutura com dados de um LED
 */
struct led
{
    int luz;             ///< Valor de iluminação (0-255)
    int pin;             ///< Pino Arduino
    bool pisc;           ///< Flag de piscagem
    unsigned long ult_p; ///< Último tempo de piscagem
};

/**
 * @struct ledRGB
 * @brief Estrutura com dados de um LED RGB
 */
struct ledRGB
{
    led R;   ///< Canal vermelho
    led G;   ///< Canal verde
    led B;   ///< Canal azul
    int est; ///< Estado
};

/**
 * @brief Inicializa o LED RGB nos pinos especificados
 * @param pinR Pino do canal vermelho
 * @param pinG Pino do canal verde
 * @param pinB Pino do canal azul
 */
void STAR(int pinR, int pinG, int pinB);

/**
 * @brief Define a cor do LED RGB
 * @param r Valor do canal vermelho (0-255)
 * @param g Valor do canal verde (0-255)
 * @param b Valor do canal azul (0-255)
 */
void COR(int r, int g, int b);

/**
 * @brief Faz o LED RGB piscar uma cor específica
 * @param r Valor do canal vermelho
 * @param g Valor do canal verde
 * @param b Valor do canal azul
 * @param vezes Número de repetições (padrão 3)
 */
void PISCA_COR(int r, int g, int b, int vezes = 3);

/**
 * @brief Atualiza o estado do LED baseado no sistema
 * Deve ser chamada continuamente no loop()
 */
void GEN_RGB();
