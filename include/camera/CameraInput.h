#pragma once

#include "CameraCommand.h"

struct GLFWwindow;

/**
 * @brief Modulo encargado del procesamiento de entradas del usuario para el control de la camara.
 * 
 * Captura las interacciones continuas del mouse mediante una estrategia de polling (sondeo por frame)
 * y eventos discretos de scroll. Transforma las coordenadas absolutas de pantalla y los desplazamientos 
 * en variaciones esféricas para empaquetarlas en un CameraCommand.
 */
class CameraInput {
public:
    /**
     * @brief Sondea el estado actual de los periféricos y genera un comando de movimiento.
     * 
     * Consulta la posición actual del cursor y los botones activos mediante GLFW.
     * Calcula la diferencia de desplazamiento (delta) respecto al cuadro anterior, aplica
     * ganancias de sensibilidad y consume el buffer del acumulador de scroll.
     * 
     * @param window Puntero a la ventana de GLFW activa.
     * @return CameraCommand Estructura con las deltas angulares y de distancia procesadas para el frame actual.
     */
    CameraCommand poll(GLFWwindow* window);

    /**
     * @brief Callback estático suscripto a eventos de la rueda del mouse en GLFW.
     * 
     * Acumula de forma continua los saltos de scroll generados entre cuadros.
     * Se registra en GLFW utilizando glfwSetScrollCallback().
     * 
     * @param window Puntero a la ventana origen del evento.
     * @param xoffset Desplazamiento horizontal de la rueda (usualmente 0).
     * @param yoffset Desplazamiento vertical de la rueda (+1 al acercar / -1 al alejar).
     */
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    double prev_x_ = 0.0;  ///< Coordenada X del cursor capturada en el cuadro inmediatamente anterior.
    double prev_y_ = 0.0;  ///< Coordenada Y del cursor capturada en el cuadro inmediatamente anterior.
    
    /**
     * @brief Flag de inicialización para evitar saltos bruscos en el primer frame.
     * 
     * Garantiza que en el primer cuadro no se procese una diferencia respecto a (0,0).
     */
    bool tiene_previa_ = false;

    /**
     * @brief Acumulador del desplazamiento de la rueda del mouse entre cuadros de renderizado.
     * 
     * Declarado estático para poder ser modificado desde la función callback global/estática.
     */
    static inline float scroll_offset_ = 0.0f;
};