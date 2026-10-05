#pragma once

// Dueño de las mallas y matrices del avión.
// init()    : carga mallas en GPU y calcula matrices locales (una vez)
// update()  : recalcula pose_ con la posición y orientación actuales (cada cuadro)
// collect() : combina pose_ * local y llena la lista de RenderItem (cada cuadro)

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "RenderItem.h"

class Aircraft {
public:
    void init();
    void update(glm::vec3 pos, glm::vec3 angulos);
    void collect(std::vector<RenderItem>& items) const;

private:
    // Mallas 
    Mesh fuselaje_;   // cilindro
    Mesh cono_nariz_; // cono
    Mesh cono_cola_;  // cono
    Mesh cubo_;       // cubo unitario, se reutiliza para ala y empenajes

    std::vector<glm::mat4>    locales_;
    std::vector<const Mesh*>  meshes_;
    std::vector<glm::vec4>    colores_;

    glm::mat4 pose_ {1.0f};
};