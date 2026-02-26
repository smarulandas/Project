# Sistema de Control de Acceso Inteligente
**Autores:**
- Felipe Idárraga Quintero
- Samuel Marulanda Salazar

## Introducción

Este proyecto implementa un sistema de control de acceso inteligente basado en STM32, diseñado para gestionar el acceso a una casa mediante múltiples interfaces de entrada y control físico mediante servomotor.

El sistema combina hardware y software para proporcionar una solución completa de control de acceso con las siguientes características:

- **Interfaz local** mediante teclado matricial para ingreso de comandos
- **Control remoto** a través de UART (terminal) y WiFi (módulo ESP01)
- **Visualización** de estado y comandos en pantalla OLED SSD1306
- **Indicadores LED** para estados de puerta y actividad del sistema
- **Control físico** mediante servomotor para apertura/cierre de puerta
- **Formato de comandos** estandarizado (#*A*# para abrir, #*C*# para cerrar, etc.)
- **Diseño modular** con componentes bien definidos y documentados

Esta solución está diseñada para ser altamente configurable, expandible y fácil de mantener, con una arquitectura clara y documentación detallada de todos sus componentes.

## Documentación

Para ayudar a entender y modificar el sistema, se incluyen varios diagramas que explican su funcionamiento a diferentes niveles:

1. [Diagrama de Contexto](./documentación/diagrama-contexto.md): Visión general del sistema y sus interacciones externas
2. [Diagrama de Contenedor](./documentación/diagrama-contenedor.md): Componentes físicos y sus conexiones
3. [Diagrama de Componentes](./documentación/diagrama-componentes.md): Estructura interna del software
4. [Diagrama de Bloques Software](./documentación/diagrama-bloques-software.md): Flujo de ejecución principal
5. [Diagrama de Flujo](./documentación/diagrama-flujo.md): Implementación detallada del código

