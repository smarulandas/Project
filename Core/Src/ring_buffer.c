
#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rb, uint8_t *mem_add, uint8_t capacity){

    // Asignar la dirección de memoria al buffer
    rb->buffer = mem_add;

    // Inicializar los campos de la estructura
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = 0;
    rb->capacity = capacity;
}

void ring_buffer_reset(ring_buffer_t *rb){
   
    // Reiniciar los punteros al principio del buffer
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = 0;
}

uint8_t ring_buffer_size(ring_buffer_t *rb) {
    uint8_t size =0;
    // Si el buffer está lleno, retornar la capacidad
    if (rb->is_full) {
        size= rb->capacity;
    }
    
    // Si head >= tail, la diferencia es el número de elementos
    if (rb->head >= rb->tail) {
        size= rb->head - rb->tail;
    }
    
    // Si head < tail, debemos considerar el wrap-around
    size= rb->capacity - (rb->tail - rb->head);
    return size;
}

uint8_t ring_buffer_is_full(ring_buffer_t *rb) {
    return rb->is_full;
}

uint8_t ring_buffer_is_empty(ring_buffer_t *rb) {
    return (!rb->is_full && (rb->head == rb->tail));
}

void ring_buffer_write(ring_buffer_t *rb, uint8_t data) {
   
    // Escribir el dato en la posición head
    rb->buffer[rb->head] = data;
    
    // Actualizar head
    rb->head = (rb->head + 1) % rb->capacity;
    
    // Si head alcanza a tail, el buffer está lleno
    if (rb->head == rb->tail) {
        rb->is_full = 1;
    }
}

uint8_t ring_buffer_read(ring_buffer_t *rb, uint8_t *byte) {
   
    // Verificar si el buffer está vacío usando la función existente
    if (ring_buffer_is_empty(rb)) {
        return 0;  // Buffer vacío
    }
    
    // Leer el dato y guardarlo en la dirección proporcionada
    *byte = rb->buffer[rb->tail];
    
    // Actualizar tail
    rb->tail = (rb->tail + 1) % rb->capacity;
    
    // El buffer ya no está lleno después de leer
    rb->is_full = 0;
    
    return 1;  // Lectura exitosa
}