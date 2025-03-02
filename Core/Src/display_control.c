
#include "display_control.h"
#include "main.h"

DisplayState_t display_state = DISPLAY_IDLE;
uint32_t display_timer = 0;

void display_init(void) {
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Sistema Listo", Font_7x10, White);
    ssd1306_UpdateScreen();
}

/*void display_status(const char* status) {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Estado:", Font_7x10, White);
    ssd1306_SetCursor(0, 15);
    ssd1306_WriteString((char*)status, Font_7x10, White);
    display_code(rb);  // Mostrar el código actual después del estado
    ssd1306_UpdateScreen();
}

void display_code(ring_buffer_t *rb) {
    char display_buffer[16] = {0};
    uint8_t size = ring_buffer_size(rb);
    
    for(uint8_t i = 0; i < size; i++) {
        display_buffer[i] = rb->buffer[(rb->tail + i) % rb->capacity];
    }
    
    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString("Codigo:", Font_7x10, White);
    ssd1306_SetCursor(0, 45);
    ssd1306_WriteString(display_buffer, Font_7x10, White);
    ssd1306_UpdateScreen();
}*/

void display_status(const char* status) {
    ssd1306_Fill(Black);
    
    // Mostrar estado
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Estado:", Font_7x10, White);
    ssd1306_SetCursor(0, 15);
    ssd1306_WriteString((char*)status, Font_7x10, White);
    ssd1306_UpdateScreen();
    
    // Esperar un momento para que se vea el estado
    display_timer = HAL_GetTick();
    display_state = DISPLAY_WAIT;
    
    // Limpiar pantalla y dejarla lista para nuevo código
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
    ssd1306_UpdateScreen();

}
void display_code(ring_buffer_t *rb) {
        // Primero, limpiar SOLO el área donde se muestra el código
        ssd1306_FillRectangle(0, 20, 128, 64, Black);  // Limpia desde y=20 hasta el final
        
        // Preparar buffer para mostrar el código actual
        char display_buffer[16] = {0};
        uint8_t size = ring_buffer_size(rb);
        
        if(size > 0) {
            for(uint8_t i = 0; i < size && i < 15; i++) {
                display_buffer[i] = rb->buffer[i];
            }
            
            // Mostrar el código nuevo
            ssd1306_SetCursor(0, 20);
            ssd1306_WriteString(display_buffer, Font_7x10, White);
        }
        
        ssd1306_UpdateScreen();
    }

// Función para actualizar el estado del display
/*void display_update(void) {
    switch(display_state) {
        case DISPLAY_WAIT:
            if((HAL_GetTick() - display_timer) >= 5000) {
                // Han pasado 1000ms, limpiar y volver a IDLE
                ssd1306_Fill(Black);
                ssd1306_SetCursor(0, 0);
                ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
                ssd1306_UpdateScreen();
                display_state = DISPLAY_IDLE;
            }
            break;
            
        case DISPLAY_IDLE:
        default:
            // No hacer nada en estado idle
            break;
    }
}*/
/*void display_update(void) {
    switch(display_state) {
        case DISPLAY_WAIT:
            // Añadir mensaje de depuración por UART
            uint32_t elapsed = HAL_GetTick() - display_timer;
            char debug_msg[50];
            sprintf(debug_msg, "Tiempo transcurrido: %lu ms\r\n", elapsed);
            HAL_UART_Transmit(&huart2, (uint8_t*)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
            
            if((HAL_GetTick() - display_timer) >= 10000) {
                // Han pasado 10 segundos, limpiar y volver a IDLE
                HAL_UART_Transmit(&huart2, (uint8_t*)"Cambiando a IDLE\r\n", 18, HAL_MAX_DELAY);
                ssd1306_Fill(Black);
                ssd1306_SetCursor(0, 0);
                ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
                ssd1306_UpdateScreen();
                display_state = DISPLAY_IDLE;
            }
            break;
            
        // ...
    }
}*/
    
void display_clear_code(void) {
    // Limpiar solo el área del código
    ssd1306_FillRectangle(0, 45, 128, 63, Black);  // Ajusta las coordenadas según tu pantalla
    ssd1306_UpdateScreen();
}

void display_command_result(const char* result, ring_buffer_t *rb) {
    // Mostrar resultado del comando y mantener visible el código actual
    ssd1306_Fill(Black);
    
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Resultado:", Font_7x10, White);
    ssd1306_SetCursor(0, 15);
    ssd1306_WriteString((char*)result, Font_7x10, White);
    
    // Mostrar código actual
    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString("Codigo:", Font_7x10, White);
    display_code(rb);
    
    ssd1306_UpdateScreen();
}

void display_error(const char* error_msg) {
    ssd1306_Fill(Black);
    
    // Mostrar mensaje de error
    ssd1306_SetCursor(0, 0);
    //ssd1306_WriteString("ERROR:", Font_7x10, White);
    ssd1306_SetCursor(0, 15);
    ssd1306_WriteString((char*)error_msg, Font_7x10, White);
    ssd1306_UpdateScreen();
    
    // Esperar un momento para que se vea el error
    HAL_Delay(1000);
    
    // Limpiar pantalla y dejarla lista para nuevo código
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
    ssd1306_UpdateScreen();
}

void display_door_status(const char* status) {
    ssd1306_Fill(Black);
    
    // Mostrar estado
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Estado:", Font_7x10, White);
    ssd1306_SetCursor(0, 15);
    ssd1306_WriteString((char*)status, Font_7x10, White);
    ssd1306_UpdateScreen();
    
    // Configurar para un tiempo más largo (10 segundos)
    display_timer = HAL_GetTick();
    display_state = DISPLAY_DOOR_WAIT;  // Usar el nuevo estado específico
}

// Modificar la función display_update
void display_update(void) {
    switch(display_state) {
        case DISPLAY_WAIT:
            if((HAL_GetTick() - display_timer) >= 2000) {
                // Han pasado 2 segundos (mensajes normales)
                ssd1306_Fill(Black);
                ssd1306_SetCursor(0, 0);
                ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
                ssd1306_UpdateScreen();
                display_state = DISPLAY_IDLE;
            }
            break;
            
        case DISPLAY_DOOR_WAIT:
            if((HAL_GetTick() - display_timer) >= 2000) {
                ssd1306_Fill(Black);
                ssd1306_SetCursor(0, 0);
                ssd1306_WriteString("Ingrese codigo:", Font_7x10, White);
                ssd1306_UpdateScreen();
                display_state = DISPLAY_IDLE;
            }
            break;
            
        case DISPLAY_IDLE:
        default:
            // No hacer nada en estado idle
            break;
    }
}