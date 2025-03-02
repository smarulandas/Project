#include <stdint.h>
#include <stdbool.h>
#include <ring_buffer.h>
#include "main.h"
// keypad_control.h
#ifndef KEYPAD_CONTROL_H
#define KEYPAD_CONTROL_H


// Declaraciones de funciones
void check_keypad(ring_buffer_t *rb);
bool check_hash_star(ring_buffer_t *rb, uint8_t pos1, uint8_t pos2);
void control_action(uint8_t key, ring_buffer_t *rb);
bool is_valid_control(uint8_t key);
bool process_command(ring_buffer_t *rb);

// Variables externas
extern uint32_t key_pressed_tick;
extern uint16_t column_pressed;
extern uint32_t debounce_tick;
extern uint8_t host_byte_received;
extern uint8_t internet_byte_received;

void init_door_state(void);  // Agregar declaración de la función



#endif // KEYPAD_CONTROL_H