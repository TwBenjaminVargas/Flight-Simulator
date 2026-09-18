#pragma once

// Una pieza lista para dibujar: qué malla, con qué transformación y de qué color.
// Es lo único que main.cpp necesita para dibujar el avión.

#include <glm/glm.hpp>
#include "Mesh.h"

struct RenderItem {
    const Mesh* mesh = nullptr;        
    glm::mat4   model = glm::mat4(1.0f); 
    glm::vec4   color = glm::vec4(1.0f);
};