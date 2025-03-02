
#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

#include "ssd1306.h"
#include "ring_buffer.h"
#include "ssd1306_fonts.h"

typedef enum {
    DISPLAY_IDLE,
    DISPLAY_STATUS,
    DISPLAY_ERROR,
    DISPLAY_WAIT,
    DISPLAY_DOOR_WAIT
} DisplayState_t;


extern DisplayState_t display_state;
extern uint32_t display_timer;
void display_init();
void display_status(const char* status);
void display_code(ring_buffer_t *rb);
void display_clear_code();
void display_command_result(const char* result, ring_buffer_t *rb);
void display_error(const char* error);
void display_update();
void display_door_status();

#endif