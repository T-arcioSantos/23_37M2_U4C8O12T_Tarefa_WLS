#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM
#include "lib/ssd1306.h"
#include "lib/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14 
#define I2C_SCL 15 
#define endereco 0x3C // Endereço do display
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick


static volatile bool retanguloMenor_ativo = false;
bool cor = true; // Variável para alternar a cor do pixel
bool EstadoLedPWM = true; // Variável para alternar o estado dos leds PWM

static volatile uint last_time = 0;
ssd1306_t ssd; // Inicializa a estrutura do display

const uint button_A = 5; // Botão A
const uint ledGreen_pin = 11; // Pino do LED Verde
const uint PWMledBlue_pin = 12; // Pino do LED Azul
const uint PWMledRed_pin = 13; // Pino do LED Vermelho

//Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"


void gpio_irq_handler(uint gpio, uint32_t events); // Protótipo da função de tratamento de interrupção
int main(){
    stdio_init_all();

    //Configuração dos LEDS PWM
    gpio_set_function(PWMledBlue_pin, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
    gpio_set_function(PWMledRed_pin, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
    uint sliceBlue = pwm_gpio_to_slice_num(PWMledBlue_pin); //obter o canal PWM da GPIO
    uint sliceRed = pwm_gpio_to_slice_num(PWMledRed_pin); //obter o canal PWM da GPIO
    pwm_set_clkdiv(sliceBlue, 125.0f); //define o divisor de clock do PWM
    pwm_set_wrap(sliceBlue, 255); //definir o valor de wrap
    pwm_set_enabled(sliceBlue, true); //habilita o pwm no slice correspondente
    pwm_set_clkdiv(sliceRed, 125.0f); //define o divisor de clock do PWM
    pwm_set_wrap(sliceRed, 255); //definir o valor de wrap
    pwm_set_enabled(sliceRed, true); //habilita o pwm no slice correspondente

    //Configuração do LED Verde
    gpio_init(ledGreen_pin);
    gpio_set_dir(ledGreen_pin, GPIO_OUT);

    //Configuração dos botões
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);
    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB); 

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa o ADC e configura os pinos do Joystick
    adc_init();// Inicializa o ADC
    adc_gpio_init(JOYSTICK_X_PIN);// Inicializa o pino do eixo X
    adc_gpio_init(JOYSTICK_Y_PIN);  // Inicializa o pino do eixo Y
    
    uint16_t adc_value_x;// Variável para armazenar o valor do ADC do eixo X
    uint16_t adc_value_y; // Variável para armazenar o valor do ADC do eixo Y
    
    // Define o valor central e a zona morta (zona_Desligamento)
    const uint16_t centro = 2048; // centro do Joystick
    const uint16_t zona_Desligamento = 50; // Ajuste esse valor conforme necessário
    
    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Habilita a interrupção do botão do Joystick
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Habilita a interrupção do botão A

    while (true){
        adc_select_input(0); // Seleciona o ADC para eixo Y. O pino 26 como entrada analógica
        adc_value_y = adc_read();
        adc_select_input(1); // Seleciona o ADC para eixo X. O pino 27 como entrada analógica
        adc_value_x = adc_read();    
       
        // Calcula a diferença em relação ao centro para cada eixo
        uint16_t distancia_x = (adc_value_x > centro) ? (adc_value_x - centro) : (centro - adc_value_x);
        uint16_t distancia_y = (adc_value_y > centro) ? (adc_value_y - centro) : (centro - adc_value_y);

        // Inicializa os níveis de PWM como 0...LED apagado
        uint16_t brilho_red = 0;
        uint16_t brilho_blue = 0;

        // Se a diferença ultrapassar a zona de desligamento, mapeia para 0 a 255
        if (distancia_x > zona_Desligamento) {
            brilho_red = ((distancia_x - zona_Desligamento) * 255) / (centro - zona_Desligamento);
            if (brilho_red > 255)
                brilho_red = 255;
        }
        if (distancia_y > zona_Desligamento) {
            brilho_blue = ((distancia_y - zona_Desligamento) * 255) / (centro - zona_Desligamento);
            if (brilho_blue > 255)
                brilho_blue = 255;
        }

        // Atualiza o PWM dos LEDs
        pwm_set_gpio_level(PWMledRed_pin, brilho_red);
        pwm_set_gpio_level(PWMledBlue_pin, brilho_blue);
        
        // Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo

        if(retanguloMenor_ativo){
            ssd1306_rect(&ssd, 4, 4, 120, 58, cor, !cor); // Desenha um retângulo menor duplicando a borda
        }
        // Calcula a posição do quadrado no display
        uint8_t pos_X = (adc_value_x *(128-8))/ 4095;
        uint8_t pos_Y = ((4095 - adc_value_y) *(64-8))/ 4095;

        // Desenha o quadrado na posição calculada
        ssd1306_rect(&ssd, pos_Y, pos_X, 8, 8, cor, cor);
        ssd1306_send_data(&ssd); // Atualiza o display
       
        sleep_ms(100);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time()); 
    
    if (current_time - last_time > 200000) { // Debounce 
        last_time = current_time; 

        if(gpio == JOYSTICK_PB){   
            retanguloMenor_ativo = !retanguloMenor_ativo;// Inverte o estado da variável
            gpio_put(ledGreen_pin, retanguloMenor_ativo); // Atualiza o LED
        }
        if(gpio == button_A)
        {
            //Modificar estado dos leds PWM
            EstadoLedPWM = !EstadoLedPWM;
            pwm_set_enabled(pwm_gpio_to_slice_num(PWMledRed_pin), EstadoLedPWM);
            pwm_set_enabled(pwm_gpio_to_slice_num(PWMledBlue_pin), EstadoLedPWM);
            
        }
    }
}