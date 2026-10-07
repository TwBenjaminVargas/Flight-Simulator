#pragma once

#include <glm/glm.hpp>

/**
 * @brief Estructura de salida que empaqueta las matrices de transformación de la cámara.
 * 
 * Funciona como un objeto de transferencia de datos (DTO) que entrega al bucle principal
 * o a los shaders la información necesaria para transformar los vértices desde el espacio
 * de mundo (*World Space*) hasta el espacio de recorte (*Clip Space*).
 */
struct CameraData {
    /**
     * @brief Matriz de vista (\f$M_{view}\f$).
     * 
     * Transforma las coordenadas de los objetos desde el espacio de mundo al espacio
     * de la cámara (*View/Eye Space*). Se calcula típicamente mediante `glm::lookAt`.
     */
    glm::mat4 view = glm::mat4(1.0f);

    /**
     * @brief Matriz de proyección (\f$M_{projection}\f$).
     * 
     * Transforma las coordenadas del espacio de la cámara al espacio de corte de volumen
     * (*Clip Space*), aplicando el frustum de proyección perspectiva (efecto de escorzo)
     * y corrigiendo la relación de aspecto del framebuffer. Se calcula mediante `glm::perspective`.
     */
    glm::mat4 projection = glm::mat4(1.0f);
};