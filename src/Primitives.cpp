#include "Primitives.h"
#include <cmath>

namespace primitives {

static constexpr float PI = 3.14159265358979323846f;

MeshData cube(float sx, float sy, float sz)
{
    float x = sx * 0.5f;
    float y = sy * 0.5f;
    float z = sz * 0.5f;

    MeshData data;

    data.vertices = {
        // FRENTE (+Z)
        {{-x,-y, z}, {0,0,1}, {0,0}},
        {{ x,-y, z}, {0,0,1}, {1,0}},
        {{ x, y, z}, {0,0,1}, {1,1}},
        {{-x, y, z}, {0,0,1}, {0,1}},

        // ATRÁS (-Z)
        {{ x,-y,-z}, {0,0,-1}, {0,0}},
        {{-x,-y,-z}, {0,0,-1}, {1,0}},
        {{-x, y,-z}, {0,0,-1}, {1,1}},
        {{ x, y,-z}, {0,0,-1}, {0,1}},

        // DERECHA (+X)
        {{ x,-y, z}, {1,0,0}, {0,0}},
        {{ x,-y,-z}, {1,0,0}, {1,0}},
        {{ x, y,-z}, {1,0,0}, {1,1}},
        {{ x, y, z}, {1,0,0}, {0,1}},

        // IZQUIERDA (-X)
        {{-x,-y,-z}, {-1,0,0}, {0,0}},
        {{-x,-y, z}, {-1,0,0}, {1,0}},
        {{-x, y, z}, {-1,0,0}, {1,1}},
        {{-x, y,-z}, {-1,0,0}, {0,1}},

        // ARRIBA (+Y)
        {{-x, y, z}, {0,1,0}, {0,0}},
        {{ x, y, z}, {0,1,0}, {1,0}},
        {{ x, y,-z}, {0,1,0}, {1,1}},
        {{-x, y,-z}, {0,1,0}, {0,1}},

        // ABAJO (-Y)
        {{-x,-y,-z}, {0,-1,0}, {0,0}},
        {{ x,-y,-z}, {0,-1,0}, {1,0}},
        {{ x,-y, z}, {0,-1,0}, {1,1}},
        {{-x,-y, z}, {0,-1,0}, {0,1}},
    };

    data.indices = {
         0,  1,  2,   0,  2,  3,
         4,  5,  6,   4,  6,  7,
         8,  9, 10,   8, 10, 11,
        12, 13, 14,  12, 14, 15,
        16, 17, 18,  16, 18, 19,
        20, 21, 22,  20, 22, 23,
    };

    return data;
}

// Cilindro orientado sobre el eje Y, centrado en el origen.
// Ángulos internos en radianes.
// Lateral: (anillos+1) filas × (gajos+1) columnas de vértices.
MeshData cylinder(float radio, float largo, unsigned gajos, unsigned anillos)
{
    MeshData data;

    const unsigned N    = gajos;
    const unsigned cols = N + 1; // vértices por anillo (costura duplicada)

    // LATERAL 
    for (unsigned ring = 0; ring <= anillos; ++ring) {
        float t = static_cast<float>(ring) / static_cast<float>(anillos);
        float y = -largo * 0.5f + t * largo; // de -L/2 a +L/2
        float v = t;

        for (unsigned i = 0; i <= N; ++i) {
            float th = 2.0f * PI * static_cast<float>(i) / static_cast<float>(N);

            Vertex vert;
            vert.position   = { radio * std::cos(th), y, radio * std::sin(th) };
            vert.normal     = { std::cos(th), 0.0f, std::sin(th) }; // radial, Y=0
            vert.tex_coords = { static_cast<float>(i) / static_cast<float>(N), v };
            data.vertices.push_back(vert);
        }
    }

    // Índices
    for (unsigned ring = 0; ring < anillos; ++ring) {
        for (unsigned i = 0; i < N; ++i) {
            unsigned b0 = ring       * cols + i;
            unsigned b1 = b0 + 1;
            unsigned t0 = (ring + 1) * cols + i;
            unsigned t1 = t0 + 1;

            data.indices.insert(data.indices.end(), {b0, t1, b1});
            data.indices.insert(data.indices.end(), {b0, t0, t1});
        }
    }

    // TAPAS
    for (int lado = 0; lado < 2; ++lado) {
        float sign = (lado == 0) ? -1.0f : 1.0f;
        float py   = sign * largo * 0.5f;
        glm::vec3 norm = { 0.0f, sign, 0.0f };

        unsigned centerIdx = static_cast<unsigned>(data.vertices.size());
        data.vertices.push_back({ {0.0f, py, 0.0f}, norm, {0.5f, 0.5f} });

        unsigned rimStart = static_cast<unsigned>(data.vertices.size());
        for (unsigned i = 0; i <= N; ++i) {
            float th   = 2.0f * PI * static_cast<float>(i) / static_cast<float>(N);
            float cosT = std::cos(th);
            float sinT = std::sin(th);
            data.vertices.push_back({
                { radio * cosT, py, radio * sinT },
                norm,
                { cosT * 0.5f + 0.5f, sinT * 0.5f + 0.5f }
            });
        }

        for (unsigned i = 0; i < N; ++i) {
            unsigned a = rimStart + i;
            unsigned b = rimStart + i + 1;
            if (lado == 0) {
                // tapa inferior: normal hacia -Y, orden invertido
                data.indices.insert(data.indices.end(), {centerIdx, b, a});
            } else {
                // tapa superior: normal hacia +Y, orden directo
                data.indices.insert(data.indices.end(), {centerIdx, a, b});
            }
        }
    }

    return data;
}

// Cono orientado sobre el eje Y: ápice en +Y, base en -Y, centrado en el origen.
// 'conicidad' es el ángulo de apertura total α en GRADOS.
// Normal lateral inclinada por el semiángulo α/2 según la presentación:
//   n_lat = (cosθ·cos(α/2), sin(α/2), sinθ·cos(α/2))
// El ápice no comparte vértices con el lateral (normal axial vs inclinada).
// Normal del ápice: (0, 1, 0).
MeshData cone(float radio, float conicidad, unsigned gajos, unsigned anillos)
{
    MeshData data;

    const unsigned N    = gajos;
    const unsigned cols = N + 1;

    // Semiángulo en radianes
    const float alpha     = conicidad * (PI / 180.0f);
    const float half_a    = alpha * 0.5f;
    const float cos_half  = std::cos(half_a);
    const float sin_half  = std::sin(half_a);

    // Altura total: h = r / tan(α/2)
    const float altura = radio / std::tan(half_a);
    const float y_apex = +altura * 0.5f; // ápice arriba
    const float y_base = -altura * 0.5f; // base abajo

    // LATERAL, anillos desde la base hasta el ápice
    // En cada anillo el radio se reduce linealmente hasta 0 en el ápice
    for (unsigned ring = 0; ring <= anillos; ++ring) {
        float t      = static_cast<float>(ring) / static_cast<float>(anillos);
        float r_ring = radio * (1.0f - t);          // radio decrece hacia el ápice
        float y      = y_base + t * altura;
        float v      = t;

        for (unsigned i = 0; i <= N; ++i) {
            float th   = 2.0f * PI * static_cast<float>(i) / static_cast<float>(N);
            float cosT = std::cos(th);
            float sinT = std::sin(th);

            Vertex vert;
            vert.position   = { r_ring * cosT, y, r_ring * sinT };
            vert.normal     = { cosT * cos_half, sin_half, sinT * cos_half };
            vert.tex_coords = { static_cast<float>(i) / static_cast<float>(N), v };
            data.vertices.push_back(vert);
        }
    }

    // Índices del lateral
    for (unsigned ring = 0; ring < anillos; ++ring) {
        for (unsigned i = 0; i < N; ++i) {
            unsigned b0 = ring       * cols + i;
            unsigned b1 = b0 + 1;
            unsigned t0 = (ring + 1) * cols + i;
            unsigned t1 = t0 + 1;

            data.indices.insert(data.indices.end(), {b0, t1, b1});
            data.indices.insert(data.indices.end(), {b0, t0, t1});
        }
    }

    // ÁPICE, un vértice por gajo 
    unsigned apexStart = static_cast<unsigned>(data.vertices.size());
    for (unsigned i = 0; i <= N; ++i) {
        Vertex vert;
        vert.position   = { 0.0f, y_apex, 0.0f };
        vert.normal     = { 0.0f, 1.0f, 0.0f };
        vert.tex_coords = { static_cast<float>(i) / static_cast<float>(N), 1.0f };
        data.vertices.push_back(vert);
    }

    // El anillo de la base del lateral (ring == 0, la fila de vértices de abajo)
    // Los triángulos van desde la base hacia el ápice
    // Usamos el último anillo del lateral (ring == anillos) como base del abanico
    unsigned topRingStart = static_cast<unsigned>(anillos * cols);
    for (unsigned i = 0; i < N; ++i) {
        unsigned b0   = topRingStart + i;
        unsigned b1   = b0 + 1;
        unsigned apex = apexStart + i; // cada gajo tiene su propio vértice del ápice

        data.indices.insert(data.indices.end(), {b0, apex, b1});
    }

    // TAPA BASE, normal axial hacia -Y, no comparte vértices con lateral
    glm::vec3 norm_base = { 0.0f, -1.0f, 0.0f };

    unsigned centerIdx = static_cast<unsigned>(data.vertices.size());
    data.vertices.push_back({ {0.0f, y_base, 0.0f}, norm_base, {0.5f, 0.5f} });

    unsigned rimStart = static_cast<unsigned>(data.vertices.size());
    for (unsigned i = 0; i <= N; ++i) {
        float th   = 2.0f * PI * static_cast<float>(i) / static_cast<float>(N);
        float cosT = std::cos(th);
        float sinT = std::sin(th);
        data.vertices.push_back({
            { radio * cosT, y_base, radio * sinT },
            norm_base,
            { cosT * 0.5f + 0.5f, sinT * 0.5f + 0.5f }
        });
    }

    for (unsigned i = 0; i < N; ++i) {
        unsigned a = rimStart + i;
        unsigned b = rimStart + i + 1;
        // normal hacia -Y, orden invertido para antihorario desde afuera
        data.indices.insert(data.indices.end(), {centerIdx, b, a});
    }

    return data;
}

} // namespace primitives
