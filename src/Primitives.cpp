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

/**
 * Genera la superficie lateral de un cilindro paramétrico sobre el eje Y (sin tapas).
 * 
 * - **Geometría y Normales:** Centrado en el origen, de altura `largo`. La normal de la 
 *   superficie es radial en el plano XZ: n = (cos θ, 0, sin θ).
 * - **Discretización:** Grilla de (anillos + 1) × (gajos + 1) vértices. Duplica la 
 *   columna final (i == N) para cerrar la textura (UV) sin discontinuidades.
 */
MeshData cylinder(float radio, float largo, unsigned gajos, unsigned anillos)
{
    MeshData data;

    const unsigned N    = gajos;
    const unsigned cols = N + 1; // Vértices por anillo (costura UV duplicada)

    // 1. GENERACIÓN DE VÉRTICES (Superficie Lateral)
    for (unsigned ring = 0; ring <= anillos; ++ring) {
        float t = static_cast<float>(ring) / static_cast<float>(anillos);
        float y = -largo * 0.5f + t * largo; // Mapeo de -L/2 a +L/2
        float v = t;

        for (unsigned i = 0; i <= N; ++i) {
            float th   = 2.0f * PI * static_cast<float>(i) / static_cast<float>(N);
            float cosT = std::cos(th);
            float sinT = std::sin(th);

            Vertex vert;
            vert.position   = { radio * cosT, y, radio * sinT };
            vert.normal     = { cosT, 0.0f, sinT }; // Normal radial
            vert.tex_coords = { static_cast<float>(i) / static_cast<float>(N), v };
            
            data.vertices.push_back(vert);
        }
    }

    // 2. GENERACIÓN DE ÍNDICES (Malla de Triángulos)
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

    return data;
}

/**
 * Genera la superficie lateral de un cono paramétrico sobre el eje Y (sin tapa).
 * 
 * - **Ápice y Atributos:** Aunque la posición 3D colapsa en el punto superior, los
 *   vértices se duplican por gajo (ring == anillos) para mantener la normal inclinada
 *   independiente n_lat = (cos θ · cos(α/2), sin(α/2), sin θ · cos(α/2)).
 * - **Dimensiones:** La altura se calcula como h = radio / tan(α / 2).
 */
MeshData cone(float radio, float conicidad, unsigned gajos, unsigned anillos)
{
    MeshData data;

    const unsigned N    = gajos;
    const unsigned cols = N + 1;

    const float alpha     = conicidad * (PI / 180.0f);
    const float half_a    = alpha * 0.5f;
    const float cos_half  = std::cos(half_a);
    const float sin_half  = std::sin(half_a);

    const float altura = radio / std::tan(half_a);
    const float y_base = -altura * 0.5f;

    // Vértices del lateral (desde la base hasta el ápice)
    for (unsigned ring = 0; ring <= anillos; ++ring) {
        float t      = static_cast<float>(ring) / static_cast<float>(anillos);
        float r_ring = radio * (1.0f - t);
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

    // Índices de las caras triangulares
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

    return data;
}

} // namespace primitives
