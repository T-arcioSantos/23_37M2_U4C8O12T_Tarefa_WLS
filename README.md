# Controle de Display OLED e LEDs PWM com Joystick no Raspberry Pi Pico

Este projeto demonstra a interação entre um joystick analógico, display OLED e LEDs RGB utilizando o Raspberry Pi Pico. O joystick controla a posição de um quadrado no display e a intensidade dos LEDs PWM, criando uma experiência interativa.

## 📋 Funcionalidades

**Controle do Display OLED:**

*   Exibe um retângulo fixo na borda do display
*   Mostra um quadrado móvel controlado pelo joystick
*   Permite alternar entre diferentes modos de visualização
*   Interface I2C para comunicação com o display SSD1306

**Controle dos LEDs PWM:**

*   LED Vermelho controlado pelo eixo X do joystick
*   LED Azul controlado pelo eixo Y do joystick
*   LED Verde indicador do modo de visualização
*   Controle de intensidade proporcional ao movimento do joystick

**Interação por Botões:**

*   Botão do Joystick: Alterna o modo de visualização do retângulo
*   Botão A: Liga/Desliga os LEDs PWM

## 🛠 Hardware

| Componente     | GPIO / Pino | Observações                |
| -------------- | ----------- | -------------------------- |
| Display OLED   | GPIO 14 (SDA) | Comunicação I2C            |
|                | GPIO 15 (SCL) | Comunicação I2C            |
| Joystick       | GPIO 26     | Eixo X (ADC)               |
|                | GPIO 27     | Eixo Y (ADC)               |
|                | GPIO 22     | Botão                     |
| LED Verde      | GPIO 11     | LED indicador             |
| LED Azul       | GPIO 12     | Controle PWM               |
| LED Vermelho   | GPIO 13     | Controle PWM               |
| Botão A        | GPIO 5      | Controle dos LEDs PWM     |

## 💻 Código

O código principal está em `23_37M2_U4C8O12T_Tarefa_WLS.c`.

**Fluxo de Execução:**

**Inicialização:**

*   Configura os pinos GPIO para LEDs e botões
*   Inicializa o módulo I2C para o display OLED
*   Configura os canais PWM para os LEDs
*   Inicializa o ADC para leitura do joystick

**Loop Principal:**

*   Lê os valores analógicos do joystick
*   Atualiza a posição do quadrado no display
*   Controla a intensidade dos LEDs PWM
*   Atualiza o display OLED

**Tratamento de Interrupções:**

*   Botão do Joystick: Alterna o modo de visualização
*   Botão A: Liga/Desliga os LEDs PWM

## Manual do Usuário - Controle Interativo com Joystick

## 🎮 Como Usar

### Controles Básicos

**Joystick**

*   Mova o joystick para controlar o quadrado no display OLED.
*   Movimento horizontal (eixo X): controla a intensidade do LED Vermelho.
*   Movimento vertical (eixo Y): controla a intensidade do LED Azul.

**Botões**

*   **Botão do Joystick (pressionar):**
    *   Liga/Desliga o retângulo interno no display.
    *   O LED Verde acende quando o retângulo interno está ativo.
*   **Botão A:**
    *   Liga/Desliga os LEDs PWM (Vermelho e Azul).

### Display OLED

*   Mostra um retângulo fixo na borda.
*   Um quadrado menor se move conforme a posição do joystick.
*   O retângulo interno pode ser ativado/desativado.

### LEDs

*   **LED Vermelho:** Intensidade controlada pelo movimento horizontal do joystick.
*   **LED Azul:** Intensidade controlada pelo movimento vertical do joystick.
*   **LED Verde:** Indica quando o retângulo interno está ativo.

## ⚠️ Observações

*   O joystick possui uma pequena "zona morta" no centro para evitar flutuações.
*   Há um debounce de 200ms nos botões para evitar acionamentos falsos.
*   O display é atualizado a cada 100ms.

## 🔧 Solução de Problemas

**LEDs não acendem:**

*   Verifique se os LEDs PWM não foram desativados pelo Botão A.
*   Mova o joystick para longe do centro.

**Display não responde:**

*   Verifique se o joystick está funcionando corretamente.
*   Reinicie o sistema se necessário.

## 📚 Bibliotecas

*   `stdio.h`: Funções padrão de entrada/saída
*   `pico/stdlib.h`: Funções do SDK do Raspberry Pi Pico
*   `hardware/adc.h`: Controle do ADC
*   `hardware/i2c.h`: Comunicação I2C
*   `hardware/pwm.h`: Controle PWM
*   `ssd1306.h`: Controle do display OLED
*   `font.h`: Fontes para o display OLED

## 👥 Desenvolvedor

Tárcio Santos

## 🎥 Demonstração

https://youtu.be/NA9fgLT5UKs?si=QqqMBy_RReGzOr1w