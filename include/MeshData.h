#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Tupla de atributos para un vértice en formato intercalado.
 */
struct Vertex {
    glm::vec3 position;   // Posición (X, Y, Z)
    glm::vec3 normal;     // Normal a la superficie (apunta hacia afuera)
    glm::vec2 tex_coords; // Coordenadas de textura (U, V), rango [0, 1]
};

/**
 * @brief Datos de la malla en el lado de la CPU.
 */
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; // Vacío si la malla no es indexada
};