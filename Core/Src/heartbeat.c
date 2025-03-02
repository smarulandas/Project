
#include "heartbeat.h"
#include  "main.h"

// Variables privadas para el control del tiempo
static uint32_t last_heartbeat_tick = 0;
static const uint32_t HEARTBEAT_PERIOD = 500; // Medio segundo
static uint8_t led_state = 0;

void heartbeat_init(void) {
    // Inicializar el estado del LED
    HAL_GPIO_WritePin(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin, GPIO_PIN_RESET);
    last_heartbeat_tick = HAL_GetTick();
}

//Programa cuando ya esta corriendo el programa, aqui es donde va a almacenar el tiempo
void heartbeat_update(void) {
    uint32_t current_tick = HAL_GetTick();
    
    // Verificar si ha pasado el período
    if((current_tick - last_heartbeat_tick) >= HEARTBEAT_PERIOD) {
        // Togglear el LED
        led_state = !led_state; //Niega el estado del LED acutual
        HAL_GPIO_WritePin(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin, led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        // Actualizar el último tick
        last_heartbeat_tick = current_tick;
    }
}