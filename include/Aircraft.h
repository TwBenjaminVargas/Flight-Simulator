#pragma once

// Dueño de las mallas y matrices del avión.
// init() : carga mallas en GPU y calcula matrices locales (una vez)
// update() : recalcula pose_ con la posición y orientación actuales (cada cuadro)
// collect() : combina pose_ * local_ y llena la lista de RenderItem (cada cuadro)

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
    Mesh fuselaje_;       // cilindro central
    Mesh nariz_;          // cono delantero
    Mesh cola_fus_;       // cono trasero
    Mesh ala_;
    Mesh empenaje_h_;
    Mesh empenaje_v_; 

    // Matrices locales
    // Orden: fuselaje, nariz, cola_fus, ala_izq, ala_der, emp_h_izq, emp_h_der, emp_v
    std::vector<glm::mat4> locales_;

    // punteros a la malla que corresponde a cada entrada de locales_ 
    std::vector<const Mesh*> meshes_;

    // Pose global del avión 
    glm::mat4 pose_ {1.0f};
};