#include "Primitives.h"
#include <cmath>
#include <numbers> 

namespace primitives {

MeshData cube(float sx, float sy, float sz)
{
    float x = sx * 0.5f;
    float y = sy * 0.5f;
    float z = sz * 0.5f;

    MeshData data;

    // Cada cara tiene su propia normal plana.
    // Las tex_coords van de (0,0) abajo-izquierda a (1,1) arriba-derecha
    // siguiendo el orden antihorario de cada cuadrilátero.

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
         0,  1,  2,   0,  2,  3,  // frente
         4,  5,  6,   4,  6,  7,  // atrás
         8,  9, 10,   8, 10, 11,  // derecha
        12, 13, 14,  12, 14, 15,  // izquierda
        16, 17, 18,  16, 18, 19,  // arriba
        20, 21, 22,  20, 22, 23,  // abajo
    };

    return data;
}

} // namespace primitives
