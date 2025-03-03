
#include "keypad_control.h"
#include "main.h"
#include "ring_buffer.h"
#include "keypad.h"
#include <stdbool.h>
#include "display_control.h"

// Variables globales para el manejo del teclado y control
uint32_t key_pressed_tick = 0;      // Timestamp de la última tecla presionada (para control de tiempos)
uint16_t column_pressed = 0;        // Almacena la columna del teclado que fue presionada
uint32_t debounce_tick = 0;         // Contador para el tiempo de debounce (anti-rebote)
uint8_t host_byte_received;         // Byte recibido del host
uint8_t internet_byte_received;     // Byte recibido de internet
static bool led_state = false;      // Estado inicial: puerta cerrada (false=cerrada, true=abierta)

/**
 * @brief Inicializa el estado de la puerta al arranque del sistema
 * 
 * Configura el estado inicial de la puerta como "cerrada":
 * - LED verde apagado
 * - LED rojo encendido
 * - Establece variable de estado
 * - Envía un mensaje por UART informando el estado inicial
 */
void init_door_state(void) {
    // Inicializar estado: puerta cerrada (LED rojo ON, verde OFF)
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);    // Apaga LED verde
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);          // Enciende LED rojo
    led_state = false;                                                        // Actualiza variable de estado
    HAL_UART_Transmit(&huart2, (uint8_t*)" Estado inicial: Puerta cerrada\r\n", 33, HAL_MAX_DELAY);  // Envía mensaje de estado
    move_servo(500); // Posición inicial puerta cerrada (0grados)
}

/* Versión anterior de la función check_keypad (comentada para referencia)
void check_keypad(ring_buffer_t *rb) {
    if (column_pressed != 0 && (key_pressed_tick + 5) < HAL_GetTick()) {
        uint8_t key = keypad_scan(column_pressed);
        
        if (!ring_buffer_is_full(rb)) {
            HAL_UART_Transmit(&huart2, &key, 1, 100);
            ring_buffer_write(rb, key);
        }
        
        column_pressed = 0;
        key_pressed_tick = HAL_GetTick();
    }
}*/

/**
 * @brief Verifica si hay teclas presionadas y las procesa
 * @param rb Puntero al buffer circular donde se almacenan las teclas
 * 
 * Esta función:
 * 1. Verifica si hay una columna presionada y si pasó el tiempo de debounce
 * 2. Obtiene la tecla presionada del teclado
 * 3. Maneja la tecla 'B' como caso especial para limpiar el buffer
 * 4. Para otras teclas válidas, las almacena en el buffer y actualiza el display
 */
void check_keypad(ring_buffer_t *rb) {
    // Verifica si hay una columna presionada y ha pasado el tiempo de debounce (5ms)
    if (column_pressed != 0 && (key_pressed_tick + 5) < HAL_GetTick()) {
        uint8_t key = keypad_scan(column_pressed);  // Obtiene la tecla de la columna presionada
        
        if(key == 'B') {
            // La tecla 'B' es un caso especial para limpiar el buffer
            control_action(key, rb);
        }
        else if (!ring_buffer_is_full(rb)) {
            // Verifica que la tecla sea un carácter imprimible (ASCII 32-126)
            if(key >= 32 && key <= 126) {
                    HAL_UART_Transmit(&huart2, &key, 1, 100);     // Envía la tecla por UART
                    ring_buffer_write(rb, key);                   // Almacena la tecla en el buffer
                    display_code(rb);                             // Actualiza el display con el código actual
                
            }
        }
        
        // Reinicia variables para la próxima tecla
        column_pressed = 0;
        key_pressed_tick = HAL_GetTick();
    }
}

/**
 * @brief Verifica si hay una secuencia "#*" o "*#" en las posiciones especificadas
 * @param rb Puntero al buffer circular
 * @param pos1 Primera posición a verificar
 * @param pos2 Segunda posición a verificar
 * @return true si se encuentra la secuencia, false en caso contrario
 * 
 * Verifica las secuencias que delimitan los comandos válidos:
 * - "#*" al inicio del comando (posiciones 0,1)
 * - "*#" al final del comando (posiciones 3,4)
 */
bool check_hash_star(ring_buffer_t *rb, uint8_t pos1, uint8_t pos2) {
    // Verifica que las posiciones sean válidas dentro del buffer
    if (pos1 >= rb->capacity || pos2 >= rb->capacity) {
        return false;
    }
    
    // Verifica secuencia "#*" al inicio (posiciones 0,1)
    if(pos1 == 0 && pos2 == 1) {
        return (rb->buffer[pos1] == '#' && rb->buffer[pos2] == '*');
    }
    // Verifica secuencia "*#" al final (posiciones 3,4)
    else if(pos1 == 3 && pos2 == 4) {
        return (rb->buffer[pos1] == '*' && rb->buffer[pos2] == '#');
    }
    
    return false;
}

/**
 * @brief Ejecuta acciones de control basadas en la tecla presionada
 * @param key Tecla de control presionada
 * @param rb Puntero al buffer circular
 * 
 * Implementa las siguientes acciones:
 * - 'B': Limpia el buffer
 * - 'A': Abre la puerta (LED verde ON, rojo OFF)
 * - 'C': Cierra la puerta (LED rojo ON, verde OFF)
 * - '0': Limpia el buffer
 * - '1': Muestra el estado actual de la puerta
 */
void control_action(uint8_t key, ring_buffer_t *rb) {
    // NOTA: Esta variable estática oculta la variable global led_state, lo que podría causar problemas
    static bool led_state = false;
        
    // Caso especial para la tecla 'B' (limpia el buffer)
    if(key == 'B') {
        ring_buffer_reset(rb);
        display_door_status("Buffer limpio");
        return;  // No llamar a display_code aquí
    }
    
    // Acciones para otras teclas de control
    switch(key) {
        case 'A':  // Abrir puerta
            move_servo(500); // Posición para abrir la puerta (0grados)
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);    // Apaga LED rojo
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);  // Enciende LED verde
            led_state = true;
            display_door_status("Puerta Abierta");
            break;
            
        case 'C':  // Cerrar puerta
            move_servo(2000); // Posición para cerrar la puerta (180 grados)
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);  // Apaga LED verde
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);        // Enciende LED rojo
            led_state = false;
            display_door_status("Puerta Cerrada");
            break;
            
        case '0':  // Limpiar buffer
            ring_buffer_reset(rb);
            display_door_status("Buffer Limpio");
            break;
            
        case '1':  // Mostrar estado actual
            if(led_state) {
                display_door_status("Puerta Abierta");
            } else {
                display_door_status("Puerta Cerrada");
            }
            break;
            
        default:  // Tecla no reconocida
            display_status("Error");
            break;
    }
}

/**
 * @brief Verifica si una tecla es válida como comando de control
 * @param key Tecla a verificar
 * @return true si es una tecla de control válida, false en caso contrario
 * 
 * Las teclas de control válidas son: 'A', 'C', '0', '1'
 */
bool is_valid_control(uint8_t key) {
    return (key == 'A' || key == 'C' || key == '0' || key == '1');
}

/**
 * @brief Procesa comandos cuando el buffer está lleno
 * @param rb Puntero al buffer circular
 * @return true si se procesó un comando, false en caso contrario
 * 
 * Verifica si el buffer contiene un comando válido con el formato:
 * "#*[tecla_control]*#" donde [tecla_control] debe ser una de las teclas válidas
 */
bool process_command(ring_buffer_t *rb) {
    // Verifica si el buffer está lleno
    if (!ring_buffer_is_full(rb)) {
        return false;
    }
    
    // Obtiene la tecla de control (posición 2 del buffer)
    uint8_t tecla_control = rb->buffer[2];
    
    // Verifica la estructura del comando:
    // - Debe comenzar con "#*"
    // - Debe terminar con "*#"
    // - La tecla de control debe ser válida
    bool inicio_correcto = check_hash_star(rb, 0, 1);
    bool final_correcto = check_hash_star(rb, 3, 4);
    bool tecla_correcta = is_valid_control(tecla_control);
    
    if (inicio_correcto && final_correcto && tecla_correcta) {
        // Si el comando es válido, ejecuta la acción correspondiente
        control_action(tecla_control, rb);
    } else {
        // Si no es válido, muestra mensajes de error
        display_error("Error");  // Mensaje en la pantalla
        HAL_UART_Transmit(&huart2, (uint8_t*)" Comando no válido\r\n", 21, HAL_MAX_DELAY);  // Mensaje por UART
    }
    
    // Limpia el buffer después de procesar
    ring_buffer_reset(rb);
    return true;
}