#pragma once

#include <vector>

struct Vertex {
    float px, py, pz; // Posición
    float r, g, b;    // Color
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; // vacío si la malla no es indexada
};
