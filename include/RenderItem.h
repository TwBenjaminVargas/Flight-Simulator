#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"

/**
 * @brief Representa una unidad individual de renderizado lista para ser dibujada.
 * 
 * Empaqueta toda la información requerida por la tubería de renderizado para dibujar
 * una pieza del modelo: la geometría (malla), su transformación en el espacio del
 * mundo y su material/color base.
 */
struct RenderItem {
    /**
     * @brief Puntero no propietario a la malla 3D en VRAM.
     * 
     * Permite reutilizar la misma geometría (ej. un cubo unitario) entre múltiples
     * piezas sin duplicar memoria en la GPU.
     */
    const Mesh* mesh = nullptr;        

    /**
     * @brief Matriz de transformación del modelo (Model Matrix).
     * 
     * Transforma los vértices locales de la malla a sus coordenadas finales en el mundo
     * (Model = Pose * Local).
     */
    glm::mat4 model = glm::mat4(1.0f); 

    /**
     * @brief Color RGBA asignado a esta pieza específica.
     * 
     * Se envía al shader fragment mediante un uniform (ej. `uColor`) para distinguir
     * visualmente los componentes de la nave.
     */
    glm::vec4 color = glm::vec4(1.0f);
};